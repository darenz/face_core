#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include"../include/recvfile.hpp"
#include<string.h>
#include"../include/classdef.hpp"
#include<errno.h>
using namespace std;

int serverInit(int port)
{
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(8000);
    s_addr.sin_addr.s_addr = 0;
    bind(server_fd,(struct sockaddr*)&s_addr,sizeof(sockaddr));
    int l_flag = listen(server_fd,128);
    if(l_flag!=-1)
        cout<<"bind succeed,listening..."<<endl;
    else
    {
        cout<<"bind erro"<<endl;
        char s[50]={
        };
        perror(s);
        cout<<s<<endl;

    }
    return server_fd;
}

int fileRecv(int socket,const char* path)
{
    char temp[1024];
    int n = 0;
    FILE* file = fopen(path,"wb+");
    cout<<"reciving file..."<<endl;
    while((n=recv(socket,temp,1024,0))>0)
    {
        fwrite(temp,sizeof(char),n,file);
    }
    fclose(file);
    cout<<endl;
    cout<<"recived"<<endl;
    return 0;
}

int main()
{
    //int server_fd = serverInit(8000);

    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(8000);
    s_addr.sin_addr.s_addr = 0;
    bind(server_fd,(struct sockaddr*)&s_addr,sizeof(sockaddr));
    int l_flag = listen(server_fd,128);
    if(l_flag!=-1)
        cout<<"bind succeed,listening..."<<endl;
    else
    {
        cout<<"bind erro"<<endl;
        char s[50]={
        };
        perror(s);
        cout<<s<<endl;
        exit(0);
    }

    while(1)
    {
        cout<<endl;
        cout<<"-------------waiting...-------------"<<endl;
        struct sockaddr re_addr;
        socklen_t re_len;
        //需要重新listen 吗？
        //不一定，效果相同
        int new_fd = accept(server_fd,&re_addr,&re_len);

        cout<<"accept socket:"<<new_fd<<endl;
        
        if(new_fd==-1)
        {
            cout<<"disconnected"<<endl;
            exit(0);
        }
        unsigned char state;
        recv(new_fd,&state,sizeof(char),0);
        switch(state)
        {
            case 1 :
                insertStudent(new_fd) ;
                break;

            case 2 :
                scanForResults(new_fd);
                break;

            default:
                break;
        }
    }
    close(server_fd);
}

int insertStudent(int socket_s)
{
    string path = "insert.jpg";
    char name_temp[50];
    char id_temp[50];
    memset(name_temp,0,50);
    memset(id_temp,0,50);

    //注意顺序
    unsigned char name_l,id_l;
    
    recv(socket_s,&name_l,sizeof(char),0);
    recv(socket_s,name_temp,name_l,0);
    recv(socket_s,&id_l,sizeof(char),0);
    recv(socket_s,id_temp,id_l,0);

    cout<<"name:"<<name_temp<<endl;
    cout<<"id:"<<id_temp<<endl;
   
    fileRecv(socket_s,path.c_str());
    
    int socket_info = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in d_addr;
    d_addr.sin_family = AF_INET;
    d_addr.sin_port = htons(8001);
    d_addr.sin_addr.s_addr = 0;
    int c_flag = connect(socket_info,(struct sockaddr*)&d_addr,sizeof(struct sockaddr));
    if(c_flag == 0)
    {
        char path_c[50];
        strcpy(path_c,path.c_str());

        send(socket_info,path_c,strlen(path_c),0);
        send(socket_info,name_temp,strlen(name_temp),0);
        send(socket_info,id_temp,strlen(id_temp),0);
        int status;
        cout<<"waiting for status..."<<endl;
        recv(socket_info,&status,sizeof(int),0);
        cout<<"sending status to client(1:OK):"<<status<<endl;
        send(socket_s,&status,sizeof(status),0);
        close(socket_info);
    }
    else 
    {
        cout<<"connet to db process failed"<<endl;
    }
    return 0;
}

int scanForResults(int new_fd)
{
    string path="scanAll.jpg";
    
    fileRecv(new_fd,path.c_str());

    int socket_info = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in d_addr;
    d_addr.sin_family = AF_INET;
    d_addr.sin_port = htons(8002);
    d_addr.sin_addr.s_addr = 0;
    int c_flag = connect(socket_info,(struct sockaddr*)&d_addr,sizeof(struct sockaddr));
    if(c_flag != 0)
    {
        cout<<"connect erro"<<endl;
        exit(0);
    }
    else
        cout<<"connected to scanning program"<<endl;
    
    char path_temp[50];
    memset(path_temp,0,50);
    strcpy(path_temp,path.c_str());

    send(socket_info,path_temp,strlen(path_temp),0);
    
    unsigned char re_temp[100];
    int n; 
    cout<<"sending results to client..."<<endl;

    while((n  = recv(socket_info,re_temp,sizeof(ResultData),0)))
    {
        int send_n = send(new_fd,re_temp,sizeof(ResultData),0);
        ResultData* re = (ResultData*)re_temp ;
        cout<<"name:"<<re->name<<endl;
        cout<<"id:"<<re->id<<endl;
        if(send_n>0)
            cout<<"length:"<<send_n<<endl;
        else
            cout<<"send erro"<<endl;
    }
    cout<<">>>>>>>>all done"<<endl;
    cout<<endl;
    close(new_fd);
    close(socket_info);

    return 0;
}
