#include "sql_api.h"

void mySelect(char* string_arg)
{
	sqlApi mydb("127.0.0.1",3306);//ip+port
	mydb.sql_connect();//连接数据库
	mydb.sql_select();//查询
}


int main()
{
	int content_len = -1;
	char buf[1024];
	char* method = NULL;
	char* query_string = NULL;
	char* string_arg = NULL;

	//检测到METHOD环境变量后，再获取请求方法
	if((method = getenv("METHOD")))
	{

		//get方法，参数在query_string中存储
		if((strcasecmp(method,"GET"))== 0)
		{
			//query_string存在时，将其转储于string_arg中
			if((query_string = getenv("QUERY_STRING")))
			{
				string_arg = query_string;
			}
		}


		//post方法,参数在请求正文，需要通过CONTENT_LENGTH来获取参数大小及位置
		else
		{
			if(getenv("CONTENT_LENGTH"))
			{
				content_len = atoi(getenv("CONTENT_LENGTH"));//强转成整型
				int i = 0;
				for(;i < content_len;++i) 
				{
					read(0,&buf[i],1);//每次读取一个字符，读取content_length次
				}
				buf[i] = '\0';
				string_arg = buf;
			}
		}
	}

	mySelect(string_arg); //查询数据库中的信息
	return 0;
}
