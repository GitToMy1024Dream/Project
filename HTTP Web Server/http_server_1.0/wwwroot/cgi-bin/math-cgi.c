#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void math_cgi(char* arg)
{
	int i = 0;
	char* argv[3];//因为form中只提供了两个变量，再加上'\0'，因此大小给3即可
	char* start = arg;

	//参数存在时才进行判断
	while(*start)
	{
		if(*start == '=')   //= 分离name |  value
		{
			start++;
			argv[i++] = start;
			continue;
		}
		if(*start == '&')   //& 分离 name=value | name=value
		{
			*start = '\0'; 
		}
		start++;
	}
	argv[i] = NULL;

	//将传入的字符串转为整形
	int x = atoi(argv[0]); 
	int y = atoi(argv[1]);


	//html，执行两个数的+ - * / %
	printf("<html><bady>");
	printf("<h1>%d + %d = %d</h1><br/>",x,y,x+y);
	printf("<h1>%d - %d = %d</h1><br/>",x,y,x-y);
	printf("<h1>%d * %d = %d</h1><br/>",x,y,x*y);
	printf("<h1>%d / %d = %d</h1><br/>",x,y,y==0?0:x/y);
	printf("<h1>%d %% %d = %d</h1><br/>",x,y,y==0?0:x%y);
	printf("</bady></html>");
}


int main()
{
	char* method = NULL;
	char* query_string = NULL;
	char* string_arg = NULL;
	int content_length = -1;
	char buf[1024];

	//检测到METHOD环境变量，在判断请求方法
	if((method = getenv("METHOD")))
	{
		//GET方法，参数在query_string中存储
		if(strcasecmp(method,"GET") == 0)
		{
			if((query_string = getenv("QUERY_STRING")))
			{
				string_arg = query_string;
			}
		}

		//POST方法，参数在请求正文，需要借助content_length来获取参数大小，及参数位置
		else{
			if(getenv("CONTENT_LENGTH"))
			{
				content_length = atoi(getenv("CONTENT_LENGTH"));
				int i = 0;
				for(;i < content_length;++i)
				{
					read(0,&buf[i],1);   //每次读取一个字符，一共读取content_length个
				}
				buf[i] = '\0';
				string_arg = buf;
			}
		}

	}
	
	math_cgi(string_arg); //参数的执行函数
	return 0;
}
