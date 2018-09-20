#include"header.h"
#include"functions.h"
#include"filehandling.h"
void receiveMessageServer() 
{ 
	fprintf(logfile,"\n receive message server ");
	fflush(logfile);
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
	pthread_t neibours;
	pthread_create(&neibours,NULL,updateNeiboursList,NULL);
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[msg_q_size] = {0};
   char buff[msg_size]={0x0}; 
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
	fprintf(logfile,"\nsocket created  ");
	fflush(logfile);
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
	fprintf(logfile,"\n reuse permission given  ");
	fflush(logfile);
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( message_receiver_port ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
	fprintf(logfile,"\n binding done  ");
	fflush(logfile);
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    while(1)
    {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 

    if(read( new_socket , buff, msg_q_size)>0) 
    	{
		//check msg is my message or someone's else i.e checking sender
		if((int)buff[from_start]!=my_temp_addr)//
		{	
			fprintf(logfile,"\n message from %d to %d ack/msg flag=%d  ,msgno=%d msg:%s",(int)buff[from_start],(int)buff[to_start],(int)buff[a_m_flag_start],btoi((char*)&buff[msg_no_start]),(char*)&buff[message_start]);
			fflush(logfile);
			//checking msg is ack or msg
			if((int)buff[a_m_flag_start])//msg
			{
				if((int)buff[to_start]==my_addr)//my message
				{
					write_my_msg(buff);
				}
				else//someones else message
				{	
					create_M_Q_Entry((char*)&buff[0],(char*)&buffer[0]);
					if(write_msg(buffer))
					{
						send_message(buffer);
					}
				}
			}
			else//ack
			{	
				msg_ack_update(buff);
			}
		}
		else//only ack has to handle
		{
			//check if its ack
			if(!(int)buffer[smsg_start+a_m_flag_start])
			
			{
				
			fprintf(logfile,"\n my message ack received msg no %d   ",btoi((char*)&buff[msg_no_start]));
			fflush(logfile);
				msg_ack_update((char*)&buffer[smsg_start]);
			}
		}
	}
    }
    
			fprintf(logfile,"\n returning from sendmessage  ");
			fflush(logfile);
    return ; 
} 

int main()
{
	logfile=fopen("recvLogfile","w");
	openfile();
	fprintf(logfile,"\n  ");
	fflush(logfile);
	receiveMessageServer();
}
