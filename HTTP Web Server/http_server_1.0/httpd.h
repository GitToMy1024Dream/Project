#ifndef _HTTPD_
#define _HTTPD_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>

#define SIZE 1024

//告警级别
#define SUCCESS 0
#define NOTICE  1
#define WARNING 2
#define ERROR   3
#define FATAL   4


int startup(const char* ip,int port);//创建监听套接字

static int get_line(int sock,char* buf,size_t size);//按行读取请求报头

void *handler_request(void* arg);//浏览器处理连接

void print_log(char* log_msg,int level);//日志信息

static void clear_header(int sock); //清除报头信息

static void echo_www(int sock,char* path,int size); //响应信息给浏览器

void echo_error(int sock,int err); //错误码信息

static int exe_cgi(int sock,char* method,char* path,char* query_string);//cgi模式

#endif
