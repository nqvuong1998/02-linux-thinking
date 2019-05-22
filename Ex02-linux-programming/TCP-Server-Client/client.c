#include<stdio.h> 
#include<stdlib.h>
#include<string.h>    
#include<sys/socket.h>    
#include<arpa/inet.h> 
#include <stdbool.h>

#include<sys/sendfile.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#define BUFSIZ 1024

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

void receiveFile(char * filename, int socket)
{
    char buffer[BUFSIZ];
    recv(socket, buffer, BUFSIZ, 0);
    int file_size = atoi(buffer);

    FILE * f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Failed to open file foo --> %s\n", strerror(errno));
        return;
    }

    int remain_data = file_size;
    ssize_t len;
    while ((remain_data > 0) && ((len = recv(socket, buffer, BUFSIZ, 0)) > 0))
    {
        fwrite(buffer, sizeof(char), len, f);
        remain_data -= len;
    }
    fclose(f);
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
    int count = 0;
    int value;

    if(len>0)
    {
        while(fscanf(f,"%d-",&value) == 1)
        {
            if(value>=num)
            {
                break;
            }
            count+=1;
        }
        fseek(f, 0L, SEEK_SET);
        fseek(tmp, 0L, SEEK_SET);
        for(int i=0;i<count;i++)
        {
            fscanf(f,"%d-",&value);
            fprintf(tmp,"%d-",value);
        }
        fprintf(tmp,"%d-",num);
        while(fscanf(f,"%d-",&value) == 1)
        {
            fprintf(tmp,"%d-",value);
        }
        remove(filename);
        rename(newname,filename);
        remove(newname);
    }
    else
    {
        fprintf(f,"%d-",num);
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

    char id[30];
    char newname[100];
	if(recv(sockfd,id,1024,0 ) >= 0)
    {
        printf("Client %s\n",id);
        strcat(id,".txt-client");
        
        strcpy(newname,id);
        strcat(newname, "-tmp");
        remove(id);
    }
    while (1)
    {
	    printf("Enter a Message: ");
        scanf("%s",buffer);
	
        if (send(sockfd,buffer,strlen(buffer)+1,0) < 0)
        {
            printf("Error \n");
            return 1;
        }
        int read_size;
        int num;
        if(strcmp(buffer,"auto")==0)
        {
            read_size=recv(sockfd,server_reply,1024,0 );
            while (read_size > 0)
            {
                printf("Server Reply: %s \n",server_reply );
                if(strcmp(server_reply,"full")==0)
                {
                    //send file from client
                    send(sockfd,"send",1024,0);
                    send(sockfd,id,1024,0);
                    sendFile(id,sockfd);

                    receiveFile("filerank.txt", sockfd);

                    printf("File Rank!!!\n");

                    break;
                }
                if(strcmp(server_reply,"Number of clients must be in range (3-9)!!!")==0)
                {
                    break;
                }
                num=atoi(server_reply);
                writeFile(id,newname,num);
                read_size=recv(sockfd,server_reply,1024,0 );
            }
            
        }
        else if(strcmp(buffer,"get")==0)
        {
            read_size=recv(sockfd,server_reply,1024,0 );
            if(read_size < 0)
            {
                puts("Error");
                break;
            }
            printf("Server Reply: %s \n",server_reply );
            if(strcmp(server_reply,"full")==0)
            {
                //send file from client
                send(sockfd,"send",1024,0);
                send(sockfd,id,1024,0);
                sendFile(id,sockfd);

                char filerank[100];
                recv(sockfd,filerank,1024,0);
                
                //receiveFile(filerank, sockfd);

                //printf("File Rank!!!\n");
            }
            else if(strcmp(server_reply,"Number of clients must be in range (3-9)!!!")!=0)
            {
                    num=atoi(server_reply);
                    writeFile(id,newname,num);
            }
            
        }
        else 
        {
            if(recv(sockfd,server_reply,1024,0 ) < 0)
            {
                puts("Error");
                break;
            }
            printf("Server Reply: %s \n",server_reply );
        }
        
   }
	close(sockfd);
	return 0;
	 
 }
