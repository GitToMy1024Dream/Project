#ifndef _SQL_API_H_
#define _SQL_API_H_

#include <iostream>
#include <string>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

class sqlApi
{
public:
		sqlApi(const std::string &_h,       //构造函数
				const int &_port,\
				const std::string &_u="root",\
				const std::string &_p="",\
				const std::string &_db="student");
	
		~sqlApi();//析构函数
	
		int sql_connect(); //数据库连接

		int sql_insert(const std::string &_name,\
					   const std::string &_sex,\
					   const std::string &_age,\
					   const std::string &_hobby,\
					   const std::string &_school); //插入
	
	//	void sql_delete(const std::string &condition); //删除

	//	void sql_update(const string& info); //更改

		int sql_select(); //查找

private:
		sqlApi();
		MYSQL*  conn;
		MYSQL_RES* res;
		std::string host;
		std::string user;
		std::string passwd;
		std::string db;
		int port;
};



#endif
