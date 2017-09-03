#include "sql_api.h"
#include <stdio.h>

//初始化数据库
sqlApi::sqlApi(const std::string &_h,\
			   const int &_port,\
			   const std::string &_u,\
			   const std::string &_p,\
			   const std::string &_db)
{
	host = _h;
	user = _u;
	passwd = _p;
	db = _db;
	port = _port;
	res = NULL;
	conn = mysql_init(NULL);
}

//关闭数据库的连接
sqlApi::~sqlApi()
{
	mysql_close(conn);
}

//连接数据库
int sqlApi::sql_connect()
{
	//数据库连接成功
	if(mysql_real_connect(conn,host.c_str(),user.c_str(),"",db.c_str(),port,NULL,0))
	{
		cout<<"<html><body>";
		cout<<"<h1>connect success!</h1>"<<endl;
		cout<<"</body></html>";
	}
	else
	{
		cout<<"<html><body>";
		cout<<"<h1>connect failed!</h1>"<<endl;
		cout<<"</body></html>";
	}
}

//插入
int sqlApi::sql_insert(const std::string &_name,\
					   const std::string &_sex,\
					   const std::string &_age,\
					   const std::string &_hobby,\
					   const std::string &_school)
{
	//调用mysql的插入命令，与用户输入的信息拼接
	std::string sql = "insert into xatu_info (name,sex,age,hobby,school) values('";
	sql += _name;
	sql += "','";
	sql += _sex;
	sql += "','";
	sql += _age;
	sql += "','";
	sql += _hobby;
	sql += "','";
	sql += _school;
	sql += "')";
	
	//向与指定的连接标识符的数据库发送一条查询命令
	int ret = mysql_query(conn,sql.c_str());
	
	//mysql_query成功返回0，失败返回非0
	if(ret != 0)
	{
		cout<<"<html><body>";
		cout<<"<h1>insert failed!</h1>"<<endl;
		cout<<"</body></html>";
	}	
	else
	{
		cout<<"<html><body>";
		cout<<"<h1>insert succsee!</h1>"<<endl;
		cout<<"</body></html>";
	}
}

//查找
int sqlApi::sql_select()
{
	std::string sql = "select * from xatu_info";

	//同上，成功返回0，失败返回非0
	if(mysql_query(conn,sql.c_str())== 0)
	{
		//检测查询是否存在结果集
		//查询结果集失败返回0，成功返回非0
		res = mysql_store_result(conn);
		if(res)
		{
			//获取结果集中行的数目
			int nums = mysql_num_rows(res);

			//获取结果集中列的数目
			int col = mysql_num_fields(res);
			cout<<"nums:"<<nums<<" col:"<<col;
			MYSQL_FIELD *fd;

			//获取列名
			//res是mysql_query()产生的结果
			printf("<html><body>\n");
			printf("<table border=\"1\">");
			printf("<tr>\n");
			for(;fd = mysql_fetch_field(res);)
			{
				printf("<td>%s</td>",fd->name);
			}
			int i = 0;
			int j = 0;
			for(;i < nums;++i)
			{
				//获取列名
				MYSQL_ROW row_res = mysql_fetch_row(res);
				j = 0;
				printf("<tr>\n");
				for(;j < col;j++)
				{
					printf("<td>%s</td>\n",row_res[j]);//char** 
				}
				printf("</tr>\n");
			}
			printf("</table></body></html>");
		}
	}
}

