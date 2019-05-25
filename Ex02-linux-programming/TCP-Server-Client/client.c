#include<stdio.h> 
#include<stdlib.h>
#include<string.h>    
#include<sys/socket.h>    
#include<arpa/inet.h> 
#include<stdbool.h>

#include<sys/sendfile.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

void sendFile(char * filename, int socket)
{
    int fd = open(filename,O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file --> %s", strerror(errno));
        return;
    }
    /* Get file stats */
    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0)
    {
        printf("Error fstat --> %s", strerror(errno));
        return;
    }

    char file_size[256];
    sprintf(file_size, "%d", file_stat.st_size);

    /* Sending file size */
    int len = send(socket, file_size, BUFSIZ, 0);
    
    if (len < 0)
    {
        printf("Error on sending greetings --> %s", strerror(errno));
        return;
    }

    int remain_data = file_stat.st_size;
    int sent_bytes = 0;
    /* Sending file data */
    while (((sent_bytes = sendfile(socket, fd, 0, BUFSIZ)) > 0) && (remain_data > 0))
    {
        remain_data -= sent_bytes;
    }
}

bool receiveFile(char * filename, int socket)
{
    char buffer[BUFSIZ];
    memset( buffer, '\0', sizeof(char)*BUFSIZ );
    if(recv(socket, buffer, BUFSIZ, 0) < 0)
    {
        return false;
    }
    int file_size = atoi(buffer);

    FILE * f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Failed to open file foo --> %s\n", strerror(errno));
        return false;
    }

    int remain_data = file_size;
    ssize_t len;
    memset( buffer, '\0', sizeof(char)*BUFSIZ );
    while ((remain_data > 0) && (len = recv(socket, buffer, BUFSIZ, 0) > 0))
    {
        fwrite(buffer, sizeof(char), strlen(buffer), f);
        remain_data -= strlen(buffer)*sizeof(char);
        memset( buffer, '\0', sizeof(char)*BUFSIZ );
    }
    fclose(f);
    return true;
}

void writeFile(char * filename, char * newname, int num)
{
    FILE *f,*tmp;
    f=fopen(filename,"a+");
    tmp=fopen(newname,"w");
    if(!f)
    {
        printf("Can not open %s!\n", filename);
        return;
    }
    if(!tmp)
    {
        printf("Can not open %s!\n", newname);
        return;
    }
    fseek(f, 0L, SEEK_END);
    int len = ftell(f);

    fseek(f, 0L, SEEK_SET);
    int value;

    if(len>0)
    {
        bool flag = false;
        while(fscanf(f,"%d\n",&value) == 1)
        {
            if(value>=num && flag == false)
            {
                fprintf(tmp,"%d\n",num);
                flag = true;
            }
            fprintf(tmp,"%d\n",value);
        }
        
        remove(filename);
        rename(newname,filename);
        remove(newname);
    }
    else
    {
        fprintf(f,"%d\n",num);
        remove(newname);
    }
    fclose(f);
    fclose(tmp);
}
 
 int main()
 {
    
    int sockfd;
    char buffer[1024];
    char server_reply[1024];
    ssize_t n;
  
    struct sockaddr_in servaddr; 

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd == -1)
    {
        perror("Could not create socket");
    }
	
    printf("Created Socket \n");
    bzero(&servaddr,sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
   
    connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr));

    char id[50];
    char newname[100];
    char filename[100];
	if(recv(sockfd,id,1024,0 ) > 0)
    {
        if(strcmp(id,"Over clients")==0)
        {
            printf("Over clients\n");
            return 1;
        }
        printf("Client ID: %s\n",id);

        strcpy(filename,id);
        strcat(filename,"-client.txt");
        
        strcpy(newname,filename);
        strcat(newname, ".out");
        remove(filename);
        FILE * f =fopen(filename,"w");
        fclose(f);
    }
    else
    {
        puts("Error");
        return 1;
    }
    
    int num=0, read_size=0;
    int count_request = 0;

    while (1)
    {
        send(sockfd, "get",1024,0);

        memset(server_reply, '\0', 1024);
        if(recv(sockfd,server_reply,1024,0 ) < 0)
        {
            puts("Error");
            break;
        }
        //printf("Server Reply: %s \n",server_reply );
        if(strcmp(server_reply,"full")==0)
        {
            printf("\nCount request: %d\n",count_request);
            //send file from client
            sendFile(filename,sockfd);

            char filerank[100];
            strcpy(filerank,"filerank-");
            strcat(filerank,id);
            strcat(filerank,"-client.txt");
            if(receiveFile(filerank, sockfd) == false)
            {
                puts("Error");
                break;
            }

            printf("File Rank!\n");
            return 0;
            //break;
        }

        if(strcmp(server_reply,"Not in range clients")==0)
        {
            printf("%s\n",server_reply);
            continue;
        }
        count_request++;
        num=atoi(server_reply);
        writeFile(filename,newname,num);
    }

	close(sockfd);
	return 0;
	 
 }
