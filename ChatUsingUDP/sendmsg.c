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
#include<sys/mman.h>//mmap(),munmap()
#define PORTS    8081
#define PORTC    8080
#define MAXLINE 8 
#define MSEC 5000
#define SEC  5
#define MSG "ATAMAM"
#define fname "neibours"
FILE *nfp;
int neiboursold[253];  
int neiboursnew[253];

void printmsg(char *m)
{	int i;
	for(i=0;i<255;i++)
	{
		printf("%c",m[i]);
	}
}
void *sendPackets() { 
    	int sockfd,i,msgno=0; 
    	char buffer[MAXLINE],address[13];
	int myaddr=20;
   	sprintf(address,"192.168.1.");
  	struct sockaddr_in servaddr; 
  
 //    Creating socket file descriptor 
    	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{ 
        	perror("socket creation failed"); 
        	exit(EXIT_FAILURE); 
    	} 
    	memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    	servaddr.sin_family = AF_INET; 
    	servaddr.sin_port = htons(PORTS);
    	servaddr.sin_addr.s_addr = inet_addr("192.168.1.255"); 
	//giving a permission for broadcasting
       	int broadcastPermission = 1;
	char msgg[255];
	int to;
    	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,sizeof(broadcastPermission)) < 0)
        	printf("setsockopt() failed"); 
	//openning file for write online list in file
	nfp=fopen(fname,"r+");
	//initially set all neibours are offline
	for(i=0;i<255;i++)
	{
		neiboursold[i]=0;
		fprintf(nfp,"%c",0);
	}
	fseek(nfp,0,SEEK_SET);
	while(1)
	{
	memset((char*)msgg,0x0,255);
	//broadcasting a message
	printf("\nEnter address of receiver");	
    	scanf("%d",&to);
	sprintf((char*)&msgg[0],"%d",to);
	sprintf((char*)&msgg[5],"%d",msgno++);
	sprintf((char*)&msgg[10],"%d",myaddr);
	
	printf("\n Enter msg:");
	fgets((char*)&msgg[15],240,stdin);	
	fgets((char*)&msgg[15],240,stdin);	
	printmsg(msgg);	
	sendto(sockfd, (const char *)msgg, sizeof(msgg),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr)); 
   		//sleep(5);

//	updating neibours table
    		for(i=2;i<255;i++)
			{
			neiboursold[i]=neiboursnew[i];
			neiboursnew[i]=0;
			if(neiboursold[i])
				fprintf(nfp,"1");
			else
				fprintf(nfp,"0");
			}
		fflush(nfp);
		fseek(nfp,0,SEEK_SET);
	}
	close(sockfd); 
}

void * receivePackets() 
{ 
    	int sockfd,i,last_addr; 
    	char buffer[MAXLINE],address[13],addr[INET_ADDRSTRLEN];
   	sprintf(address,"192.168.1.");
   	struct sockaddr_in servaddr; 
  
 //    Creating socket file descriptor 
    	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{ 
        	perror("socket creation failed"); 
        	exit(EXIT_FAILURE); 
    	} 
    	memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    	servaddr.sin_family = AF_INET;
    	servaddr.sin_addr.s_addr = INADDR_ANY; 
    	servaddr.sin_port = htons(PORTC);
	//binding a name to the server
    	if ( bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
    	{
        	perror("bind failed");
        	exit(EXIT_FAILURE);
    	}

	while(1)
	{		
    		int n,len;
		//receiving a msg from clients
    		recvfrom(sockfd,(char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,&len); 
 		inet_ntop(AF_INET,&(servaddr.sin_addr), addr, INET_ADDRSTRLEN);	//converting a ipv4 ip address to string (a.b.c.d)
		if(strncmp(buffer,MSG,6)==0)//checking a msg 
		{
			strcpy(buffer,"      ");//set msg to empty
			if((strncmp(address,addr,10)==0))//check the msg formmat
			{
				last_addr=atoi(addr+10);
				if(last_addr>0&&last_addr<255)//check the iddentity of neibours
					neiboursnew[last_addr]=1; //updating status as live
			}

		}
	}
	
	close(sockfd); 
	
} 

void updateNeibours()
{
	pthread_t send,receive;
	int i;
	pthread_create(&send,NULL,sendPackets,NULL);
//	pthread_create(&receive,NULL,receivePackets,NULL);
/*	while(1)
	{
		system("clear");
		printf("\n online neibours are:");
		for(i=2;i<255;i++)
			if(neiboursold[i])
				printf("\n%d",i);
		sleep(4);
	}
	*/
	pthread_join(send,NULL);
//	pthread_join(receive,NULL);
}
void main()
{
	updateNeibours();
}
