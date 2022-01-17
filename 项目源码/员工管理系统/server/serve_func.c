#include <stdio.h>
#include "serve_func.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
sqlite3 * createDataBase(sqlite3 * db)
{
	if(sqlite3_open("staff.db",&db)!=0)
	{
		printf("%s\n",sqlite3_errmsg(db));
		return NULL;
	}
	char * errmsg = NULL;
	char * sql = "create table if not exists attendant (name char primary key,password char,state int)";
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return NULL;
	}
	printf("create attendant table success\n");
	sql = "create table if not exists staff (num char primary key,name char,password char,phone char,address char,salary int,age int,state int)";
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return NULL;
	}
	printf("create staff table success\n");
	sql = "update staff set state = 0";
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return NULL;
	}
	sql = "update attendant set state = 0";
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return NULL;
	}

	return db;
}	

int recv_attendant_register(int newfd, char * name, char * password, sqlite3 * db)
{

	printf("收到管理员注册请求\n");
	char sql[128];
	char * errmsg = NULL;
	int row,column;
	char ** presult = NULL;
	char str[128];
	int state = 0;
	sprintf(sql,"select * from attendant where name = \"%s\"",name);
	if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	if(row==0)
	{
		memset(sql,0,sizeof(sql));
		sprintf(sql,"insert into attendant values(\"%s\",\"%s\",%d)",name,password,state);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
		{
			printf("%s\n",errmsg);
			return -1;
		}
		sprintf(str,"注册成功");
	}
	else
	{
		sprintf(str,"该管理员已注册");
	}
	sqlite3_free_table(presult);
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}
	return 0;
}
int recv_staff_register(int newfd,char * num,char * name,char * phone,char * password,char * address,int salary,int age,sqlite3 * db)
{

	printf("收到员工注册请求\n");
	char sql[128];
	char * errmsg = NULL;
	int row,column;
	char ** presult = NULL;
	char str[128];
	int state = 0;
	sprintf(sql,"select * from staff where num = \"%s\"",num);
	if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	if(row==0)
	{
		memset(sql,0,sizeof(sql));
		sprintf(sql,"insert into staff values(\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,%d,%d)",num,name,password,phone,address,salary,age,state);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
		{
			printf("%s\n",errmsg);
			return -1;
		}
		sprintf(str,"注册成功");
	}
	else
	{
		sprintf(str,"该员工已注册");
	}
	sqlite3_free_table(presult);
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}
	return 0;

}

int recv_login(int newfd, char * name,char * password,sqlite3 * db,int isAttendant)
{
	printf("收到登录请求\n");
	char sql[128];
	char * errmsg = NULL;
	int row,column;
	char ** presult = NULL;
	char str[128];
	if(isAttendant)
	{
		memset(str,0,sizeof(str));
		memset(sql,0,sizeof(sql));
		sprintf(sql,"select * from attendant where name = \"%s\"",name);
		if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
		{
			printf("%s\n",errmsg);
			return -1;
		}
		if(row==0)
		{
			sprintf(str,"N管理员不存在");
		}
		else
		{
			if(strcmp(password,presult[4])==0)
			{
				if(strcmp(presult[5],"0")==0)
				{
					sprintf(str,"S登录成功");
					memset(sql,0,sizeof(sql));
					sprintf(sql,"update attendant set state=%d where name=\"%s\"",1,name);
					if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
					{
						printf("%s\n",errmsg);
						return -1;
					}
				}
				else
				{
					sprintf(str,"N重复登录");
				}
			}
			else
			{
				sprintf(str,"N密码错误");
			}
		}
	}
	else
	{
		memset(str,0,sizeof(str));
		memset(sql,0,sizeof(sql));
		sprintf(sql,"select * from staff where num = \"%s\"",name);
		if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
		{
			printf("%s\n",errmsg);
			return -1;
		}
		if(row==0)
		{
			sprintf(str,"N员工不存在");
		}
		else
		{
		
			if(strcmp(password,presult[10])==0)
			{
				if(strcmp(presult[15],"0")==0)
				{
					sprintf(str,"S登录成功");
					memset(sql,0,sizeof(sql));
					sprintf(sql,"update staff set state=%d where num=\"%s\"",1,name);
					if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
					{
						printf("%s\n",errmsg);
						return -1;
					}
				}
				else
				{
					sprintf(str,"N重复登录");
				}
			}
			else
			{
				sprintf(str,"N密码错误");
			}
				
		}
	}
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}
	return 0;
}
int recv_addStaffInformation(int newfd,char* num, char * name,char * phone,char * password,char * address,int salary,int age,sqlite3 * db)
{
	printf("收到添加请求\n");
	char sql[128];
	char * errmsg = NULL;
	int row,column;
	char ** presult = NULL;
	char str[128];
	int state = 0;
	sprintf(sql,"select * from staff where num = \"%s\"",num);
	if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	if(row==0)
	{
		memset(sql,0,sizeof(sql));
		sprintf(sql,"insert into staff values(\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,%d,%d)",num,name,password,phone,address,salary,age,state);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
		{
			printf("%s\n",errmsg);
			return -1;
		}
		sprintf(str,"添加成功");
	}
	else
	{
		sprintf(str,"该员工已注册");
	}
	sqlite3_free_table(presult);
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}

	return 0;
}
int recv_deleteStaffInformation(int newfd,char * num,sqlite3 * db)
{

	printf("收到删除请求\n");
	char sql[128];
	char * errmsg = NULL;
	int row,column;
	char ** presult = NULL;
	char str[128];
	sprintf(sql,"select * from staff where num = \"%s\"",num);
	if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	if(row==0)
	{
		sprintf(str,"该员工未注册");
	}
	else
	{
		memset(sql,0,sizeof(sql));
		sprintf(sql,"delete from staff where num = \"%s\"",num);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
		{
			printf("%s\n",errmsg);
			return -1;
		}
		sprintf(str,"删除成功");
	}
	sqlite3_free_table(presult);
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}


	return 0;
}

