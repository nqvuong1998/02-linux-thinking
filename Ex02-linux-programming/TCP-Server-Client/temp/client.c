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

#define BUFFER_SIZE 1024
#define ADDRESS "127.0.0.1"
#define PORT 8888

struct ArrClient
{
    int * arr;
    int sizeArr;
};

struct ArrClient arrClient = {NULL,0};

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
    memset(file_size,'\0',256);
    sprintf(file_size, "%d", file_stat.st_size);

    /* Sending file size */
    int len = send(socket, file_size, BUFSIZ, 0);
    
    if (len < 0)
    {
        printf("Error on sending greetings --> %s", strerror(errno));
        return;
    }
    
    int remain_data = file_stat.st_size;
    //printf("File size: %d\n",remain_data);
    int sent_bytes = 0;
    int offset = 0;
    /* Sending file data */
    while (((sent_bytes = sendfile(socket, fd, 0, BUFSIZ)) > 0) && (remain_data > 0))
    {
        remain_data -= sent_bytes;
        //printf("file %s - len %d - bufsiz %d\n",filename, sent_bytes,BUFSIZ);
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
    //printf("File size: %d\n",remain_data);
    int len;
    
    // memset( buffer, 0, sizeof(char)*BUFSIZ );
    // while ((remain_data > 0) && (len = recv(socket, buffer, BUFSIZ, 0)) > 0)
    // {
    //     fwrite(buffer, sizeof(char), len, f);
    //     //printf("file %s - len %d - bufferlen %d - bufsiz %d\n",filename, len,strlen(buffer),BUFSIZ);
    //     remain_data -= len;
    //     memset( buffer, 0, sizeof(char)*BUFSIZ );
    // }
    if(remain_data<=0)return true;

    do{
        memset( buffer, 0, sizeof(char)*BUFSIZ );
        len = recv(socket, buffer, BUFSIZ, 0);
        if(len <= 0)return false;
        fwrite(buffer, sizeof(char), len, f);
        remain_data -= len;

    }while(remain_data>0);
    fclose(f);
    return true;
}

void insertSorted(int *arr, int n, int key) 
{  
    int i; 
    for (i=n-1; ( i >= 0  && arr[i] > key); i--) 
       arr[i+1] = arr[i]; 
  
    arr[i+1] = key;
}

void writeFile(char * filename, int num)
{
    arrClient.arr=(int*)realloc(arrClient.arr, sizeof(int)*(arrClient.sizeArr+1));
    insertSorted(arrClient.arr, arrClient.sizeArr, num);
    arrClient.sizeArr++;
    FILE * f = fopen(filename,"w");

    if(!f)
    {
        printf("Can not open %s!\n", filename);
        return;
    }

    for(int i=0;i<arrClient.sizeArr;i++)
    {
        fprintf(f,"%d\n",arrClient.arr[i]);
    }

    fclose(f);
}
 
 int main()
 {
    arrClient.arr = (int *)malloc(sizeof(int)*1);
    int sockfd;
    char buffer[1024];
    char server_reply[1024];
    int n;
  
    struct sockaddr_in servaddr; 

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd == -1)
    {
        perror("Could not create socket");
    }
	
    printf("Created Socket \n");
    bzero(&servaddr,sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(ADDRESS);
   
    connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr));

    char id[50];
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
    }
    else
    {
        puts("Error recv ID");
        close(sockfd);
        return 1;
    }
    
    int num=0, read_size=0;
    int count_request = 0;

    while (1)
    {
        send(sockfd, "get",1024,0);

        memset(server_reply, '\0', 1024);
        if(recv(sockfd,server_reply,1024,0 ) <= 0)
        {
            puts("Error get data");
            break;
        }
        
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
                puts("Error recv file rank");
                close(sockfd);
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
        //printf("Server Reply: %d - count %d\n",num, count_request );
        writeFile(filename,num);
    }

	close(sockfd);
	return 0;
	 
 }
