#include<iostream>
#include<string>
#include<sstream>
#include<cstdlib>
#include<ctime>


using namespace std;
#ifndef _CVT_STRING
#define _CVT_STRING
int FloatArray2String(const float* array,const int length,string& out_string);
int String2FloatArray(const string& in_string,const int length,float* array);

#endif

