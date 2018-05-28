#include"../include/classdef.hpp"
using namespace std;
Student::Student()
{

}

Student& Student::operator=(const Student& otherS)
{
    this->face.face_info = otherS.face.face_info;
    this->face.face_rectangle = otherS.face.face_rectangle;
    //already newed in FaceInformation's construtor
    //this->face.features = new float[2048];
    for(int i=0;i<2048;i++)
    {
        this->face.features[i] = otherS.face.features[i];
    }
    for(int i=0;i<5;i++)
    {
        this->face.landmark[i] = otherS.face.landmark[i];
    }
    this->identity.id = otherS.identity.id;
    this->identity.name = otherS.identity.name;
    return *this;
}

Status Student::pullIdentity(DataSource& data_source,seeta::FaceIdentification* const identifier)
{
//    float sim_t=0;
    float sim_temp=0;
    float similarity=0;
    int point = 0;
    uint i;
    cout<<endl;
    cout<<"similaritis"<<endl;
    for(i=0;i<data_source.size();i++)
    {
        sim_temp = identifier->CalcSimilarity(face.features,data_source[i].features);
        cout<<sim_temp<<endl;
        if(sim_temp>similarity) 
        {
            point = i;
            similarity = sim_temp;
        }
        if(sim_temp == 0)
        {
            cout<<"CalcSimilarity ERRO"<<endl;
            return ERRO;
        }
    }
    cout<<endl;
    if(similarity < 0.5)
            return NOTFOUND;

    identity.name = data_source[point].name;
    identity.id = data_source[point].id;

    cout<<identity.name<<endl; 
    cout<<"final similarity is :"<<similarity<<endl;
    
    return OK;
}

