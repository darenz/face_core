#include<iostream> 
#include<string>
#include<mysql/mysql.h>
#include"classdef.hpp"
#include"cvtstring.hpp"

#ifndef _MYDB
#define _MYDB

#define NOTEXISTS 1;
#define EXISTS 0;
using namespace std;
class mysql_db
{
    public:
        mysql_db();
        ~mysql_db()
        {
            mysql_close(mysql);
        }
        //mysql_db(string host,string user,string passwd,string db);

        int query(const string query);
        int show();
        int insertNewStudent(const SaveData& data);
        int searchStudentsIfExists(string name,string id);
        //int getStudentFeatures();

        int toStudentDataSource(DataSource& data_source);
    private:
        MYSQL* mysql;
        MYSQL_RES* result;
        MYSQL_ROW row; 
};

#endif


