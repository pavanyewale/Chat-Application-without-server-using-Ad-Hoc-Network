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
#define PORTC    8080
#define PORTD    8086
#define MAXLINE 8 
#define MSEC 5000
#define SEC  5
#define MSG "ATAMAM"
#define fname "neibours"
FILE *nfp,*fp;
int neiboursold[255];  
int neiboursnew[255];
int n[255];
int p4;
void *sendPackets() { 
    	int sockfd; 
    	char address[13];
   	sprintf(address,"192.168.1.");
  	struct sockaddr_in servaddr; 
  	FILE *fp;
	fp=fopen("logfiles/sendPacklog","w");
	
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
	       fprintf(fp,"\n msg sent");
       		fflush(fp);	       
   		sleep(2);
	}
	close(sockfd);
}
void * updateNeibourTable()
{
	int i;
	//openning file for write online list in file
	nfp=fopen(fname,"r+");
	//initially set all neibours are offline
	for(i=0;i<255;i++)
	{
		neiboursold[i]=0;
		fprintf(nfp,"%c",0x0);
	}
	fseek(nfp,0,SEEK_SET);
	FILE *fp;
	fp=fopen("logfiles/updateneilog","w");
//	updating neibours table
    	while(1)
	{	fprintf(fp,"updating");
		fflush(fp);
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
		sleep(1);
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
 	FILE *fp;
       fp=fopen("logfiles/recvPacklog","w");	
 //    Creating socket file descriptor 
    	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{ 
        	perror("socket creation failed"); 
        	exit(EXIT_FAILURE); 
    	} 
    	memset(&servaddr, 0, sizeof(servaddr)); 
      fprintf(fp,"\nsocket created");
      fflush(fp);
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
      fprintf(fp,"\nbinded");
      fflush(fp);

	while(1)
	{		
    		int len;
		//receiving a msg from clients
    		recvfrom(sockfd,(char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,(socklen_t *)&len); 
 		
      fprintf(fp,"\npacket received");
      fflush(fp);
		inet_ntop(AF_INET,&(servaddr.sin_addr), addr, INET_ADDRSTRLEN);	//converting a ipv4 ip address to string (a.b.c.d)
		if(strncmp(buffer,MSG,6)==0)//checking a msg 
		{
			strcpy(buffer,"      ");//set msg to empty
			if((strncmp(address,addr,10)==0))//check the msg formmat
			{
				fprintf(fp,"\nmsg from %s",addr);
				fflush(fp);
				last_addr=atoi(addr+10);
				if(last_addr>0&&last_addr<255)//check the iddentity of neibours
					neiboursnew[last_addr]=1; //updating status as live
			}

		}
	}
	pthread_join(update,NULL);
	close(sockfd); 
	
} 
void *updateNeiboursList()
{
	FILE *fp;
	fp=fopen("logfiles/upnelistlog","w");
        FILE *nfp;
        char c;
        int i=1;
        nfp=fopen("neibours","r");
	fp=fopen("online","w");
while(1)
{
	fprintf(fp,"\n updating ");
	fflush(fp);
        for(i=0;i<255;i++)
        {
                fscanf(nfp,"%c",&c);
                if((int)c)
			fprintf(fp,"\n%d",i);
        }
        fseek(nfp,0,SEEK_SET);
        fflush(fp);
        sleep(3);
}
}

void updateNeibours()
{
	int p1,p2,p3;
	char c;
	if((p1=fork())==0)
	{
		sendPackets();
	}
	else{
		if((p2=fork())==0)
		{
		receivePackets();
		}
		else{
			if((p3=fork())==0)	
				updateNeiboursList();
			else
			{
				printf("\n Enter any key and press enter to exit");
				scanf("%c",&c);
				
				kill(p1,SIGKILL);
				kill(p2,SIGKILL);
				kill(p3,SIGKILL);
				kill(p4,SIGKILL);
				
			
			}
		}
	}

}

int main()
{/*
	if( (p4=fork())==0)
	{
		rangePacketResponseServer();
	}*/
	updateNeibours();
 return 0;
}
