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

        int query(const string query);
        int show();
        int insertNewStudent(const SaveData& data);
        int searchStudentsIfExists(string name,string id);

        int toStudentDataSource(DataSource& data_source);
    private:
        MYSQL* mysql;
        MYSQL_RES* result;
        MYSQL_ROW row; 
};

#endif


