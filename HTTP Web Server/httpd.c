#include "httpd.h"


//创建监听套接字
int startup(const char* ip,int port)
{
	//1.创建
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0){
		print_log("socket failed",FATAL);
		exit(1);
	}

	//保证服务器关机后可以立即重启
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	
	//2.绑定
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
		print_log("bind failed",FATAL);
		exit(2);
	}

	//3.监听 (监听队列长度不要设置太大)
	if(listen(sock,10) < 0){
		print_log("listen failed",FATAL);
		exit(3);
	}
	
	return sock;
}

//按行读取请求报头信息
//按字符读取，因为'\n'不明确
//可能存在:'\r' '\r\n' '\n'
static int get_line(int sock,char *buf,size_t size)
{
	char ch = '\0';
	int i = 0;

	while( ch != '\n' && i < (size-1))
	{
		int s = recv(sock,&ch,1,0);//与read一样，只是多了一个flag
		if(s > 0){
			//如果读到\r，需要判断下一个字符是否为\n
			if(ch == '\r')
			{
				//MSG_PEEK窥探下一个字符，防止本次读操作导致数据清除
				int ret = recv(sock,&ch,1,MSG_PEEK);
				if(ret > 0){
					if(ch == '\n'){
						recv(sock,&ch,1,0);
					}
					else{
						ch = '\n'; 
					}
				}
			}
			buf[i++] = ch;
		}
		else
		{
			ch = '\n';
		}
	}
	buf[i] = '\0';
	return i;  //返回这一行读到的字符个数
}

//日志信息
void print_log(char* log_msg,int level)
{
#ifdef _STDOUT_
	const char* const level_msg[] = {
		"SUCCESS",
		"NOTICE",
		"WARNING",
		"ERROR",
		"FATAL"
	};
	printf("[%s][%s]\n",log_msg,level_msg[level%5]);

#endif
}

//错误码
void echo_error(int sock,int err)
{
	switch(err)
	{
		case 401:
			break;
		case 404:
			break;
		case 500:
			break;
		case 503:
			break;
		default:
			break;
	}
}

//清除报头信息
static void clear_header(int sock)
{
	char buf[SIZE];
	int ret = 0;
	do{
		ret = get_line(sock,buf,sizeof(buf));
	}while(ret > 0 && strcmp(buf,"\n"));
}

//响应信息给浏览器
static void echo_www(int sock,char* path,int s)
{
	printf("path: %s\n",path);
	int fd = open(path,O_RDONLY);//以只读方式打开资源路径
	if(fd < 0){
		echo_error(sock,404);
		print_log("open",FATAL);
		return;
	}
	char buf[SIZE];
	//将响应行信息拼接在buf之后，多的一个\r\n是空行
	sprintf(buf,"HTTP/1.0 200 OK\r\n\r\n");
	printf("buf:%s\n",buf);
	//发送响应行
	if(send(sock,buf,strlen(buf),0) < 0){
		echo_error(sock,404);
		return ;
	}
	//发送文件
	if(sendfile(sock,fd,NULL,s) < 0){
		echo_error(sock,404);
		return ;
	}
	close(fd);
}

//浏览器请求连接
void *handler_request(void* arg)
{
	int ret = 0;
	int sock = (int)arg;
	char buf[SIZE];
	char method[SIZE]; //保存http请求行中使用的方法 get/post
	char url[SIZE];    //保存请求资源的路径
	char path[SIZE];   
	int i = 0;
	int j = 0;
	int cgi = 0;    //标记是否采用cgi模式
	char* query_string = NULL; //记录参数的地址


	int line = get_line(sock,buf,sizeof(buf));//获取请求行
	printf("%d:%s",line,buf);

	//获取请求行失败
	if(line < 0){
		print_log("get_line error",WARNING);
		goto end;
	}
	//成功后，截取请求方法
	while(j < sizeof(method)-1 && i < sizeof(buf)-1\
			&& !isspace(buf[i]))  //未到空行
	{
		method[j] = buf[i];
		i++;
		j++;
	}
	method[j] = '\0';

	//既不是get，又不是post方法，属于非法操作
	if(strcasecmp(method,"GET") && strcasecmp(method,"POST"))
	{
		echo_error(sock,404);
		goto end;
	}
	
	//过滤空格
	while(isspace(buf[i]) && i < sizeof(buf))
	{
		i++;
	}
	j = 0;
	//保存url
	while(!isspace(buf[i]) && (j < sizeof(url)-1)\
			&& (i < sizeof(buf)-1))
	{
		url[j] = buf[i];
		i++;
		j++;
	}
	url[j] = '\0';
	printf("method:%s\n",method);
	printf("url:%s\n",url);
	
	//post方法采用cgi模式
	if(strcasecmp(method,"POST") == 0){
		cgi = 1;
	}
	
	//get方法
	//路径与参数之间用?隔开
	//参数可有可无
	if(strcasecmp(method,"GET") == 0)
	{
		query_string = url;

		//查找'?'字符
		while(*query_string != '\0' && *query_string != '?')
		{
			query_string++;
		}
		//如果'?'存在，证明GET方法带参数
		if(*query_string == '?')
		{
			*query_string = '\0';
			query_string++; //参数的起始位置
			cgi = 1; //带参数使用cgi模式处理
		}
	}
	printf("query_string:%s\n",query_string);

	//将请求资源的路径保存在path中
	sprintf(path,"wwwroot%s",url);

	//如果请求的是目录，显示主页
	if(path[strlen(path)-1] == '/')
	{
		strcat(path,"index.html");
	}
	
	struct stat st;//记录浏览器所请求资源的信息
	printf("path:%s\n",path);

	//获取请求资源失败，返回404
	if(stat(path,&st) != 0){
		print_log("stat failed",FATAL);
		echo_error(sock,404);
		goto end;
	}
	//获取资源成功
	else
	{
		//请求的资源是一个目录时，显示主页
		if(S_ISDIR(st.st_mode))
		{
			strcat(path,"/index.html");
		}
		//满足所有者、所属组、其他人
		//任何一个条件，都是二进制文件
		//要变成可执行程序将结果返回浏览器
		//因此，必须使用cgi模式
		else if((st.st_mode & S_IXUSR)||\
				(st.st_mode & S_IXGRP)||\
				(st.st_mode & S_IXOTH))
		{
			cgi = 1;
		}
	}

	//cgi模式
	if(cgi == 1)
	{	
		ret = exe_cgi(sock,method,path,query_string);
	}

	else
	{
		clear_header(sock);//清空报头信息
		echo_www(sock,path,st.st_size);//响应信息给浏览器
	}
end:
	close(sock);
	return (void*)ret;
}

