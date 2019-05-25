#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
#include<pthread.h> 
#include<time.h>
#include<stdbool.h>

#include<sys/sendfile.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#define MAX_CLIENTS 9
#define MIN_CLIENTS 1
#define MAX_SIZE_ARR 10000
#define MIN_SIZE_ARR 10000
#define MAX_VALUE 100000
#define MIN_VALUE 90000

#define BUFFER_SIZE 1024

int sizeArr = 0;
int * arr;
int idx = 0;
pthread_mutex_t lock;

struct client_info
{
    bool isActive;
    int sum;
    bool isSendFile;
    pthread_t thread;
    int socket;
};

struct client_info clients[MAX_CLIENTS];
int countClients = 0;

void init_clients()
{
    for(int i=0;i<MAX_CLIENTS;i++)
    {
        clients[i].isActive=false;
        clients[i].sum=0;
        clients[i].isSendFile=false;
        clients[i].thread = NULL;
        clients[i].socket = -1;
    }
}

int register_clients(pthread_t thread, int socket)
{
    int i;
    for(i=0;i<MAX_CLIENTS;i++)
    {
        if(clients[i].isActive==false)
        {
            clients[i].isActive=true;
            clients[i].sum=0;
            clients[i].isSendFile=false;
            clients[i].thread = thread;
            clients[i].socket = socket;
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
    clients[i].isSendFile=false;
    clients[i].thread = NULL;
    clients[i].socket = -1;
}

void set_client_send_file(int i)
{
    clients[i].isSendFile=true;
}

void set_client_sum(int i, int sum)
{
    clients[i].sum=sum;
}

bool isSendAllFiles()
{
    for(int i=0;i<MAX_CLIENTS;i++)
    {
        if(clients[i].isActive==true)
        {
            if(clients[i].isSendFile==false)
            {
                return false;
            }
        }
    }
    return true;
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

void printInfo()
{
    printf("Range clients: %d - %d\n",MIN_CLIENTS,MAX_CLIENTS);
    printf("Size arr: %d\n",sizeArr);
    printf("\n--------------\n");
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
    while(fscanf(f,"%d\n",&value) == 1)
    {
        sum+=value;
    }
    fclose(f);
    return sum;
}

void rankingAndSendToCLient(char * filename, int socket)
{
    if( access(filename, F_OK) != -1 ) 
    {
        // file exists
        sendFile(filename, socket);
        return;
    } 
      
    int rank[MAX_CLIENTS] = {0};

    for (int i = 0; i < MAX_CLIENTS; i++) { 
        int r = 1, s = 1; 
        if(clients[i].isActive==true)
        { 
            for (int j = 0; j < MAX_CLIENTS; j++) { 
                if(clients[j].isActive == true)
                {
                    if (j != i && clients[j].sum > clients[i].sum) 
                        r += 1; 
                    
                    if (j != i && clients[j].sum == clients[i].sum) 
                        s += 1;      
                }
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
            fprintf(f, "Client ID : %d - Sum: %d - Rank : %d\n",i,clients[i].sum,rank[i]);
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
        sprintf(str,"Not in range clients");
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

void *ranking_handler(void * context)
{
    while (idx < sizeArr)
    {
    }
    
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        if(clients[i].isActive == true)
        {
            pthread_join(clients[i].thread, NULL);
        }
    }
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        if(clients[i].isActive == true)
        {
            rankingAndSendToCLient("filerank-server.txt", clients[i].socket);
        }
    }
    printf("File Rank!\n");
    exit(0);
}

void *server_handler (void *fd_pointer)
{
    int sock = *(int *)fd_pointer;
    pthread_mutex_lock(&lock);
    int code = register_clients(pthread_self(), sock);
    countClients++;
    pthread_mutex_unlock(&lock);
    
    int read_size = 0, write_size = 0;

    char message[1024];

    char id[10];

    if(countClients > MAX_CLIENTS) 
    {
        printf("1 client is aborted because of over client!\n");
        send(sock,"Over clients",1024,0);
        pthread_mutex_lock(&lock);
        countClients--;
        pthread_mutex_unlock(&lock);
    }
    else
    {
        sprintf(id,"%d",code);
        send(sock,id,1024,0);
    
        memset( message, '\0', 1024);
        while(read_size = recv(sock, message, 1024, 0) > 0)
        {
            if(strcmp(message,"get")!=0)
            {
                printf("Message wrong at client %d!\n", code);
                continue;
            }

            pthread_mutex_lock(&lock);
            char * value = getElementFromArr();
            send(sock,value,1024,0);
            pthread_mutex_unlock(&lock);  
                
            if(strcmp(value,"full")==0)
            {
                free(value);
                //receive file from client
                 memset( message, '\0', 1024);

                char filename[100];
                strcpy(filename,id);
                strcat(filename,"-server.txt");

                if(receiveFile(filename, sock) == false)
                {
                    puts("Error");
                    break;
                }

                int sum = calSum(filename);
                set_client_sum(code, sum);

                pthread_exit(NULL);
                break;
            }
            
            if(strcmp(value,"Not in range clients")==0)
            {
                free(value);           
                continue;
            }
            free(value);
            memset( message, '\0', 1024);
        }
    }
    
    if(read_size == 0)
    {
        pthread_mutex_lock(&lock);
        set_client_die(code);
        countClients--;
        pthread_mutex_unlock(&lock);
        printf("Client %d disconnected\n", code);
        fflush(stdout);
    }
	else if(read_size == -1)
    {
        
        pthread_mutex_lock(&lock);
        set_client_die(code);
        countClients--;
        pthread_mutex_unlock(&lock);
        printf("recv failed at client %d\n", code);
    }
    free(fd_pointer);
    pthread_exit(NULL);
    return 0;
}

int main()
{
    remove("filerank.txt-server");
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\nMutex init has failed\n");
        return 1;
    }

    initRandomArr();

    printInfo();

    init_clients();

    int listenfd, connfd, *new_sock;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr; 

    listenfd = socket(AF_INET,SOCK_STREAM,0);

    //non-blocking
    //int flags = fcntl(listenfd, F_GETFL);
    //fcntl(listenfd, F_SETFL, flags | O_NONBLOCK);
    ////fcntl(listenfd, F_SETFL, O_NONBLOCK);

   
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

    pthread_t ranking_thread;
   pthread_create(&ranking_thread,NULL,ranking_handler,NULL);

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

