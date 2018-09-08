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
#define PORT     8080 
#define MAXLINE 8 
#define MSEC 5000
#define SEC  5
int neibours[253];  
// Driver code 
int main() { 
	
    int sockfd,i; 
    char buffer[MAXLINE],buffer1[MAXLINE],address[13];
   sprintf(address,"192.168.1.");
   sprintf(buffer,"ATAMAM");
   struct sockaddr_in servaddr; 
  
 //    Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT);
	while(1)
	{	
	printf("\nonline list:");
    for(i=2;i<255;i++)
	{	
		sprintf(address+10,"%d",i);
    	servaddr.sin_addr.s_addr = inet_addr(address); 
    	int n, len; 
      
    	sendto(sockfd, (const char *)buffer, 6,MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr)); 
         usleep(MSEC);
    	n = recvfrom(sockfd, (char *)buffer1, MAXLINE,MSG_DONTWAIT, (struct sockaddr *) &servaddr,&len); 
	if(strncmp(buffer1,buffer,6)==0)
		fprintf(stdout,"\n %s is online",address);
	strcpy(buffer1,"      ");
	}
//		sleep(SEC);
	}
	close(sockfd); 
	
    return 0; 
} 
