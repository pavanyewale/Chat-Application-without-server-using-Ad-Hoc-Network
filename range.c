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
#include<signal.h>
#include<sys/mman.h>//mmap(),munmap()
#define PORTS    8081
#define MAXLINE 8 
#define MSEC 5000
#define SEC  5
#define MSG "ATAMAM"
#define my_addr 20     //***************change it according to yours********************************//
#define fname "neibours"
FILE *nfp;
int neiboursold[255];  
int neiboursnew[255];
void *sendPackets() { 
    	int sockfd; 
    	char address[13];
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
    	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,sizeof(broadcastPermission)) < 0)
        	printf("setsockopt() failed"); 
	while(1)
	{
	//broadcasting a message	
    		sendto(sockfd, (const char *)MSG, sizeof(MSG),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
   		sleep(3);
	}
	close(sockfd);
}
void * updateNeibourTable()
{
	int i;
	//openning file for write online list in file
	nfp=fopen(fname,"w");
	if(nfp==NULL)
	//initially set all neibours are offline
	for(i=0;i<255;i++)
	{
		neiboursold[i]=0;
		neiboursnew[i]=0;
		fprintf(nfp,"%c",0x0);
	}
	fseek(nfp,0,SEEK_SET);
	fflush(nfp);
//	updating neibours table
    	while(1)
	{
		for(i=0;i<255;i++)
			{
			neiboursold[i]=neiboursnew[i];
			neiboursnew[i]=0;
			if(neiboursold[i])
				fprintf(nfp,"%c",0x1);
			else
				fprintf(nfp,"%c",0x0);
			}
		fflush(nfp);
		fseek(nfp,0,SEEK_SET);
		sleep(5);
	}		
 
}

void * receivePackets() 
{ 
    	int sockfd,last_addr; 
	pthread_t update;
	pthread_create(&update,NULL,updateNeibourTable,NULL);
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
    	servaddr.sin_port = htons(PORTS);
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
			strcpy(buffer,"      ");//set msg to empty
			if((strncmp(address,addr,10)==0))//check the msg formmat
			{
				last_addr=atoi(addr+10);
				fprintf(stdout,"\nmsg from %d",last_addr);
				if(last_addr>0&&last_addr<255&&last_addr!=my_addr)//check the iddentity of neibours
					neiboursnew[last_addr]=1; //updating status as live
			}

		
	}
	pthread_join(update,NULL);
	close(sockfd); 
	
} 

int main()
{	
	int p1,p2;
	char c;
	if((p1=fork())==0)
	{
		receivePackets();
	}
	else{
		if((p2=fork())==0)
		{
			sendPackets();
		}
		printf("\n press enter to exit");
		scanf("%c",&c);
		kill(p1,SIGKILL);
		kill(p2,SIGKILL);
		printf("\n exited successfully..");
	}
}
