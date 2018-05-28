#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<vector>
#include"linkList.hpp"
#include"../seetalib/include/alignment/face_alignment.h"
#include"../seetalib/include/detection/face_detection.h"
#include"../seetalib/include/identification/face_identification.h"

using namespace std;

#ifndef _STUDENT_CLASS
#define _STUDENT_CLASS

#define OK 1
#define ERRO 0
#define NOTFOUND 2
//////////////typedef/////////////////
typedef int Status;
typedef cv::Rect LocationType ;
typedef cv::Mat InputImg ;

class SaveData
{
    public:
        string table_id;
        FaceFeatures features;
        string name;
        string id;
///what the fuck
        SaveData(){
            features = new float[2048];
            name = "none";
            id = "0000";
            table_id = "";
        }

        SaveData(string table_id,string name,string id,float* features)
        {
            this->table_id = table_id;
            this->name = name;
            this->id = id;
            this->features = new float[2048];
            memcpy(this->features,features,2048*sizeof(float));
        }

        ~SaveData()
        {
            delete [] features;
        }

        SaveData(const SaveData& otherS)
        {
            memcpy(this->features,otherS.features,2048*sizeof(float));
            this->name = otherS.name;
            this->id = otherS.id;
            this->table_id = otherS.table_id;
        }

        void operator=(const SaveData& otherS)
        {
            this->name = otherS.name;
            this->id = otherS.id;
            this->table_id = otherS.table_id;
            memcpy(this->features,otherS.features,2048*sizeof(float));
        }
};

typedef LinkList<SaveData> DataSource  ;


class FaceInformation
{
    public:
            seeta::FaceInfo face_info;
            seeta::FacialLandmark landmark[5];
            FaceFeatures features;
            cv::Rect face_rectangle;

            FaceInformation()
            {
                features = new float[2048];
            }
            ~FaceInformation()
            {
                delete [] features;
            }

            FaceInformation& operator =(const FaceInformation& otherF)
            {
                for(int i=0;i<2048;i++)
                {
                    this->features[i] = otherF.features[i];
                }
                return *this;
            }
};

class IdInformation
{
        public:
            IdInformation()
            {
                name = "none";
                id = "0000";
            }
            string name;
            string id;
};

class ResultData // result to send back
{
        public:
            ResultData()
            {
                memset(name,0,20);
                memset(id,0,20);
                string name_s = "none";
                memcpy(name,name_s.c_str(),name_s.length());
                string id_s = "0000";
                memcpy(id,id_s.c_str(),id_s.length());
            }

            ResultData(string name ,
                    string id,
                    cv::Rect face_rectangle)
            {
                memset(this->name,0,20);
                memset(this->id,0,20);
                string name_s = "none";
                memcpy(this->name,name.c_str(),name.length());
                memcpy(this->id,id.c_str(),id.length());
                this->face_rectangle = face_rectangle;
            }

            ResultData& operator =(const ResultData& otherR)
            {
                memcpy(this->name,otherR.name,strlen(otherR.name));
                memcpy(this->id,otherR.id,strlen(otherR.id));
                this->face_rectangle = otherR.face_rectangle;
                return *this;
            }

            char name[20];
            char id[20];
            cv::Rect face_rectangle;
};

class Student
{
    public:
        Student();

        Student& operator =(const Student& otherS);

        // get identity by face features from data base
        Status pullIdentity(DataSource& data_source,seeta::FaceIdentification* const identifier);

/*
        // if pull fail,add new information and corresponding features:
        Status setId(string name,string id);
        Status pushIdentityAndFeatures(DataSource& data_source);
 */       
        FaceInformation face; //input

        IdInformation identity;
};

class Producer
{
    public:
        Producer(DataSource& tdata_source,string detectoer_model,string alignmenter_model,string identifier_model);
        
        Status getImage(string path);

        Status generateFaces();

        Status generateID();
        
        // generate results to LinkList results_list
        Status generateResult();

        // send back to app
        Status sendResult(LinkList<ResultData>& results);

        // wait app CMD to modify the results got
        Status waitModifyCMD();

        // store 
        // informantion results && features
        Status updateDataSource();
        
        int getN()
        {
            return face_num;
        }

        ~Producer()
        {
            /*
            for(uint i=0;i<size;i++)
            {
                p = &info_list[i];
                delete p;
            }
            */
        }
       
    private:
        cv::Mat image;
        cv::Mat gray;
        DataSource data_source;
        seeta::ImageData image_data;
        seeta::ImageData gray_data;

        LinkList<Student> student_list;
        LinkList<ResultData> results_list;

        vector<seeta::FaceInfo> info_list;
        seeta::FaceDetection detector;
        seeta::FaceAlignment alignmenter;
        seeta::FaceIdentification identifier;

        int32_t face_num;
        int feature_size;
};

#endif
