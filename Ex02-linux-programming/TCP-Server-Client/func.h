#ifndef FUNC_H_
#define FUNC_H_
   
#include<pthread.h> 
#include<stdbool.h>
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
#define MIN_CLIENTS 3
#define MAX_SIZE_ARR 1000
#define MIN_SIZE_ARR 100
#define MAX_VALUE 10000
#define MIN_VALUE 100

#define BUFFER_SIZE 1024
#define ADDRESS "127.0.0.1"
#define PORT 8888
#define LISTEN_PORT 50

struct ArrServer
{
    int * arr;
    int sizeArr;
    int idx;
};

struct ArrClient
{
    int * arr;
    int sizeArr;
};

struct ClientInfo
{
    bool isActive;
    int sum;
    pthread_t thread;
    int socket;
};

extern pthread_mutex_t lock;
extern struct ClientInfo clients[MAX_CLIENTS + 1];
extern struct ArrServer arrServer;
extern int countClients;
extern struct ArrClient arrClient;

//-----------------------------------------------

void sendFile(char * filename, int socket);
bool receiveFile(char * filename, int socket);
int calSum(char * filename);
void rankingAndSendToClient(char * filename, int socket);
void writeFile(char * filename, int num);

//-----------------------------------------------

void init_clients();
int register_clients(pthread_t thread, int socket);
int count_client_active();
void set_client_die(int i);
void set_client_sum(int i, int sum);

//------------------------------------------------

void initRandomArr();
void printInfo();
char * getElementFromArr();
void insertSorted(int *arr, int n, int key);

#endif // FUNC_H_