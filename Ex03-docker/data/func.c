#include "func.h"

pthread_mutex_t lock;
struct ClientInfo clients[MAX_CLIENTS];
struct ArrServer arrServer = {NULL,0,0};
int countClients = 0;
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
    int value;
    while(fscanf(f,"%d\n",&value) > 0)
    {
        sum+=value;
    }
    fclose(f);
    return sum;
}

void rankingAndSendToClient(char * filename, int socket)
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
            //printf("Client ID : %d - Sum: %d - Rank : %d\n",i,clients[i].sum,rank[i]);
        }
    }
    fclose(f);
    sendFile(filename, socket);
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

//------------------------------------------------

void init_clients()
{
    for(int i=0;i<MAX_CLIENTS;i++)
    {
        clients[i].isActive=false;
        clients[i].sum=0;
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
    clients[i].thread = NULL;
    clients[i].socket = -1;
}

void set_client_sum(int i, int sum)
{
    clients[i].sum=sum;
}

//------------------------------------------------

void initRandomArr()
{
    srand(time(0));
    arrServer.sizeArr = (rand() % (MAX_SIZE_ARR - MIN_SIZE_ARR + 1)) + MIN_SIZE_ARR;

    arrServer.arr = (int *)malloc(arrServer.sizeArr * sizeof(int));
    int i;
    for(i=0;i<arrServer.sizeArr;i++)
    {
        arrServer.arr[i] = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
    }
}

void printInfo()
{
    printf("Range clients: %d - %d\n",MIN_CLIENTS,MAX_CLIENTS);
    printf("Size arr: %d\n", arrServer.sizeArr);
    printf("\n--------------\n");
}

char * getElementFromArr()
{
    char * str = (char*)malloc(100*sizeof(char));
    if(count_client_active()>=MIN_CLIENTS && count_client_active()<=MAX_CLIENTS)
    {
        if(arrServer.idx >= arrServer.sizeArr)
        {
            sprintf(str,"full");
        }
        else
        {
            sprintf(str,"%d",arrServer.arr[arrServer.idx++]);
        }   
    }
    else
    {
        sprintf(str,"Not in range clients");
    }
    return str;
}

void insertSorted(int *arr, int n, int key) 
{  
    int i; 
    for (i=n-1; ( i >= 0  && arr[i] > key); i--) 
       arr[i+1] = arr[i]; 
  
    arr[i+1] = key;
}