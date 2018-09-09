//This program send packet to the all IP addresses within range (192.168.1.2) to (192.168.1.254) and wait for a response 
//if there is a response within the a specific time then its means that the machine is online else offline
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include<arpa/inet.h>
#include<pthread.h>
#include<stdlib.h>
#define PORTS    8081
#define PORTC    8080
#define MAXLINE 8 
#define MSEC 5000
#define SEC  5
#define MSG "ATAMAM"
int neiboursold[253];  
int neiboursnew[253];
// Driver code 

void *sendPackets() { 
//	printf("\n sendPackets running...");
    int sockfd,i; 
    char buffer[MAXLINE],address[13];
   sprintf(address,"192.168.1.");
   struct sockaddr_in servaddr; 
  
 //    Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORTS);
    
	for(i=2;i<255;i++)
		neiboursold[i]=0;

	while(1)
	{	
    	for(i=2;i<255;i++)
		{	
		sprintf(address+10,"%d",i);
    		servaddr.sin_addr.s_addr = inet_addr(address); 
    		sendto(sockfd, (const char *)MSG, sizeof(MSG),MSG_DONTWAIT, (const struct sockaddr *) &servaddr,sizeof(servaddr)); 
		}
   	sleep(5);
//	printf("\nupdating table from sendpackets");
    	for(i=2;i<255;i++)
		{
		neiboursold[i]=neiboursnew[i];
		neiboursnew[i]=0;
		}
	}
	close(sockfd); 
}

void * receivePackets() 
{ 
//	printf("\n receivePackets running");
    int sockfd,i,last_addr; 
    char buffer[MAXLINE],address[13],addr[INET_ADDRSTRLEN];
   sprintf(address,"192.168.1.");
   struct sockaddr_in servaddr; 
  
 //    Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORTC);
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

	while(1)
	{		
    	int n,len;
    	n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,&len); 
 	inet_ntop(AF_INET,&(servaddr.sin_addr), addr, INET_ADDRSTRLEN);	
  //     	printf("packet received from %s",addr);	
//	printf("%s",buffer);
//	printf("address format%s",address);
	if(strncmp(buffer,MSG,6)==0)
	{
		strcpy(buffer,"      ");
//		printf("\nmsg verified");
		if((strncmp(address,addr,10)==0))
		{
			last_addr=atoi(addr+10);
//			printf("address:%s",addr);
			if(last_addr>0&&last_addr<255)
				neiboursnew[last_addr]=1;
		}

	}
	}
	
	close(sockfd); 
	
} 

void main()
{
pthread_t send,receive;
int i;
pthread_create(&send,NULL,sendPackets,NULL);
pthread_create(&receive,NULL,receivePackets,NULL);
while(1)
{
	system("clear");
printf("\n online neibours are:");
for(i=2;i<255;i++)
	if(neiboursold[i])
		printf("\n%d",i);
sleep(4);
}
pthread_join(send,NULL);
pthread_join(receive,NULL);
}

