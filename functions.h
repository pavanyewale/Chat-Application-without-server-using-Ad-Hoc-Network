#define sqfpn "sendMSGQueue"
int btoi(unsigned char *str)
{
	fprintf(logfile,"\nbtoi ");
	fflush(logfile);	
	int i=0;
	i+=str[0]<<0;
	i+=str[1]<<8;
	i+=str[2]<<16;
	//i+=str[3]<<32;
	return i;
}

int send_message(char *q_msg)
{
	fprintf(logfile,"\nsend_message ");
	fflush(logfile);	
	int sockfd,conn,i,k,byte,bit,count=0,to;
	char address[addr_size];
        struct sockaddr_in server_address;
        //sockfd=socket(AF_INET,SOCK_STREAM,0);
        server_address.sin_family=AF_INET;
        server_address.sin_port=htons(message_receiver_port);
	strncpy((char*)&address[0],addr_format,addr_format_size);
	
	for(i=1;i<255;i++)
	{
		if(neibours[i])
		fprintf(stdout," %d",i);
	}
	
	for(i=0;i<255;i++)
	{	
		k=i+1;
		bit=(k-1)%8;
		byte=k/8;
		if(bit==0)
			byte+=1;
		byte+=1;
		if(!((q_msg[byte]>>bit)&1))
		{
			count++;
			if(neibours[i])
			{
				fprintf(logfile,"\n%d is online sending msg  ",i);
				fflush(logfile);	
				sprintf((char*)&address[addr_format_size],"%d",i);
				server_address.sin_addr.s_addr = inet_addr(address);
        			sockfd=socket(AF_INET,SOCK_STREAM,0);
				fprintf(logfile,"\n\n server: %s",address);
        			conn=connect(sockfd,(struct sockaddr *)&server_address,sizeof(server_address));
        			if(!conn)
        			{
					fprintf(logfile,"\n connection successfull msg sent ");
					fflush(logfile);	
                			send(sockfd,(char *)&q_msg[smsg_start],message_size,MSG_CONFIRM);
					q_msg[byte]|=1<<(bit);
        			}
        			else	
        			{
					fprintf(logfile,"\n connection failed ");
					fflush(logfile);	

                		//	fprintf(stdout,"\n Connection failed..");
        			}
			close(sockfd);

			}
		}
	}
	//close(sockfd);	
	return count;
}


void create_M_Q_Entry(char *msg,char *q_msg)
{
	fprintf(logfile,"\ncreate_M_Q_Entry ");
	fflush(logfile);	
	int i;
	q_msg[use_flag_start]=0x1;
	memset((char*)&q_msg[msg_send_to_start],0x0,msg_send_to_size);
	for(i=0;i<smsg_size;i++)
		q_msg[smsg_start+i]=msg[i];
}

void *updateNeiboursList()
{
	//fprintf(logfile,"\nupdateNeighbours list ");
	//fflush(logfile);	

	FILE *nfp;
	char c;
	int i=1;
	nfp=fopen("neibours","r");
	if(nfp==NULL)
	{
		fprintf(stdout,"\n please run range program first ..");
		exit(0);
	}
while(1)
{
	for(i=0;i<255;i++)
	{
		fscanf(nfp,"%c",&c);
		neibours[i]=((int)c==1)?1:0;
	}		
	fseek(nfp,0,SEEK_SET);
	sleep(5);
}
}

void get_all_values(char *message,int *ack,int *from,int *msgno,int *to,char *time,char *msg)
{
	fprintf(logfile,"\nget_all_values ");
	fflush(logfile);	
	int i; 	
	*ack=(int)message[0];
	*from = (int)message[1];
	*msgno = btoi((char*)&message[2]);
	*to = (int)message[6];
	strncpy(time,(char*)&message[7],20);
	strncpy(msg,(char*)&message[60],64);
	return;
}

