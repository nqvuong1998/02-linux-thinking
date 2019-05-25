#include "func.h"
 
 int main()
 {
    arrClient.arr = (int *)malloc(sizeof(int)*1);
    int sockfd;
    char buffer[BUFFER_SIZE];
    char server_reply[BUFFER_SIZE];
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
	if(recv(sockfd,id,BUFFER_SIZE,0 ) > 0)
    {
        if(strcmp(id,"Over clients")==0)
        {
            printf("Over clients\n");
            return 1;
        }
        printf("Client ID: %s\n",id);

        strcpy(filename,id);
        strcat(filename,"-client.txt");
        FILE * f = fopen(filename, "w");
        fclose(f);
    }
    else
    {
        puts("Error recv ID");
        close(sockfd);
        return 1;
    }
    
    int num=0;
    int count_request = 0;

    while (1)
    {
        send(sockfd, "get",BUFFER_SIZE,0);

        memset(server_reply, '\0', BUFFER_SIZE);
        if(recv(sockfd,server_reply,BUFFER_SIZE,0 ) <= 0)
        {
            puts("Error get data");
            break;
        }
        
        printf("Server Reply: %s\n",server_reply);
        if(strcmp(server_reply,"full")==0)
        {
            printf("\nCount request: %d\n",count_request);

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
            continue;
        }
        
        count_request++;
        num=atoi(server_reply);
        writeFile(filename,num);
    }

	close(sockfd);
	return 0;
	 
 }
