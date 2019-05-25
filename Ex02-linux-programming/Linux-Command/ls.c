#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>

void print_date(struct tm inputTime);
 
void print_perms(mode_t st);

void print_info_dir(char path[]);

void print_info_file(char path[]);

int is_dir(const char *path);

int main(int argc, char * argv[]) {
    char pathname[128];
    if(argc == 1){
        getcwd(pathname, 128);
        print_info_dir(pathname);
    }
    else{
        int i;
        for(i = 1;i <= argc - 1;i++){
            if(!is_dir(argv[i])){
                if(strstr(argv[i],"/") == NULL){
                    strcpy(pathname,"./");
                    strcat(pathname,argv[i]);
                }
                else strcpy(pathname,argv[i]);
                print_info_file(pathname);
                memset(pathname, 0, sizeof(pathname));
                printf(" %s\n", argv[i]);
            }
            else {
                if(argc > 2) printf("%s:\n", argv[i]);
                print_info_dir(argv[i]);
            }

            if(i < argc-1 && argc > 2) printf("\n");
        }
    }
    return 0;
}

void print_date(struct tm inputTime){
    time_t timer;
    struct tm * tm_info;
    char curYear[5];
    time(&timer);
    tm_info=localtime(&timer);
    strftime(curYear, sizeof(curYear), "%Y", tm_info);

    char inputYear[5];
    strftime(inputYear, sizeof(inputYear), "%Y", &inputTime);

    char datestring[256];
    if(strcmp(curYear,inputYear) == 0){
        strftime(datestring, sizeof(datestring), "Thg %m %d %H:%M", &inputTime);
    }
    else {
        strftime(datestring, sizeof(datestring), "Thg %m %d  %Y", &inputTime);
    }
    printf(" %15s", datestring);
}
 
void print_perms(mode_t st) {
    char perms[11];
    if(S_ISREG(st)) perms[0]='-';
    else if(S_ISDIR(st)) perms[0]='d';
    else if(S_ISFIFO(st)) perms[0]='p';
    else if(S_ISSOCK(st)) perms[0]='s';
    else if(S_ISCHR(st)) perms[0]='c';
    else if(S_ISBLK(st)) perms[0]='b';
    else if(S_ISLNK(st)) perms[0]='l';
    else perms[0]='?'; 
    perms[1] = (st & S_IRUSR) ? 'r':'-';
    perms[2] = (st & S_IWUSR) ? 'w':'-';
    perms[3] = (st & S_IXUSR) ? 'x':'-';
    perms[4] = (st & S_IRGRP) ? 'r':'-';
    perms[5] = (st & S_IWGRP) ? 'w':'-';
    perms[6] = (st & S_IXGRP) ? 'x':'-';
    perms[7] = (st & S_IROTH) ? 'r':'-';
    perms[8] = (st & S_IWOTH) ? 'w':'-';
    perms[9] = (st & S_IXOTH) ? 'x':'-';
    perms[10] = '\0';
    printf("%s", perms);
}

void print_info_dir(char path[]){
    char tmpPath[128];
    DIR * dir; struct dirent * file; struct stat sbuf;
    char buf[128];
    struct passwd pwent, * pwentp;
    struct group grp, * grpt;
    char datestring[256];
    struct tm time;
    dir = opendir(path);
    if(dir == NULL){
        printf("ls: cannot access '%s': No such file or directory\n", path);
        return;
    }

    int total = 0;
    while(file=readdir(dir)) {
        if(file->d_name[0]=='.') continue;
        strcpy(tmpPath,path);
        strcat(tmpPath,"/");
        strcat(tmpPath,file->d_name);
        stat(tmpPath,&sbuf);
        
        print_perms(sbuf.st_mode);
        printf(" %2d", (int)sbuf.st_nlink);

        if (!getpwuid_r(sbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
            printf(" %s", pwent.pw_name);
        else
            printf(" %d", sbuf.st_uid);
 
        if (!getgrgid_r (sbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
            printf(" %s", grp.gr_name);
        else
            printf(" %d", sbuf.st_gid);

        total += (int)sbuf.st_blocks;

        printf(" %10d", (int)sbuf.st_size);
        
        localtime_r(&sbuf.st_mtime, &time);
        
        print_date(time);

        printf(" %s\n", file->d_name);

        memset(tmpPath, 0, sizeof(tmpPath));
    }

    printf("total %d\n",total/2);
}

void print_info_file(char path[]){
    struct stat sbuf;
    char buf[128];
    struct passwd pwent, * pwentp;
    struct group grp, * grpt;
    char datestring[256];
    struct tm time;

    int isExist = stat(path, &sbuf);

    if(isExist != 0){
        printf("ls: cannot access '%s': No such file or directory\n", path);
        return;
    }

    print_perms(sbuf.st_mode);
    printf(" %2d", (int)sbuf.st_nlink);

    if (!getpwuid_r(sbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
        printf(" %s", pwent.pw_name);
    else
        printf(" %d", sbuf.st_uid);
 
    if (!getgrgid_r (sbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
        printf(" %s", grp.gr_name);
    else
        printf(" %d", sbuf.st_gid);

    printf(" %10d", (int)sbuf.st_size);
        
    localtime_r(&sbuf.st_mtime, &time);
        
    print_date(time);

}

int is_dir(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}