#include "sql_api.h"

void myInsert(char* arg)
{
	char buf[1024];
	char *argv[6];   //name sex age hobby school '\0' 
	strcpy(buf,arg);

	char* start = buf;
	int i = 0;

	//参数存在才进行判断
	while(*start)
	{
		if(*start == '=')  //= 分离 name | value
		{
			start++;
			argv[i++] = start;
			continue;
		}
		if(*start == '&') // & 分离 name=value | name=value
		{
			*start = '\0';
		}
		start++;
	}
	argv[i] = NULL;
	sqlApi mydb("127.0.0.1",3306);  //用户的主机信息及数据库端口
	mydb.sql_connect(); //数据库连接

	mydb.sql_insert(argv[0],argv[1],argv[2],argv[3],argv[4]); //插入操作
}

int main()
{
	char buf[1024];
	int content_len = -1;
	char *method = NULL;
	char *query_string = NULL;
	char *string_arg = NULL;

	//检测到METHOD环境变量，才能判断请求方法
	if((method=getenv("METHOD")))
	{

		//get方法，参数在query_string中
		if(strcasecmp(method,"GET") == 0)
		{
			if((query_string = getenv("QUERY_STRING")))
				string_arg = query_string;
		}

		//post方法，参数在请求正文
		//需通过content_length来确定参数的长度及位置
		else
		{
			if(getenv("CONTENT_LENGTH"))
			{
				content_len = atoi(getenv("CONTENT_LENGTH"));
				int i = 0;
				for(;i < content_len;i++)
				{
					read(0,&buf[i],1);//每次读取一个字符，读取content_length的长度
				}
				buf[i] = '\0';
				string_arg = buf; //将参数存储于string_arg中
			}
		}
	}

	myInsert(string_arg); //将获取到的参数存储到数据库中
	return 0;
}
