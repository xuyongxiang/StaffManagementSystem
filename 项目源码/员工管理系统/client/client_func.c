#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "client_func.h"

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

void displaymain(int sfd)
{
	char choose = 0;
	while(1)
	{
		printf("********************************\n");
		printf("**********员工管理系统**********\n");
		printf("**********1.管理员注册**********\n");
		printf("**********2.员工注册************\n");
		printf("**********3.管理员登录**********\n");
		printf("**********4.员工登录************\n");
		printf("**********5.退出****************\n");
		printf("********************************\n");
		choose = getchar();
		while(getchar()!=10);
		switch (choose)
		{
		case '1':
			send_attendant_register(sfd);
			break;
		case '2':
			send_staff_register(sfd);
			break;
		case '3':
			send_attendant_login(sfd);
			break;
		case '4':
			send_staff_login(sfd);
			break;
		case '5':
			return;
		default:
			printf("输入有误，请重新输入\n");
			continue;
		}
		printf("\n输入任意字符清屏\n");
		while(getchar()!=10);
		system("clear");
	}


}
void displaySelect(int sfd)
{
	char choose = 0;
	while(1)
	{
		printf("**********************************\n");
		printf("*********1.根据工号查询***********\n");
		printf("*********2.根据姓名查询***********\n");
		printf("*********3.根据手机号查询*********\n");
		printf("*********4.返回上一级*************\n");
		printf("**********************************\n");
		choose = getchar();
		while(getchar()!=10);
		switch(choose)
		{
			case '1':
				send_selectStaffInformation(sfd,"num");
				break;
			case '2':
				send_selectStaffInformation(sfd,"name");
				break;
			case '3':
				send_selectStaffInformation(sfd,"phone");
				break;
			case '4':
				return;
			default:
				printf("输入有误，请重新输入\n");
				continue;
		}
		printf("\n输入任意字符清屏\n");
		while(getchar()!=10);
		system("clear");
	}

}
void displayAttendant(int sfd,char * name)
{
	char choose = 0;
	while(1)
	{
		printf("********************************\n");
		printf("*********1.添加员工信息*********\n");
		printf("*********2.删除员工信息*********\n");
		printf("*********3.修改员工信息*********\n");
		printf("*********4.查询员工信息*********\n");
		printf("*********5.查看全部员工信息*****\n");
		printf("*********6.退出登录*************\n");
		printf("********************************\n");
		choose = getchar();
		while(getchar()!=10);
		switch(choose)
		{
		case '1':
			send_addStaffInformation(sfd);
			break;
		case '2':
			send_deleteStaffInformation(sfd);
			break;
		case '3':
			send_changeStaffInformation(sfd);
			break;
		case '4':
			displaySelect(sfd);
			break;
		case '5':
			send_selectAllStaffInformation(sfd);
			break;
		case '6':
			send_quit(sfd,name,1);
			return;
		default:
			printf("输入有误，请重新输入\n");
			continue;
		}
		printf("\n输入任意字符清屏\n");
		while(getchar()!=10);
		system("clear");

	}

}
void displayChangeMyInformation(int sfd,char * num)
{
		
	char choose = 0;
	while(1)
	{
		printf("********************************\n");
		printf("***********1.修改姓名***********\n");
		printf("***********2.修改密码***********\n");
		printf("***********3.修改手机号*********\n");
		printf("***********4.修改地址***********\n");
		printf("***********5.返回上一级*********\n");
		printf("********************************\n");
		choose = getchar();
		while(getchar()!=10);
		switch(choose)
		{
			case '1':
				send_changeMyInformation(sfd,"name",num);
				break;
			case '2':
				send_changeMyInformation(sfd,"password",num);
				break;
			case '3':
				send_changeMyInformation(sfd,"phone",num);
				break;
			case '4':
				send_changeMyInformation(sfd,"address",num);
				break;
			case '5':
				return;
			default:
				printf("输入有误，请重新输入\n");
				continue;
		}
		printf("\n输入任意字符清屏\n");
		while(getchar()!=10);
		system("clear");
	}
}
int send_changeMyInformation(int sfd,char * type,char * num)
{
	agreeMent ag;
	ag.type = 'K';
	strcpy(ag.num,num);
	if(strcmp(type,"name")==0)
	{
		printf("请输入修改后的姓名>>>");
		fgets(ag.name,sizeof(ag.name),stdin);
		ag.name[strlen(ag.name)-1] = 0;
	}
	else if(strcmp(type,"password")==0)
	{
		printf("请输入修改后的密码>>>");
		fgets(ag.password,sizeof(ag.password),stdin);
		ag.password[strlen(ag.password)-1] = 0;
	}
	else if(strcmp(type,"phone")==0)
	{
		printf("请输入修改后的手机号>>>");
		fgets(ag.phone,sizeof(ag.phone),stdin);
		ag.phone[strlen(ag.phone)-1] = 0;
	}
	else if(strcmp(type,"address")==0)
	{
		printf("请输入修改后的地址>>>");
		fgets(ag.address,sizeof(ag.address),stdin);
		ag.address[strlen(ag.address)-1] = 0;
	}
	if(send(sfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	if(recv(sfd,buf,sizeof(buf),0)<0)
	{
		perror("recv");	
		return -1;
	}
	printf("%s\n",buf);
	return 0;
}
void displayChangeStaff(int sfd,char * num)
{

	char choose = 0;
	while(1)
	{
		printf("********************************\n");
		printf("***********1.修改姓名***********\n");
		printf("***********2.修改手机号*********\n");
		printf("***********3.修改年龄***********\n");
		printf("***********4.修改地址***********\n");
		printf("***********5.修改薪资***********\n");
		printf("***********6.返回上一级*********\n");
		printf("********************************\n");
		choose = getchar();
		while(getchar()!=10);
		switch(choose)
		{
			case '1':
				send_changeStaffInformationWithType(sfd,"name",num);
				break;
			case '2':
				send_changeStaffInformationWithType(sfd,"phone",num);
				break;
			case '3':
				send_changeStaffInformationWithType(sfd,"age",num);
				break;
			case '4':
				send_changeStaffInformationWithType(sfd,"address",num);
				break;
			case '5':
				send_changeStaffInformationWithType(sfd,"salary",num);
				break;
			case '6':
				return;
			default:
				printf("输入有误，请重新输入\n");
				continue;
		}
		printf("\n输入任意字符清屏\n");
		while(getchar()!=10);
		system("clear");
	}


}

void displayStaff(int sfd,char * num)
{

	char choose = 0;
	while(1)
	{
		printf("********************************\n");
		printf("*********1.查看个人信息*********\n");
		printf("*********2.修改个人信息*********\n");
		printf("*********3.退出登录*************\n");
		printf("********************************\n");
		choose = getchar();
		while(getchar()!=10);
		switch(choose)
		{
			case '1':
					send_selectMyInformation(sfd,num);	
				break;
			case '2':
					displayChangeMyInformation(sfd,num);
				break;
			case '3':
				send_quit(sfd,num,0);
				return;
			default:
				printf("输入有误，请重新输入\n");
				continue;
		}
		printf("\n输入任意字符清屏\n");
		while(getchar()!=10);
		system("clear");
	}
}
int send_selectMyInformation(int sfd,char * num)
{
	agreeMent ag;
	ag.type = 'J';
	strcpy(ag.num,num);
	if(send(sfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	while(1)
	{
		memset(buf,0,sizeof(buf));
		if(recv(sfd,buf,sizeof(buf),0)<0)
		{
			perror("recv");	
			return -1;
		}
		if(strcmp(buf,"None")==0)
		{
			break;
		}
		printf("%s\n",buf);
	}

	return 0;
}

int send_staff_login(int sfd)
{
	agreeMent ag;
	ag.type = 'C';
	printf("请输入工号>>>");
	fgets(ag.num,sizeof(ag.num),stdin);
	ag.num[strlen(ag.num)-1] = 0;
	printf("请输入密码>>>");
	fgets(ag.password,sizeof(ag.password),stdin);
	ag.password[strlen(ag.password)-1] = 0;
	if(send(sfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	if(recv(sfd,buf,sizeof(buf),0)<0)
	{
		perror("recv");	
		return -1;
	}
	if(buf[0]=='N')
	{
		printf("%s\n",buf+1);
	}
	else
	{
		displayStaff(sfd,ag.num);	
	}
	return 0;
}
int send_attendant_login(int sfd)
{

	agreeMent ag;
	ag.type = 'B';
	printf("请输入姓名>>>");
	fgets(ag.name,sizeof(ag.name),stdin);
	ag.name[strlen(ag.name)-1] = 0;
	printf("请输入密码>>>");
	fgets(ag.password,sizeof(ag.password),stdin);
	ag.password[strlen(ag.password)-1] = 0;
	if(send(sfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	if(recv(sfd,buf,sizeof(buf),0)<0)
	{
		perror("recv");	
		return -1;
	}
	if(buf[0]=='N')
	{
		printf("%s\n",buf+1);
	}
	else
	{
		displayAttendant(sfd,ag.name);	
	}
	return 0;
}

int send_attendant_register(int sfd)
{
	agreeMent ag;
	ag.type = 'A';
	printf("请输入姓名>>>");
	fgets(ag.name,sizeof(ag.name),stdin);
	ag.name[strlen(ag.name)-1] = 0;
	printf("请输入密码>>>");
	fgets(ag.password,sizeof(ag.password),stdin);
	ag.password[strlen(ag.password)-1] = 0;
	if(send(sfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	if(recv(sfd,buf,sizeof(buf),0)<0)
	{
		perror("recv");	
		return -1;
	}
	printf("%s\n",buf);
	return 0;
}
int send_staff_register(int sfd)
{
	agreeMent ag;
	ag.type = 'S';
	printf("请输入工号>>>");
	fgets(ag.num,sizeof(ag.num),stdin);
	ag.num[strlen(ag.num)-1] = 0;

	printf("请输入姓名>>>");
	fgets(ag.name,sizeof(ag.name),stdin);
	ag.name[strlen(ag.name)-1] = 0;

	printf("请输入密码>>>");
	fgets(ag.password,sizeof(ag.password),stdin);
	ag.password[strlen(ag.password)-1] = 0;

	printf("请输入手机号码>>>");
	fgets(ag.phone,sizeof(ag.phone),stdin);
	ag.phone[strlen(ag.phone)-1] = 0;

	printf("请输入地址>>>");
	fgets(ag.address,sizeof(ag.address),stdin);
	ag.address[strlen(ag.address)-1] = 0;

	printf("请输入薪资>>>");
	scanf("%d",&ag.salary);

	printf("请输入年龄>>>");
	scanf("%d",&ag.age);
	while(getchar()!=10);
	if(send(sfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	if(recv(sfd,buf,sizeof(buf),0)<0)
	{
		perror("recv");	
		return -1;
	}
	printf("%s\n",buf);
	return 0;
}
int send_addStaffInformation(int newfd)
{
	agreeMent ag;
	ag.type = 'D';
	printf("请输入工号>>>");
	fgets(ag.num,sizeof(ag.num),stdin);
	ag.num[strlen(ag.num)-1] = 0;

	printf("请输入姓名>>>");
	fgets(ag.name,sizeof(ag.name),stdin);
	ag.name[strlen(ag.name)-1] = 0;

	printf("请输入密码>>>");
	fgets(ag.password,sizeof(ag.password),stdin);
	ag.password[strlen(ag.password)-1] = 0;

	printf("请输入手机号码>>>");
	fgets(ag.phone,sizeof(ag.phone),stdin);
	ag.phone[strlen(ag.phone)-1] = 0;

	printf("请输入地址>>>");
	fgets(ag.address,sizeof(ag.address),stdin);
	ag.address[strlen(ag.address)-1] = 0;

	printf("请输入薪资>>>");
	scanf("%d",&ag.salary);

	printf("请输入年龄>>>");
	scanf("%d",&ag.age);
	while(getchar()!=10);
	if(send(newfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	if(recv(newfd,buf,sizeof(buf),0)<0)
	{
		perror("recv");	
		return -1;
	}
	printf("%s\n",buf);
	return 0;
}
int send_deleteStaffInformation(int newfd)
{
	agreeMent ag;
	ag.type = 'E';
	printf("请输入工号>>>");
	fgets(ag.num,sizeof(ag.num),stdin);
	ag.num[strlen(ag.num)-1] = 0;
	if(send(newfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	if(recv(newfd,buf,sizeof(buf),0)<0)
	{
		perror("recv");	
		return -1;
	}
	printf("%s\n",buf);
	return 0;
}
int send_changeStaffInformation(int newfd)
{
	agreeMent ag;
	ag.type = 'F';
	printf("请输入要修改员工的工号>>>");
	fgets(ag.num,sizeof(ag.num),stdin);
	ag.num[strlen(ag.num)-1] = 0;
	if(send(newfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	memset(buf,0,sizeof(buf));
	if(recv(newfd,buf,sizeof(buf),0)<0)
	{
		perror("recv");	
		return -1;
	}
	if(buf[0]=='N')
	{
		printf("%s\n",buf+1);
	}
	else
	{
		displayChangeStaff(newfd,ag.num);
	}
	return 0;
}

int send_changeStaffInformationWithType(int newfd,char * type,char * num)
{
	agreeMent ag;
	ag.type = 'L';
	strcpy(ag.num,num);
	if(strcmp(type,"name")==0)
	{
		printf("请输入修改后的姓名>>>");
		fgets(ag.name,sizeof(ag.name),stdin);
		ag.name[strlen(ag.name)-1] = 0;
	}
	else if(strcmp(type,"age")==0)
	{
		printf("请输入修改后的年龄>>>");
		scanf("%d",&ag.age);
	}
	else if(strcmp(type,"phone")==0)
	{
		printf("请输入修改后的手机号>>>");
		fgets(ag.phone,sizeof(ag.phone),stdin);
		ag.phone[strlen(ag.phone)-1] = 0;
	}
	else if(strcmp(type,"address")==0)
	{
		printf("请输入修改后的地址>>>");
		fgets(ag.address,sizeof(ag.address),stdin);
		ag.address[strlen(ag.address)-1] = 0;
	}
	else if(strcmp(type,"salary")==0)
	{
		printf("请输入修改后的薪资>>>");
		scanf("%d",&ag.salary);
	}
	if(send(newfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	if(recv(newfd,buf,sizeof(buf),0)<0)
	{
		perror("recv");	
		return -1;
	}
	printf("%s\n",buf);
	return 0;
}
int send_selectStaffInformation(int newfd,char * select)
{
	agreeMent ag;
	memset(&ag,0,sizeof(ag));
	ag.type = 'G';
	if(strcmp(select,"num")==0)
	{
		printf("请输入工号>>>");
		fgets(ag.num,sizeof(ag.num),stdin);
		ag.num[strlen(ag.num)-1] = 0;
	}
	else if(strcmp(select,"name")==0)
	{
		printf("请输入姓名>>>");
		fgets(ag.name,sizeof(ag.name),stdin);
		ag.name[strlen(ag.name)-1] = 0;
	}
	else if(strcmp(select,"phone")==0)
	{
		printf("请输入手机号>>>");
		fgets(ag.phone,sizeof(ag.phone),stdin);
		ag.phone[strlen(ag.phone)-1] = 0;
	}
	if(send(newfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	while(1)
	{
		memset(buf,0,sizeof(buf));
		if(recv(newfd,buf,sizeof(buf),0)<0)
		{
			perror("recv");	
			return -1;
		}
		if(strcmp(buf,"None")==0)
		{
			break;
		}
		printf("%s\n",buf);
	}
	return 0;
}
int send_selectAllStaffInformation(int newfd)
{
	agreeMent ag;
	ag.type = 'M';
	if(send(newfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[128];
	while(1)
	{
		memset(buf,0,sizeof(buf));
		if(recv(newfd,buf,sizeof(buf),0)<0)
		{
			perror("recv");	
			return -1;
		}
		if(strcmp(buf,"None")==0)
		{
			break;
		}
		printf("%s\n",buf);
	}
	return 0;	
}
int send_quit(int sfd,char * num,int isAttendant)
{
	agreeMent ag;
	if(isAttendant)
	{
		ag.type = 'H';
		strcpy(ag.name,num);
	}
	else
	{
		ag.type = 'I';
		strcpy(ag.num,num);
	}
	if(send(sfd,&ag,sizeof(ag),0)<0)
	{
		perror("send");
		return -1;
	}
	return 0;
}




