#ifndef __SERVE_FUNC_H__
#define __SERVE_FUNC_H__
#include <sqlite3.h>

sqlite3 * createDataBase(sqlite3 * db);
int recv_attendant_register(int newfd, char * name, char * password, sqlite3 * db);
int recv_staff_register(int newfd,char* num, char * name,char * phone,char * password,char * address,int salary,int age,sqlite3 * db);

int recv_login(int newfd, char * name,char * password,sqlite3 * db, int isAttendant);
int recv_quit(char * name,sqlite3 * db, int isAttendant);

int recv_addStaffInformation(int newfd,char* num, char * name,char * phone,char * password,char * address,int salary,int age,sqlite3 * db);
int recv_deleteStaffInformation(int newfd,char * num,sqlite3 * db);


int recv_changeStaffInformation(int newfd,char * num,sqlite3 * db);
int recv_changeStaffInformationWithType(int newfd,char * num,char * type,char * change,sqlite3 * db);
int recv_changeMyInformation(int newfd,char * type,char * change,char * num,sqlite3 * db);

int recv_selectStaffInformation(int newfd,char * select,char * type,sqlite3 * db);
int recv_selectMyInformation(int newfd,char * num,sqlite3 * db);
int recv_selectAllStaffInformation(int newfd,sqlite3 * db);
#endif
