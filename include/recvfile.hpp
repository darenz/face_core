#include<iostream>
using namespace std;

#ifndef _RECV_FILE
#define _RECV_FILE

#define INSERT_STUDENT 1;
#define DO_SCAN 2;
int serverInit(int port);
int fileRecv(int socket,const char* path);
int insertStudent(int socket_s);
int scanForResults(int new_fd);

#endif
