#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "serve_func.h"
#define ERR_MSG(msg) do{\
	printf("%d\n",__LINE__);\
	perror(msg);\
}while(0)
typedef struct
{
	char type;
	char num[30];
	char name[30];
	char password[50];
	char phone[18];
	char address[30];
	int salary;
	int age;
}__attribute__((packed)) agreeMent;

struct nfdSeq
{
	sqlite3 * db;
	int newfd;
};
void * recv_func(void * arg)
{
	agreeMent ag;
	int res = 0;
	struct nfdSeq seq = *(struct nfdSeq *)arg;
	while(1)
	{
		memset(&ag,0,sizeof(ag));
		res = recv(seq.newfd,&ag,sizeof(ag),0);
		if(res<0)
		{
			ERR_MSG("recv");
			return NULL;
		}
		else if (res ==0 )
		{
			printf("客户端关闭\n");
			return NULL;
		}
		switch (ag.type)
		{
			//管理员注册
			case 'A':
				recv_attendant_register(seq.newfd,ag.name,ag.password,seq.db);
				break;
			//员工注册
			case 'S':
				recv_staff_register(seq.newfd,ag.num,ag.name,ag.phone,ag.password,ag.address,ag.salary,ag.age,seq.db);
				break;
			//管理员登录
			case 'B':
				recv_login(seq.newfd,ag.name,ag.password,seq.db,1);
				break;
			//员工登录
			case 'C':
				recv_login(seq.newfd,ag.num,ag.password,seq.db,0);
				break;
			case 'D':
				recv_addStaffInformation(seq.newfd,ag.num,ag.name,ag.phone,ag.password,ag.address,ag.salary,ag.age,seq.db);
				break;
			case 'E':
				recv_deleteStaffInformation(seq.newfd,ag.num,seq.db);
				break;
			case 'F':
				recv_changeStaffInformation(seq.newfd,ag.num,seq.db);
				break;
			case 'G':
				if(strlen(ag.num)!=0)
				{
					recv_selectStaffInformation(seq.newfd,ag.num,"num",seq.db);
				}
				else if (strlen(ag.name)!=0)
				{
					recv_selectStaffInformation(seq.newfd,ag.name,"name",seq.db);
				}
				else if (strlen(ag.phone)!=0)
				{
					recv_selectStaffInformation(seq.newfd,ag.phone,"phone",seq.db);
				}
				break;
			case 'H':
				recv_quit(ag.name,seq.db,1);
				break;
			case 'I':
				recv_quit(ag.num,seq.db,0);
				break;
			case 'J':
				recv_selectMyInformation(seq.newfd,ag.num,seq.db);
				break;
			case 'K':
				if(strlen(ag.num)!=0)
				{
					recv_changeMyInformation(seq.newfd,"num",ag.num,ag.num,seq.db);
				}
				else if (strlen(ag.name)!=0)
				{
					recv_changeMyInformation(seq.newfd,"name",ag.name,ag.num,seq.db);
				}
				else if (strlen(ag.phone)!=0)
				{
					recv_changeMyInformation(seq.newfd,"phone",ag.phone,ag.num,seq.db);
				}
				else if (strlen(ag.password)!=0)
				{
					recv_changeMyInformation(seq.newfd,"password",ag.password,ag.num,seq.db);
				}
				break;
			case 'L':
				if(strlen(ag.name)!=0)
				{
					recv_changeStaffInformationWithType(seq.newfd,ag.num,"name",ag.name,seq.db);
				}
				else if (ag.age!=0)
				{
					char age[128];
					sprintf(age,"%d",ag.age);
					recv_changeStaffInformationWithType(seq.newfd,ag.num,"age",age,seq.db);
				}
				else if (strlen(ag.phone)!=0)
				{
					recv_changeStaffInformationWithType(seq.newfd,ag.num,"phone",ag.phone,seq.db);
				}
				else if (strlen(ag.address)!=0)
				{
					recv_changeStaffInformationWithType(seq.newfd,ag.num,"address",ag.address,seq.db);
				}
				else if (ag.salary!=0)
				{
					char salary[128];
					sprintf(salary,"%d",ag.salary);
					recv_changeStaffInformationWithType(seq.newfd,ag.num,"salary",salary,seq.db);
				}
				break;
			case 'M':
				recv_selectAllStaffInformation(seq.newfd,seq.db);
				break;
		}
	
	}
	pthread_exit(NULL);
}
int main(int argc, const char *argv[])
{
	if(argc < 3)
	{
		printf("请输入IP和端口号\n");
		return -1;
	}
	sqlite3 * db = NULL;
	db = createDataBase(db);
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		ERR_MSG("socket");
		return -1;
	}
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[2]));
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	int reuse = 1;
	if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))<0)
	{
		ERR_MSG("setsockopt");
		return -1;
	}
	if(bind(sfd,(struct sockaddr *)&sin,sizeof(sin))<0)
	{
		ERR_MSG("bind");
		return -1;
	}
	if(listen(sfd,10)<0)
	{
		ERR_MSG("listen");
		return -1;
	}
	struct sockaddr_in cin;
	int newfd = 0;
	int len = sizeof(cin);
	pthread_t pid;
	struct nfdSeq seq;
	seq.db = db;
	while(1)
	{
		newfd = accept(sfd,(struct sockaddr *)&cin,&len);
		seq.newfd = newfd;
		pthread_create(&pid,NULL,recv_func,&seq);
		pthread_detach(pid);
	}
	close(newfd);
	close(sfd);
	sqlite3_close(db);
	return 0;
}
