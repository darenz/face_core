#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include<errno.h>

#include<opencv2/opencv.hpp>
#include<string>
#include<vector>
#include"../seetalib/include/alignment/face_alignment.h"
#include"../seetalib/include/detection/face_detection.h"
#include"../seetalib/include/identification/face_identification.h"
#include"../include/classdef.hpp"
#include"../include/linkList.hpp"
#include"../include/db.hpp"
using namespace std;

void showResults(string path,LinkList<ResultData>& results);
int main()
{
    mysql_db db;
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(8002);
    myaddr.sin_addr.s_addr = 0;//localhost
    int flag = bind(server_fd,(struct sockaddr*)&myaddr,sizeof(struct sockaddr));
    cout<<"bind flag:"<<flag<<endl;
    if(flag == -1)
    {
        cout<<"bind erro"<<endl;
        char err[50]={};
        perror(err);
        cout<<err<<endl;
        exit(0);
    }
    listen(server_fd,128);

    while(1)
    {
        DataSource data_source;
        db.toStudentDataSource(data_source);

        cout<<endl;

        cout<<"----------------waiting...-------------"<<endl;

        struct sockaddr new_addr;
        socklen_t new_l=sizeof(struct sockaddr);
        int sock_info = accept(server_fd,&new_addr,&new_l);
        if(sock_info<0)
        {
            cout<<"accept erro"<<endl;
            char err[50]={};
            perror(err);
            cout<<err<<endl;
            exit(0);
        }
        else
            cout<<"accepted"<<endl;

        char path_temp[100];
        memset(path_temp,0,100);

        recv(sock_info,path_temp,100,0);
        string path = path_temp;
        cout<<"path:"<<path_temp<<endl;

        cout<<"generating..."<<endl;
        Producer producer(data_source,"seetalib/model/seeta_fd.bin","seetalib/model/seeta_fa.bin","seetalib/model/seeta_fi.bin");  
        producer.getImage(path);
        producer.generateFaces();
        producer.generateID();
        producer.generateResult();
        cout<<"generate done"<<endl;

        cout<<"sending results..."<<endl;
        LinkList<ResultData> results;
        producer.sendResult(results);
        for(int i=0;i<results.getLength();i++)
        {
            int n = send(sock_info,&results[i],sizeof(ResultData),0);
            if(n!=sizeof(ResultData))
            {
                cout<<"get result erro"<<endl;
                exit(0);
            }
        }
        cout<<"<<<<<<<<<<<<sent"<<endl;
        cout<<endl;
        close(sock_info);
        /*
        showResults("img_test.jpg",results);
        cv::waitKey(0);
        cv::destroyAllWindows();
        */
    }
    close(server_fd);
}

void showResults(string path,LinkList<ResultData>& results)
{
    cv::Mat img = cv::imread(path.c_str(),1);
    if(img.data == NULL)
    {
        cout<<"image not found"<<endl;
        exit(0);
    }
    for(int i=0;i<results.getLength();i++)
    {
        cv::Rect rec = results[i].face_rectangle;
        cv::rectangle(img,rec,CV_RGB(255, 0, 128),4,8,0);
        cv::putText(img,results[i].name,cv::Point(rec.x,rec.y),cv::FONT_HERSHEY_PLAIN,2,CV_RGB(255,0,128));
    }
    cv::imshow("img",img);
}

