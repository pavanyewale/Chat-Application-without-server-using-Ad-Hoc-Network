
void errormsg(char *msg,int flag)
{
	fprintf(stdout,"\n%s\n",msg);
	if(flag)
	exit(0);
}
//if file not exist create file
void createfile()
{
	
	sqfp = fopen(sqfpn,"w+");
	if(!sqfp)
		errormsg("file creating error",1);
	fwrite("0000",1,4,sqfp);//write 4 byte for number of msg 
	fflush(sqfp);
	fclose(sqfp);
}
//open msg file queue
void openfile()
{	
	mmfp = fopen("mymessage","r+");
	if(mmfp==NULL)
		mmfp=fopen("mymessage","w");
		fflush(mmfp);
	sqfp = fopen(sqfpn,"r+");
	if(!sqfp)
	{
		errormsg("File opening error",0);
		createfile();
		sqfp = fopen(sqfpn,"r+");
		if(!sqfp)
		errormsg("File opening error 1",1);

	}
}

int hextoint(unsigned char *no)
{
	int result = 0;
	result+=no[0]<<0;//becase first byte is from ans akn
	result+=no[1]<<8;
	result+=no[2]<<16;
//	result+=no[3]<<32;
	return result;
}
//write my message in file
int write_my_msg(char *msg)
{
	fprintf(logfile,"\nwritemy message");
	fflush(logfile);
	int nobyte=-1;
	int len;
	char buffere[91];//message size
	rewind(mmfp);//set file pointer to begging of file
	while(1)
	{
		nobyte = fread(buffere,1,91,mmfp);
		if(nobyte>=91)
			if(hextoint((char*)&msg[2])==hextoint((char*)&buffere[2]) && (int)msg[1]==(int)buffere[1]){
				fprintf(logfile,"\n message already present");
				fprintf(logfile,"\n %s",msg);
				fflush(logfile);
					return 0;//message already present
			}
			else
				continue;
			
		else
		{
			fwrite(msg,1,91,mmfp);
			fflush(mmfp);
			len = strlen(msg);
			fprintf(logfile,"\n message written in my message size %d",len);
				fflush(logfile);
			return 1;//message written succefuly
		}

	}
}
//write new msg in file success 1 unsucess 0
int write_msg(char *msg)
{
	int nobyte=0;
	long int filepointer=-1;
	char buffere[msg_q_size];
//	openfile();
	fseek(sqfp,4,SEEK_SET);
	while(1)
	{
		nobyte = fread(buffere,1,msg_q_size,sqfp);
		if(nobyte==msg_q_size)
		{
			if((int)buffere[use_flag_start]==0)//if msg already sent dont need we can replace that message
				filepointer = ftell(sqfp) - msg_q_size;//last empty space location of file block
			if(hextoint((char*)&msg[smsg_start+msg_no_start])==hextoint((char*)&buffere[smsg_start+msg_no_start]) && (int)msg[smsg_start+from_start]==(int)buffere[smsg_start+from_start])
	               {
				return 0;//already msg there
		       }

		}
		else
		{
		//	fseek(sqfp,-1,SEEK_CUR);
			if(filepointer!=-1)
			{
				fseek(sqfp,filepointer,SEEK_SET);
				fwrite(msg,1,msg_q_size,sqfp);
				fflush(sqfp);
				return 1;
			}
			else
			{
				fwrite(msg,1,msg_q_size,sqfp);
				fflush(sqfp);
				return 2;
				
			}
		}
	}
	return 0;//
}

void negatesendto(char *str,int from,int to)
{
	for(from;from<=to;from++)
		~str[from];//

}
//update ack present in queue
int msg_ack_update(char *msg)
{
	fseek(sqfp,4,SEEK_SET);
	char buffere[msg_q_size];
	int nobyte;

	while(1)
	{
		nobyte = fread(buffere,1,msg_q_size,sqfp);//it return numberof byte readed if nobyte noteql to msg size then it reach EOF
		if(nobyte==msg_q_size)
		{
			if(hextoint((char*)&msg[msg_no_start])==hextoint((char*)&buffere[smsg_start+msg_no_start]) && (int)msg[from_start]==(int)buffere[smsg_start+from_start] && (int)buffere[smsg_start+a_m_flag_start]==1)
			{
					buffere[smsg_start+a_m_flag_start]=0x0;//change magqueue msg to aknowlgement
					negatesendto(buffere,1,msg_send_to_size);//change all bit 1 to 0
					fseek(sqfp,msg_q_size*-1,SEEK_CUR);//go back to msg position
					fwrite(buffere,1,msg_q_size,sqfp);//write modified msg
					fflush(sqfp);
					return 1;//msg present in queue		
			}
		}
		else
			return 0;//msg not present
	}
}
