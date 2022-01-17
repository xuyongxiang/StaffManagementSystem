#ifndef CLIENT_FUNC_H
#define CLIENT_FUNC_H

void displaymain(int);
int send_attendant_register(int);
int send_staff_register(int);
int send_attendant_login(int);
int send_staff_login(int);
int send_addStaffInformation(int newfd);
int send_deleteStaffInformation(int newfd);

int send_changeStaffInformationWithType(int newfd,char * type,char * num);
int send_changeStaffInformation(int newfd);

int send_selectStaffInformation(int newfd,char * select);
int send_selectAllStaffInformation(int newfd);
int send_selectMyInformation(int newfd,char * num);

int send_changeMyInformation(int newfd,char * type,char * num);
int send_quit(int newfd,char * num,int isAttendant);
#endif
