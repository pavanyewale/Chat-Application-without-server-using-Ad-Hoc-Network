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
#include<time.h>
//message constants
#define msg_q_size 124  //msg which is in queue
#define msg_size 91   //send_msg size
#define use_flag_start 0  
#define msg_send_to_start 1
#define msg_send_to_size 32
#define smsg_start 33  //send msg
#define smsg_size 91
#define a_m_flag_start 0
#define a_m_flag_size 1
#define from_start 1
#define from_size 1
#define msg_no_start 2
#define msg_no_size 4
#define to_start 6
#define to_size 1
#define time_start 7
#define time_size 20
#define message_start 27
#define message_size 64   //real text msg size
//end
#define max_machines 10
#define addr_size 15 
#define my_addr 20
#define my_temp_addr 22
#define addr_format "192.168.1."
#define addr_format_size (int)strlen(addr_format)
#define message_receiver_port 8083 //msg receiving server port no 
FILE *nfp,*mmfp,*sqfp,*logfile;
int curr_Msg_No;
int neibours[255];
int curr_msg_no=0,sockfd;
struct sockaddr_in server_address;
char address[addr_size];
