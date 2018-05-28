#include<iostream>
#include<string>
#include<sstream>

using namespace std;

int FloatArray2String(const float* array,const int length,string& out_string);
int String2FloatArray(const string& in_string,const int length,float* array);

int FloatArray2String(const float* array,const int length,string& out_string)
{
    ostringstream temp;
    for(int i=0;i<length;i++)
        temp << array[i] <<' ';
    out_string = temp.str();
    return 0;
}

int String2FloatArray(const string& in_string,const int length,float* array)
{
    istringstream str(in_string);
    for(int i=0;i<length;i++)
        str>>array[i];
    return 0;
}

/*
int main()
{
    float f[4] = {
        0,
        0.2323,
        1.2334,
       0 
    };
    
    float t[4];

    string out;
    FloatArray2String(f,4,out);
    cout<<"string:"<<out<<endl;
    String2FloatArray(out,4,t);
    cout<<"array:";
    for(int i=0;i<4;i++)
    {
        cout<<t[i]<<' ';
    }
    cout<<endl;
}
*/
