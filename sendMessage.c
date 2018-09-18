#include"header.h"
#include"functions.h"
void create_msg(char *mssg,int to)
{
	
	fprintf(logfile,"\n create_msg");
	fflush(logfile);	
	char msg[smsg_size],q_msg[msg_q_size];
	int i;
	unsigned char *p;
  	time_t rawtime;
  	struct tm * timeinfo;
	memset(msg,0x0,msg_q_size);

	msg[a_m_flag_start]=0x1;
	//writing source address
	sprintf((char *)&msg[from_start],"%c",my_addr);
	//writing current msg no
	p=((unsigned char*)&curr_msg_no);
	curr_msg_no++;
	strncpy((char*)&msg[msg_no_start],p,4);
	//writing destination address
	sprintf((char*)&msg[to_start],"%c",to);
	//adding time
  	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
  	sprintf((char*)&msg[time_start],"%s",asctime (timeinfo));
	//adding msg
	sprintf((char*)&msg[message_start],"%s",mssg);
	create_M_Q_Entry(msg,(char*)&q_msg[0]);

	send_message(q_msg);
}

int main()
{
	pthread_t neibours;
	pthread_create(&neibours,NULL,updateNeiboursList,NULL);
	logfile=fopen("sendLogfile","w");
	fprintf(logfile,"\nmain() ");
	fflush(logfile);	
	while(1)
	{	int to;
		char msg[512];
		printf("\n To send msg:\n Enter to:");
		scanf("%d",&to);
		printf("\n Enter msg:");
		fgets((char*)&msg[0],64,stdin);
		fgets((char*)&msg[0],64,stdin);
		printf("%s",msg);
		create_msg(msg,to);
		
	}
}
