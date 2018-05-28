#include<iostream>
#include"../include/db.hpp"
using namespace std;

mysql_db::mysql_db()
{
    mysql = mysql_init(NULL);
    //MYSQL* mysql
    //host
    //user
    //password
    //db
    //port
    //socket
    //client_flag
    if(!mysql_real_connect(mysql,"localhost","root","mysqlcoding","students",0,NULL,0))
    {
        cout<<"database connect failed"<<endl;
        exit(0);
    }
    cout<<"database connected~!"<<endl;
    result = NULL;
}

int mysql_db::query(const string query)
{
    if(result!=NULL)
        mysql_free_result(result);
    int q_flag = mysql_query(mysql,query.c_str());
    if(q_flag)
    {
        cout<<"query failed"<<endl;
        return ERRO;
    }
    result = mysql_use_result(mysql);
    if(result == NULL)
    {
        cout<<"use failed"<<endl;
        return 0;
    }
    return OK;
}

int mysql_db::show()
{
    uint num_fields = mysql_num_fields(result);
    while((row = mysql_fetch_row(result)))
    {
        unsigned long *lengths;
        lengths = mysql_fetch_lengths(result);
        for(uint i = 0; i < num_fields; i++)
        {
            printf("[%.*s] ", (int) lengths[i],
                    row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
    return OK;
}

int mysql_db::insertNewStudent(const SaveData& data)
{
    string name = data.name;
    string id = data.id;
    float* f = data.features;
    string convert;
    FloatArray2String(f,2048,convert);
    name = "\"" + name + "\"";
    convert = "\"" + convert + "\"";
    id = "\"" + id + "\"";
    cout<<"max_size:"<<convert.max_size()<<endl;
    cout<<"size:"<<convert.size()<<endl;
    string q = 
        "insert into students_table (name,id,features) values ("
         + name +','
         + id +','
         + convert + ')';
    //cout<<convert<<endl;
    
    if(mysql_query(mysql,q.c_str()))
    {
        cout<<"insert erro"<<endl;
        exit(0);
    }
    cout<<"insert succeed~!"<<endl;
    return OK;
}

int mysql_db::toStudentDataSource(DataSource& data_source)
{
    query("select * from students_table");
    //uint num_fields = mysql_num_fields(result);
    cout<<endl;
    while((row = mysql_fetch_row(result)))
    {
        //unsigned long *lengths;
        //lengths = mysql_fetch_lengths(result);
            string table_id = row[0];
            string name = row[1];
            string id = row[2];
            float* features = new float[2048];
            string s = row[3];
            String2FloatArray(s,2048,features);
            SaveData* ps = new SaveData(table_id,name,id,features)        ;
            data_source.append(*ps) ;

            cout<<"Find in db:"<<name<<" "<<id<<endl;
    }
    cout<<endl;
    return OK;
}

int mysql_db::searchStudentsIfExists(string name,string id)
{
    string q = "select name,id from students_table where exsits(select * from students_table where name="+name+" and id="+id+")";
    query(q);
    if(result==NULL)
        return NOTEXISTS;
    return EXISTS;
}
