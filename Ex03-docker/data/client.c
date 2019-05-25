#include "func.h"
 
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