int recv_changeStaffInformation(int newfd,char * num,sqlite3 * db)
{
	printf("收到修改请求\n");
	char sql[128];
	char * errmsg = NULL;
	int row,column;
	char ** presult = NULL;
	char str[128];
	sprintf(sql,"select * from staff where num = \"%s\"",num);
	if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	if(row==0)
	{
		sprintf(str,"N该员工未注册");
	}
	else
	{
		sprintf(str,"S可以修改");
	}
	sqlite3_free_table(presult);
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}

	return 0;
}

int recv_changeStaffInformationWithType(int newfd,char * num,char * type,char * change,sqlite3 * db)
{
	printf("收到管理员修改请求\n");
	char sql[128];
	char * errmsg = NULL;
	char str[128];
	if(strcmp(type,"name")==0)
	{
		sprintf(sql,"update staff set name=\"%s\" where num=\"%s\"",change,num);
	}
	else if(strcmp(type,"age")==0)
	{
		sprintf(sql,"update staff set age=%d where num=\"%s\"",atoi(change),num);
	}
	else if(strcmp(type,"phone")==0)
	{
		sprintf(sql,"update staff set phone=\"%s\" where num=\"%s\"",change,num);
	}
	else if(strcmp(type,"address")==0)
	{
		sprintf(sql,"update staff set address=\"%s\" where num=\"%s\"",change,num);
	}
	else if(strcmp(type,"salary")==0)
	{
		sprintf(sql,"update staff set salary=%d where num=\"%s\"",atoi(change),num);
	}
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	sprintf(str,"修改成功");
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}
	return 0;

}
int recv_changeMyInformation(int newfd,char * type,char * change,char * num,sqlite3 * db)
{
	printf("收到员工修改请求\n");
	char sql[128];
	char * errmsg = NULL;
	char str[128];
	if(strcmp(type,"name")==0)
	{
		sprintf(sql,"update staff set name=\"%s\" where num=\"%s\"",change,num);
	}
	else if(strcmp(type,"phone")==0)
	{
		sprintf(sql,"update staff set phone=\"%s\" where num=\"%s\"",change,num);
	}
	else if(strcmp(type,"address")==0)
	{
		sprintf(sql,"update staff set address=\"%s\" where num=\"%s\"",change,num);
	}
	else if(strcmp(type,"password")==0)
	{
		sprintf(sql,"update staff set password=\"%s\" where num=\"%s\"",change,num);
	}

	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	sprintf(str,"修改成功");
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}
	return 0;
}