//cgi模式 (get+参数或post)
static int exe_cgi(int sock,char* method,char* path,char* query_string)
{
	int content_length = -1;
	char method_env[SIZE];
	char query_string_env[SIZE];
	char content_leng_env[SIZE];
	char buf[SIZE];
	int ret = 0;
	//若为get方法，参数在资源路径后面
	if(strcasecmp(method,"GET") == 0)
	{
		clear_header(sock);//清空报头信息
		printf("query_string arg:%s\n",query_string);//query_string记录的参数
	}
	//若为post方法，参数在请求正文
	else
	{
	
		//获取content-length参数，并清空报头信息
		do
		{
			ret = get_line(sock,buf,sizeof(buf));
		
			//在请求报头中获取消息正文数据的字节数(参数大小)
			if(strncasecmp(buf,"Content-Length: ",strlen("Content-Length: ") == 0))
			{
			    content_length = atoi(buf+strlen("Content_Length: "));

			}
		}while(ret >0 && strcmp(buf,"\n") != 0);

		//小于0.说明post方法的正文未传参数，出错返回
		if(content_length < 0)
		{
			echo_error(sock,404);
			return 1;
		}
	}
	
	//向浏览器发送响应行
	const char* echo_line = "HTTP/1.0 200 OK\r\n";
	send(sock,echo_line,strlen(echo_line),0);
	const char* type = "Content-Type:text/html;charset=ISO-8859-1\r\n";
	send(sock,type,strlen(type),0);
	const char* null_line = "\r\n";
	send(sock,null_line,strlen(null_line),0);

	printf("query_string:%s\n",query_string);

	int input[2];  //写管道(相对于子进程)
	int output[2]; //读管道(相对于子进程)

	//创建一个写管道，用于父子进程间通信
	if(pipe(input) < 0)
	{
		echo_error(sock,404);
		return 2;
	}

	//再创建一个读管道,用于父子进程间通信
	if(pipe(output) < 0)
	{
		echo_error(sock,404);
		return 3;
	}

	//fork一个子进程
	pid_t id = fork();
	if(id < 0){
		echo_error(sock,404);
		return 4;
	}
	
	//子进程去执行cgi程序
	else if(id == 0)//child
	{

		//对于子进程来说
		//我要关闭写管道的写端，因为我要读取父进程发送的数据
		//我要关闭读管道的读端，因为我要向父进程发送数据
		close(input[1]); //关闭写端
		close(output[0]); //关闭读端
		
		sprintf(method_env,"METHOD=%s",method);//将method设置为环境变量
		putenv(method_env); //导入环境变量

		//如果是get方法，将参数的地址设置为环境变量
		if(strcasecmp(method,"GET") == 0)
		{
			sprintf(query_string_env,"QUERY_STRING=%s",query_string);
			putenv(query_string_env);
		}

		//如果是post方法，将content_length设置为环境变量
		else
		{
			sprintf(content_leng_env,"CONTENT_LENGTH=%d",content_length);
			putenv(content_leng_env);
		}

		dup2(input[0],0);  //重定向标准输入
		dup2(output[1],1); //重定向标准输出

		//子进程替换资源路径path去执行cgi程序
		//第一个path:执行的资源路径
		//第二个path:在命令行上如何执行
		//命令行参数以NULL结尾
		execl(path,path,NULL);
		exit(1); //一旦失败直接终止
		
	}
	
	else //father
	{

		//对于父进程来说
		//我要关闭写管道的读端，因为我要向子进程发送数据
		//我要关闭读管道的写端，因为我要接收子进程的数据
		close(input[0]);  //关闭读端
		close(output[1]); //关闭写端

		char ch = '\0';

		//如果是POST方法，则父进程需要将参数通过
		//管道发送给子进程
		if(strcasecmp(method,"POST")== 0)
		{
			int i = 0;
			for(;i < content_length;i++)
			{
				recv(sock,&ch,1,0);
				write(input[1],&ch,1);
			}
		}
		ch = '\0';
		
	//	//向浏览器发送响应行
	//	char* s = "HTTP/1.0 200 OK \r\n\r\n";
	//	send(sock,s,strlen(s),0);


		//父进程读取子进程发送的数据，并返回结果至浏览器
		while(read(output[0],&ch,1) > 0)
		{
			send(sock,&ch,1,0);
		}
		waitpid(id,NULL,0);//阻塞式等待，不会影响主进程，此
	                       //进程是在线程中创建,是进程的一个执行分支
		close(input[1]);
		close(output[0]);
	}	
}

