#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
#include<pthread.h> 
#include<time.h>
#include <stdbool.h>

#include<sys/sendfile.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#define MAX_CLIENTS 9
#define MIN_CLIENTS 1
#define MAX_SIZE_ARR 10
#define MIN_SIZE_ARR 1
#define MAX_VALUE 100
#define MIN_VALUE 1

#define BUFSIZ 1024

int sizeArr = 0;
int * arr;
int idx = 0;
pthread_mutex_t lock;

int * idArr;

struct client_info
{
    bool isActive;
    int sum;
};

struct client_info clients[MAX_CLIENTS];

void init_clients()
{
    for(int i=0;i<MAX_CLIENTS;i++)
    {
        clients[i].isActive=false;
        clients[i].sum=0;
    }
}

int register_clients()
{
    int i;
    for(i=0;i<MAX_CLIENTS;i++)
    {
        if(clients[i].isActive==false)
        {
            clients[i].isActive=true;
            clients[i].sum=0;
            break;
        }
    }
    return i;
}

int count_client_active()
{
    int count = 0;
    for(int i=0;i<MAX_CLIENTS;i++)
    {
        if(clients[i].isActive==true)
        {
            count++;
        }
    }
    return count;
}

void set_client_die(int i)
{
    clients[i].isActive=false;
    clients[i].sum=0;
}

void set_client_sum(int i, int sum)
{
    clients[i].sum=sum;
}

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

void printArr()
{
    for(int i=0;i<sizeArr;i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int calSum(char * filename)
{
    int sum=0;
    FILE *f;
    f=fopen(filename,"r");
    if(!f)
    {
        printf("Can not open %s!\n", filename);
        return;
    }
    fseek(f, 0L, SEEK_SET); 
    int value;
    while(fscanf(f,"%d-",&value) == 1)
    {
        sum+=value;
    }
    fclose(f);
    return sum;
}

void rankingAndSendToCLient(char * filename, int socket)
{
    int rank[MAX_CLIENTS] = {0}; 
      
    for (int i = 0; i < MAX_CLIENTS - 1; i++) { 
        int r = 1, s = 1; 
          
        for (int j = 0; j < MAX_CLIENTS; j++) { 
            if(clients[j].isActive == true)
            {
                if (j != i && clients[j].sum < clients[i].sum) 
                    r += 1; 
                  
                if (j != i && clients[j].sum == clients[i].sum) 
                    s += 1;      
            }
        } 
          
        rank[i] = (int)(r + (float)(s - 1) / (float) 2); 
    }  
    FILE *f = fopen(filename,"w");
    if(!f)
    {
        printf("Can not open file rank!\n");
        return;
    }
    for(int i = 0;i<MAX_CLIENTS;i++)
    {
        if(clients[i].isActive==true)
        {
            fprintf(f, "Client ID : %d - Rank : %d\n",i,rank[i]);
        }
    }
    fclose(f);
    sendFile(filename, socket);
}

char * getElementFromArr()
{
    char * str = (char*)malloc(100*sizeof(char));
    if(count_client_active()>=MIN_CLIENTS && count_client_active()<=MAX_CLIENTS)
    {
        if(idx >= sizeArr)
        {
            sprintf(str,"full");
        }
        else
        {
            sprintf(str,"%d",arr[idx++]);
        }   
    }
    else
    {
        sprintf(str,"Number of clients must be in range (3-9)!!!");
    }
    return str;
}

void initRandomArr()
{
    srand(time(0));
    sizeArr = (rand() % (MAX_SIZE_ARR - MIN_SIZE_ARR + 1)) + MIN_SIZE_ARR;

    arr = (int *)malloc(sizeArr * sizeof(int));
    int i;
    for(i=0;i<sizeArr;i++)
    {
        arr[i] = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
    }
}

void *server_handler (void *fd_pointer)
{
	int sock = *(int *)fd_pointer;

    int read_size, write_size;

    char message[1024];

    char id[10];
    int code = register_clients();
    sprintf(id,"%d",code);
    write(sock,id,strlen(id)+1);
    
    while((read_size = recv(sock,message,1024,0)) > 0)
    {
        if(strcmp(message,"get")==0)
        {
            
            pthread_mutex_lock(&lock);
            char * value = getElementFromArr();
            pthread_mutex_unlock(&lock); 
            send(sock,value,strlen(value)+1,0);
            free(value);
        }
        else if (strcmp(message,"auto")==0)
        {
            while(1)
            {
                pthread_mutex_lock(&lock);
                char * value = getElementFromArr();
                
                pthread_mutex_unlock(&lock); 
                send(sock,value,strlen(value)+1,0);
                
                
                if(strcmp(value,"full")==0 || strcmp(value,"Number of clients must be in range (3-9)!!!")==0)
                {
                    free(value);

                    break;
                }
                free(value);
                sleep(1);
                
            }
        }
        else if(strcmp(message,"send")==0)
        {
            //receive file from client
            recv(sock,message,1024,0);
            char filename[100];
            strcpy(filename,message);
            strcat(filename,"-server");
            receiveFile(filename, sock);

            set_client_sum(code, calSum(filename));

            rankingAndSendToCLient("filerank.txt", sock);
            
        }
        else
        {
            write(sock,"???",4);
        }
    }
    
    if(read_size == 0)
    {
        set_client_die(code);
        printf("Client %d disconnected\n", code);
        fflush(stdout);
    }
	else if(read_size == -1)
    {
        set_client_die(code);
        printf("recv failed at client %d\n", code);
    }
    free(fd_pointer);
    return 0;
}

int main()
{
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\nMutex init has failed\n");
        return 1;
    }

    idArr = (int*)calloc(MAX_CLIENTS,sizeof(int));

    initRandomArr();

    printArr();

    init_clients();

    int listenfd, connfd, *new_sock;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr; 

    listenfd = socket(AF_INET,SOCK_STREAM,0);
   
    if (listenfd == -1)
    {
	   perror("Could not create Socket \n"); 
    }
   
   puts("Socket Created");
   
   bzero(&servaddr,sizeof (servaddr));
   servaddr.sin_family = AF_INET;
   //servaddr.sin_addr.s_addr = INADDR_ANY;
   servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
   servaddr.sin_port = htons(8888);
   
   if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
   {
	   perror("bind failed");
	   return 1;
   }
   
   puts("bind success");
   listen(listenfd, 50);
   
   puts("Waiting for connections ...\n");
   clilen = sizeof(cliaddr);

    while ((connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen)))
    {
            puts("Connection accepted");
            
            pthread_t server_thread;
            new_sock = malloc(1);
            *new_sock = connfd;
            if(pthread_create(&server_thread,NULL,server_handler,(void*) new_sock)!=0)
            {
                printf("Failed to create thread\n");
            }
    }
    
    pthread_mutex_destroy(&lock);

    if (connfd < 0)
    {
        perror("Accept Failed");
        return 1;
    }
        
    return 0;
}