int recv_selectStaffInformation(int newfd,char * select,char * type,sqlite3 * db)
{
	
	printf("收到管理员查询请求\n");
	char sql[128];
	char * errmsg = NULL;
	int row,column;
	char ** presult = NULL;
	char str[128];
	if(strcmp(type,"num")==0)
	{
		sprintf(sql,"select * from staff where num = \"%s\"",select);
	}
	else if(strcmp(type,"name")==0)
	{
		sprintf(sql,"select * from staff where name = \"%s\"",select);
	}
	else if(strcmp(type,"phone")==0)
	{
		sprintf(sql,"select * from staff where phone = \"%s\"",select);
	}
	if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	if(row==0)
	{
		sprintf(str,"暂无数据");
		if(send(newfd,str,sizeof(str),0)<0)
		{
			perror("send");
			return -1;
		}

	}
	else
	{
		int i =0,j=8;
		sprintf(str,"工号\t姓名\t手机号\t地址\t薪资\t年龄");
		if(send(newfd,str,sizeof(str),0)<0)
		{
			perror("send");
			return -1;
		}

		for(i=0;i<row;i++)
		{
			bzero(str,sizeof(str));
			sprintf(str,"%s\t%s\t%s\t%s\t%s\t%s",presult[j],presult[j+1],presult[j+3],presult[j+4],presult[j+5],presult[j+6]);
			j+=8;
			if(send(newfd,str,sizeof(str),0)<0)
			{
				perror("send");
				return -1;
			}

		}
	}
	bzero(str,sizeof(str));
	sprintf(str,"None");
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}

	sqlite3_free_table(presult);


	return 0;

}
int recv_selectMyInformation(int newfd,char * num,sqlite3 * db)
{
	printf("收到员工查询请求\n");
	char sql[128];
	char * errmsg = NULL;
	int row,column;
	char ** presult = NULL;
	char str[128];
	sprintf(sql,"select * from staff where num = \"%s\"",num);
	if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	sprintf(str,"工号\t姓名\t手机号\t地址\t薪资\t年龄");
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}
	bzero(str,sizeof(str));
	int	j = 8;
	sprintf(str,"%s\t%s\t%s\t%s\t%s\t%s",presult[j],presult[j+1],presult[j+3],presult[j+4],presult[j+5],presult[j+6]);
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}
	bzero(str,sizeof(str));
	sprintf(str,"None");
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}
	return 0;
}
int recv_selectAllStaffInformation(int newfd,sqlite3 * db)
{

	printf("收到管理员查看全部请求\n");
	char sql[128];
	char * errmsg = NULL;
	int row,column;
	char ** presult = NULL;
	char str[128];
	sprintf(sql,"select * from staff");
	if(sqlite3_get_table(db,sql,&presult,&row,&column,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	if(row==0)
	{
		sprintf(str,"暂无数据");
		if(send(newfd,str,sizeof(str),0)<0)
		{
			perror("send");
			return -1;
		}
	}
	else
	{
		sprintf(str,"%s\t%s\t%s\t%s\t%s\t%s","工号","姓名","手机号","地址","薪资","年龄");
		if(send(newfd,str,sizeof(str),0)<0)
		{
			perror("send");
			return -1;
		}

		int i =0,j=8;
		for(i=0;i<row;i++)
		{
			bzero(str,sizeof(str));
			sprintf(str,"%s\t%s\t%s\t%s\t%s\t%s",presult[j],presult[j+1],presult[j+3],presult[j+4],presult[j+5],presult[j+6]);
			j+=8;
			if(send(newfd,str,sizeof(str),0)<0)
			{
				perror("send");
				return -1;
			}

		}	
	}
	bzero(str,sizeof(str));
	sprintf(str,"None");
	if(send(newfd,str,sizeof(str),0)<0)
	{
		perror("send");
		return -1;
	}
	sqlite3_free_table(presult);
	return 0;
}
int recv_quit(char * name,sqlite3 * db,int isAttendant)
{
	printf("收到退出登录请求\n");
	char sql[128];
	char * errmsg = NULL;
	char str[128];
	if(isAttendant)
	{
		sprintf(sql,"update attendant set state=%d where name=\"%s\"",0,name);
	}
	else
	{
		sprintf(sql,"update staff set state=%d where num=\"%s\"",0,name);
	}
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	return 0;
}




