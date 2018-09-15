#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#define myip 20
#define MAXRECVSTRING 255  /* Longest string to receive */
int msgs[255][500];
void DieWithError(char *errorMessage)
{
	printf("\n %s",errorMessage);
}  /* External error handling function */

int main(int argc, char *argv[])
{
    int sock,i,j,sock1,k;                         /* Socket */
    struct sockaddr_in broadcastAddr,sendadd; /* Broadcast Address */
    unsigned short broadcastPort;     /* Port */
    char recvString[MAXRECVSTRING+1]; /* Buffer for received string */
    int recvStringLen;
	int broadcastPermission=1;                /* Length of received string */
	memset(&sendadd,0,sizeof(sendadd));
	
   // if (argc != 2)    /* Test for correct number of arguments */
   /* {
        fprintf(stderr,"Usage: %s <Broadcast Port>\n", argv[0]);
        exit(1);
    }
*/
	for(i=0;i<255;i++)
		for(j=0;j<500;j++)
		msgs[i][j]=0;
    /* Create a best-effort datagram socket using UDP */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
   if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,sizeof(broadcastPermission)) < 0)
        	printf("setsockopt() failed"); 

    /* Construct bind structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    broadcastAddr.sin_port = htons(8081);      /* Broadcast port */
    
    /* Bind to the broadcast port */
    if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0)
        DieWithError("bind() failed");
	while(1){
    /* Receive a single datagram from the server */
    if ((recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, NULL, 0)) < 0)
        DieWithError("recvfrom() failed");

//	for(i=0;i<255;i++)
  //  printf("%c", recvString[i]);    /* Print the received string */
	sscanf((const char*)&recvString[0],"%d",&i);
	
	sscanf((const char*)&recvString[5],"%d",&j);
	sscanf((const char*)&recvString[10],"%d",&k);
	//printf("received from %d to %d msgno %d msg : %s",k,i,j,(char*)&recvString[15]);
	if((!msgs[i][j])&&(k!=myip))
	{
	msgs[i][j]=1;
	if(i==myip)
	{
		printf("\nmessage from %d:-%s",k,(char*)&recvString[15]);
    		    /* Print the received string */
	//if((int)recvString[0]==myip)
	}
	else
	{
    broadcastAddr.sin_addr.s_addr = inet_addr("192.168.1.255");  /* Any incoming interface */

    printf("msg broadcasted");
		sendto(sock, (const char *)recvString, sizeof(recvString),MSG_CONFIRM, (const struct sockaddr *) &broadcastAddr,sizeof(broadcastAddr)); 

    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
	}
   }
	else
		printf("\n message allready handled..");
}
    close(sock);
    exit(0);
}
