#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<vector>
#include"../include/classdef.hpp"
#include"../seetalib/include/alignment/face_alignment.h"
#include"../seetalib/include/detection/face_detection.h"
#include"../seetalib/include/identification/face_identification.h"

using namespace std;

Producer::Producer(DataSource& tdata_source,string detectoer_model,string alignmenter_model,string identifier_model) : detector(detectoer_model.c_str()),alignmenter(alignmenter_model.c_str())
{
    identifier.LoadModel(identifier_model.c_str());
    
    for(uint i=0;i<tdata_source.size();i++)
    {
        this->data_source.append(tdata_source[i]);
    }
   
    detector.SetMinFaceSize(40);
    detector.SetScoreThresh(2.f);
    detector.SetImagePyramidScaleFactor(0.8f);
    detector.SetWindowStep(4,4);

            face_num = 0;
    feature_size = identifier.feature_size();
    cout<<feature_size<<endl;
    if(feature_size != 2048)
    {
        std::cout<<"feature_size erro"<<std::endl;
        exit(0);
    }
}

Status Producer::getImage(string img_dir)
{
    image = cv::imread(img_dir,1);
    if(image.data == NULL)
    {
        cout<<"img not found"<<endl;
        return ERRO;
    }
    gray = cv::imread(img_dir,0);
    if(gray.data == NULL)
    {
        cout<<"img not found"<<endl;
        return ERRO;
    }
    image_data.data = image.data;
    image_data.height = image.rows;
    image_data.width = image.cols;
    image_data.num_channels = image.channels();

    gray_data.data = gray.data;
    gray_data.height = gray.rows;
    gray_data.width = gray.cols;
    gray_data.num_channels = gray.channels();

    return OK;
}

Status Producer::generateFaces()
{
    if(gray_data.data == NULL)
        return ERRO;
    info_list = detector.Detect(gray_data);
    face_num = static_cast<int32_t>(info_list.size());
    if(face_num == 0)
    {
        std::cout<<"no face match"<<endl;
        return ERRO;
    }
    Student* s_p = NULL;
    for(int i=0;i<face_num;i++) 
    {
        s_p = new Student(); 
        s_p->face.face_info = info_list[i];

        //step 2
        alignmenter.PointDetectLandmarks(gray_data,s_p->face.face_info, s_p->face.landmark);
        //cout<<"alignmenter flag is :"<<flag<<endl;

        //step 3
        ///////////////////////////////////////
        //attention ! double new!
        //s_p->face.features = new float[2048];        
        /*if(s_p->face.features == NULL)
        {
            cout<<"new failed"<<endl;
            exit(0);
        }*/
        identifier.ExtractFeatureWithCrop(image_data,s_p->face.landmark,s_p->face.features);
        //cout<<"extract_f is:"<<extract_f<<endl;

        //get rectangle
        s_p->face.face_rectangle.x = s_p->face.face_info.bbox.x;
        s_p->face.face_rectangle.y = s_p->face.face_info.bbox.y;
        s_p->face.face_rectangle.width = s_p->face.face_info.bbox.width;
        s_p->face.face_rectangle.height = s_p->face.face_info.bbox.height;

        //push in student list
        student_list.append(*s_p); //use operator "=" of Student
    }
    return OK;
}

Status Producer::generateID()
{
    for(uint i=0;i<student_list.size();i++)
    {
        Status s = student_list[i].pullIdentity(data_source,&identifier);
        if(s == ERRO)
        {
            cout<<"pull identity erro"<<endl;
          //  return ERRO;
            exit(0);
        }
        else if(s == NOTFOUND)
        {
            cout<<"NOTFOUND"<<endl;
            continue;
        }
        else 
            continue;
    }
    return OK;
}

Status Producer::generateResult()
{
    ResultData* rp;
    uint size = student_list.size();
    cout<<"student_list size:"<<size<<endl;
    for(uint i=0;i<size;i++)
    {
        rp = new ResultData(student_list[i].identity.name,student_list[i].identity.id,student_list[i].face.face_rectangle);
        if(rp == NULL)
        {
            cout<<"new ResultData failed"<<endl;
            return ERRO;
        }
        results_list.append(*rp);//use operator "=" of ResultData
    }
    return OK;
}

Status Producer::sendResult(LinkList<ResultData>& results)
{
    int length = results_list.size();
    for(int i=0;i<length;i++)
        results.append(results_list[i]);
    return OK;
}

Status Producer::waitModifyCMD()
{
    ;
    return OK;
}

Status updateDataSource()
{
    ;
    return OK;
}
