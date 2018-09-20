#include"header.h"
#include"functions.h"
//send msg to user who is oflline now he may online
int recSendMessage()
{
	char buffere[msg_q_size];
	sqfp=fopen(sqfpn,"r+");
	fseek(sqfp,4,SEEK_SET);	
	int nobyte=-1;
	unsigned int bit;
	while(1)
	{
		nobyte = fread(buffere,1,msg_q_size,sqfp);
		if(nobyte == msg_q_size)
		{
			bit = send_message(buffere);	
			if(!bit&&!buffere[smsg_start+a_m_flag_start])
			{
				buffere[0]=0x0;
				fprintf(logfile,"\n msg deleted akn send all");
				fflush(logfile);
			}
			fseek(sqfp,msg_q_size*-1,SEEK_CUR);//go back to position and write there
			fwrite(buffere,1,msg_q_size,sqfp);
			fprintf(logfile,"\n msg send and wriiten back to orignal position");
			fflush(logfile);
		}
		else
		{
			fprintf(stdout,"\n in else ");
		//	exit(0);
			fseek(sqfp,4,SEEK_SET);	
			sleep(10);
		
		}
	}

}

int main()
{
	logfile=fopen("recsendLogfile","w");
	fprintf(logfile,"\nmain()");
	fflush(logfile);
	pthread_t neighbours;
	pthread_create(&neighbours,NULL,updateNeiboursList,NULL);
       	recSendMessage();

}
