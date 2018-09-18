#include"header.h"
#include"functions.h"
#include"filehandling.h"
void receiveMessageServer() 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
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
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( message_receiver_port ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
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
    while(read( new_socket , buff, 1024)>0) 
    	{
		//check msg is my message or someone's else i.e checking sender
		if((int)buff[from_start]!=my_temp_addr)//
		{	//checking msg is ack or msg
			if((int)buff[a_m_flag_start])//msg
			{
				if((int)buff[to_start]==my_addr)//my message
				{
					write_my_msg(buff);
				}
				else//someones else message
				{	
					create_M_Q_Entry((char*)&buff[0],(char*)&buffer[0]);
					for(int i=0;i<msg_q_size;i++)
						fprintf(fp,"%c",buffer[i]);
					fflush(fp);
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
				msg_ack_update((char*)&buffer[smsg_start]);
			}
		}
	}
    }
    return ; 
} 

int main()
{
	openfile();
	receiveMessageServer();
}
