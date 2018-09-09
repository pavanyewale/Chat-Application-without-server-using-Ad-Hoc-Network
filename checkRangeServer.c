//This program will respond to the another machine's packets showing that I am online
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORTS     8081 
#define PORTC     8080
#define MAXLINE 8 
int main() 
{ 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *msg="ATAMAM";
    char addr[INET_ADDRSTRLEN],address[13]; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
    { 
      	perror("socket creation failed"); 
        exit(EXIT_FAILURE);
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
    // Filling server inform1ation 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORTS); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len, n; 
    while(1)
    {
    	recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);//it will wait till it not gets a msg
	if(strncmp(buffer,msg,6)==0)
    		{	
    			//setting port of client machine
    			cliaddr.sin_port = htons(PORTC);
			sendto(sockfd, (const char *)msg, strlen(msg), MSG_DONTWAIT, (const struct sockaddr *) &cliaddr, len);
		}
    }
      
    return 0; 
}
