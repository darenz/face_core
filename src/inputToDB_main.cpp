#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>

#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<vector>
#include"../seetalib/include/alignment/face_alignment.h"
#include"../seetalib/include/detection/face_detection.h"
#include"../seetalib/include/identification/face_identification.h"
#include"../include/classdef.hpp"
#include"../include/linkList.hpp"
#include"../include/db.hpp"
#include"../include/cvtstring.hpp"

using namespace std;

float* getFeatures(string dir);

int main()
{
    LinkList<SaveData> data_source;

   
//    seeta::FaceDetection face_detector(detectoer_model.c_str());
//    face_detector.SetMinFaceSize(40);
//    face_detector.SetScoreThresh(2.f);
//    face_detector.SetImagePyramidScaleFactor(0.8f);
//    face_detector.SetWindowStep(4,4);
//    seeta::FaceAlignment face_alignmenter(alignmenter_model.c_str());
//    seeta::FaceIdentification face_recognizer(identifier_model.c_str()) ;
//    int feature_size = face_recognizer.feature_size();
//    if(feature_size != 2048)
//    {
//	    cout<<"feature erro"<<std::endl;
//	    return 0;
//    }

    mysql_db db;
    
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(8001);
    myaddr.sin_addr.s_addr = 0;//localhost
    int flag = bind(server_fd,(struct sockaddr*)&myaddr,sizeof(struct sockaddr));
    cout<<"bind flag:"<<flag<<endl;
    if(flag==-1)
    {
        char s[50]={
        };
        perror(s);
        cout<<s<<endl;
    }
    char path_temp[50];
    char name_temp[50];
    char id_temp[50];
    
    listen(server_fd,128);
    while(1)
    {
       memset(path_temp,0,50);
        memset(name_temp,0,50);
        memset(id_temp,0,50);

        cout<<"------------waiting...----------"<<endl;

        //是否需要重复监听？
        //要不要效果都一样
        struct sockaddr re_addr;
        socklen_t re_l = sizeof(struct sockaddr);
        int new_socket = accept(server_fd,&re_addr,&re_l);
        cout<<"accepted:"<<new_socket<<endl;

        int path_l = recv(new_socket,path_temp,50,0);
        int name_l = recv(new_socket,name_temp,50,0);
        int id_l = recv(new_socket,id_temp,50,0);
        cout<<"path:"<<path_temp<<endl;
        cout<<"name:"<<name_temp<<endl;
        cout<<"id:"<<id_temp<<endl;
        if(path_l==0 || name_l==0 || id_l==0)
        {
            cout<<"recive information erro"<<endl;
            return 0;
        }
        
        int status ;
        string img_path = path_temp;
        string name = name_temp;
        string id = id_temp;
        cout<<"searching student..."<<endl;
        int db_f = db.searchStudentsIfExists(name,id);
        if(db_f == 1)
        {
            cout<<"inserting student..."<<endl;
            SaveData* p = new SaveData(); 
            float* temp = getFeatures(img_path);
            if(temp == NULL)
            {
                cout<<"get features erro"<<endl;
                return 0;
            }
            memcpy(p->features,temp,2048*sizeof(float));
            delete [] temp;
            temp = NULL;

            p->name = name;
            p->id = id;

            db.insertNewStudent(*p);
            p->~SaveData();
            p = NULL;
            status = 1; 
            cout<<endl;
        }
        else
        {
            cout<<"student exists"<<endl;
            status = -1;
        }
        send(new_socket,&status,sizeof(int),0);
    }
    close(server_fd);
}

float* getFeatures(string dir)
{
     /// initialize 
    string detectoer_model = "seetalib/model/seeta_fd.bin";
    string alignmenter_model = "seetalib/model/seeta_fa.bin";
    string identifier_model = "seetalib/model/seeta_fi.bin";
    
    seeta::FaceDetection detector(detectoer_model.c_str());
    detector.SetMinFaceSize(40);
    detector.SetScoreThresh(2.f);
    detector.SetImagePyramidScaleFactor(0.8f);
    detector.SetWindowStep(4,4);

    seeta::FaceAlignment alignmenter(alignmenter_model.c_str());
    seeta::FaceIdentification recognizer(identifier_model.c_str()) ;
    int feature_size = recognizer.feature_size();
    if(feature_size != 2048)
    {
        cout<<"feature erro"<<std::endl;
        return 0;
    }

    cv::Mat cv_img = cv::imread(dir,0);//gray scale
    if(cv_img.data == NULL)
    {
        cout<<"image not found"<<endl;
        return NULL;
    }
    seeta::ImageData seeta_img(cv_img.cols,cv_img.rows,cv_img.channels());
    seeta_img.data = cv_img.data;
    std::vector<seeta::FaceInfo> faces_v = detector.Detect(seeta_img);

    int32_t face_num = static_cast<int32_t>(faces_v.size());
    if(face_num == 0)
    {
        cout<<"no face found"<<endl;
        return NULL;
    }
    if(face_num>1)
    {
        cout<<"more than one face,not allowed"<<endl;
        return NULL;
    }

    seeta::FacialLandmark face_points[5];
    alignmenter.PointDetectLandmarks(seeta_img,faces_v[0],face_points);
    
    FaceFeatures feat = new float[feature_size];
    if(feat==NULL)
    {
        cout<<"new failed"<<endl;
        exit(0);
    }

    cv::Mat img_3channels = cv::imread(dir,1);
    seeta::ImageData seeta_img_3channels(img_3channels.cols,img_3channels.rows,img_3channels.channels());
    seeta_img_3channels.data = img_3channels.data;

    recognizer.ExtractFeatureWithCrop(seeta_img_3channels,face_points,feat);

    cv::Rect face_rect;
    face_rect.x = faces_v[0].bbox.x;
    face_rect.y = faces_v[0].bbox.y;
    face_rect.width = faces_v[0].bbox.width;
    face_rect.height = faces_v[0].bbox.height;
//// ?
/*
    uint l = faces_v.size();
    seeta::FaceInfo* pface;
    for(uint i=0;i<l;i++)
    {
        pface = &faces_v[i];
        delete pface;
    }
    */
//// ?

    /*
    cv::rectangle(img_3channels, face_rect, CV_RGB(0, 0, 255), 4, 8, 0);
    cv::namedWindow("img", cv::WINDOW_AUTOSIZE);
    cv::imshow("img", img_3channels);
    cv::waitKey(0);
    cv::destroyAllWindows();
    */
    return feat;
}

