#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <stdlib.h>

int fds[sizeof(fd_set)*8];
//帮助文档
static usage(const char* proc)
{
	printf("Usage: %s [local_ip] [local_port]\n",proc);
}
//获取监听套接字
int startup(const char* ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0){
		perror("socket");
		exit(2);
	}
	int opt = 1;

	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	//绑定
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
		perror("bind");
		exit(3);
	}
	//监听
	if(listen(sock,10) < 0){
		perror("listen");
		exit(4);
	}
	return sock;
}

int main(int argc,char* argv[])
{
	if(argc != 3){
		usage(argv[0]);
		return 1;
	}
	//创建监听套接字
	int listen_sock = startup(argv[1],atoi(argv[2]));
	printf("fd_set: %d\n",sizeof(fd_set)*8);
	int nums = sizeof(fds)/sizeof(fds[0]);
	int i = 0;
	//将套接字数组初始化为-1
	for(;i < nums;i++){
		fds[i] = -1;
	}
	//默认数组的第一位为监听套接字
	fds[0] = listen_sock;
	fd_set rfds;//创建文件描述符集
	while(1){
		int maxfd = -1;//最大文件描述符的下标
		struct timeval timeout = {0,0};//轮询检测
		FD_ZERO(&rfds);//初始化文件描述符集
		i = 0;
		//查找最大的文件描述符
		for(;i < nums;i++){
			if(fds[i] == -1)
				continue;
			FD_SET(fds[i],&rfds);//设置fds_array[i]的位
			if(maxfd < fds[i]){
				maxfd = fds[i];
			}
		}
		//进行监听即等待
		switch(select(maxfd+1,&rfds,NULL,NULL,NULL)){
			case -1:
				perror("select");//出错
				break;
			case 0:
				printf("timeout...\n");//超时且未有fd就绪
				break;
			default:
				//至少有一个fd就绪
				i = 0;
				for(;i < nums;i++){
					//监听套接字就绪，获得了一个连接
					if(i == 0 && FD_ISSET(fds[i],&rfds)){
						struct sockaddr_in client;
						socklen_t len = sizeof(client);
						int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
						//连接失败
						if(new_sock < 0){
							perror("accept");
							continue;
						}
						printf("get a client: [%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
				
						int j = 1;
						for(;j < nums;j++){
						}
						//空间已用完，不能再添加客户端
						if(j == nums){
							close(new_sock);
						}
						else{
							fds[j] = new_sock;
						}
					}
					else if(i != 0 && FD_ISSET(fds[i],&rfds)){
						//正常的fd读取就绪
						char buf[1024];
						ssize_t s = read(fds[i],buf,sizeof(buf)-1);
						if(s > 0){
							buf[s] = 0;
							printf("client# %s\n",buf);
						}
						else if(s == 0){
							printf("client quit!\n");
							close(fds[i]);
							fds[i] = -1;
						}
						else {
							perror("read");
							close(fds[i]);
							fds[i] = -1;
						}
					}
					else{
						buf[s] = 0;
						printf("client# %s\n",buf);
					}
				}
		break;
		}
	}
	return 0;
}
