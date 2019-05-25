#include "func.h"

void *ranking_handler(void * context);
void *server_handler (void *fd_pointer);

int main()
{
    remove("filerank-server.txt");
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

    if (listenfd == -1)
    {
	   perror("Could not create Socket \n"); 
    }
   
   puts("Socket Created");
   
   bzero(&servaddr,sizeof (servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(ADDRESS);
   servaddr.sin_port = htons(PORT);
   
   if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
   {
	   perror("bind failed");
	   return 1;
   }
   
   puts("bind success");
   listen(listenfd, LISTEN_PORT);
   
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

void *ranking_handler(void * context)
{
    while (arrServer.idx < arrServer.sizeArr)
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
            rankingAndSendToClient("filerank-server.txt", clients[i].socket);
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

    char message[BUFFER_SIZE];

    char id[10];

    if(countClients > MAX_CLIENTS) 
    {
        printf("1 client is aborted because of over client!\n");
        send(sock,"Over clients",BUFFER_SIZE,0);
        pthread_mutex_lock(&lock);
        countClients--;
        pthread_mutex_unlock(&lock);
    }
    else
    {
        sprintf(id,"%d",code);
        send(sock,id,BUFFER_SIZE,0);
    
        memset( message, '\0', BUFFER_SIZE);
        while(read_size = recv(sock, message, BUFFER_SIZE, 0) > 0)
        {
            if(strcmp(message,"get")!=0)
            {
                printf("Message wrong at client %d!\n", code);
                continue;
            }

            pthread_mutex_lock(&lock);
            char * value = getElementFromArr();
            send(sock,value,BUFFER_SIZE,0);
            pthread_mutex_unlock(&lock);  
                
            if(strcmp(value,"full")==0)
            {
                free(value);

                memset( message, '\0', BUFFER_SIZE);

                char filename[100];
                strcpy(filename,id);
                strcat(filename,"-server.txt");

                if(receiveFile(filename, sock) == false)
                {
                    printf("Error recv file data from client %d\n",code);
                    pthread_mutex_lock(&lock);
                    set_client_die(code);
                    countClients--;
                    pthread_mutex_unlock(&lock);
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
            memset( message, '\0', BUFFER_SIZE);
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

