#include "httpd.h"

//帮助文档
static usage(const char* proc)
{
	printf("Usage:%s [local_ip] [local_port]\n",proc);
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		return 1;
	}
	int listen_sock = startup(argv[1],atoi(argv[2]));//获取监听套接字

	while(1)
	{
		/* 创建一个ipv4 socket地址 */
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		
		//接受连接
		/* accept只是从监听队列中取出连接，不论连接处于何种状态  */
		int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
		if(new_sock < 0){
			print_log("accept failed",WARNING);
			continue;
		}
	
		//得到一个新连接
		printf("get a client:[%s:%d]\n",\
				inet_ntoa(client.sin_addr),\
				ntohs(client.sin_port));

		pthread_t id;
		//创建线程
		//子线程执行handler_request方法
		int ret =pthread_create(&id,NULL,handler_request,(void *)new_sock);
		if(ret != 0){
			print_log("pthread_create failed",WARNING);
			close(new_sock);
		}
		//主线程进行分离(多线程)
		else{
			pthread_detach(id);
		}
	}
	close(listen_sock);
	return 0;
}
