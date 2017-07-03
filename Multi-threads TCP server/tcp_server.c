#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

static void usage(const char* proc)
{
	printf("Usage: %s[local_ip] [local_port]\n",proc);
}

int startup(const char* ip,int port)
{
	//先创建socket，再绑定赋值，监听
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0){
		perror("socket");
		exit(2);
	}
	printf("sock %d\n",sock);
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);

	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
		perror("bind");
		exit(3);
	}
	if(listen(sock,10) < 0){
		perror("listen");
		exit(4);
	}
	return sock;
}

void *request(void *arg)
{
	int new_sock = (int)arg;
	printf("get a new client");
	while(1){
		char buf[1024];
		ssize_t s = read(new_sock,buf,sizeof(buf) - 1);//读操作
		if(s > 0){
			buf[s] = 0;
			printf("client# %s\n",buf);
			write(new_sock,buf,strlen(buf));
		}
		else if(s == 0){
			printf("client close!\n");
			break;
		}
		else{
			perror("read");
			break;
		}
	}
}

int main(int argc,char* argv[])
{
	if(argc != 3){
		usage(argv[0]);
		return 1;
	}
	int listen_sock = startup(argv[1],atoi(argv[2]));
	while(1){
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
		if(new_sock < 0){
			perror("accept");
			continue;
		}
		//先read后write，多线程
		pthread_t id;
		pthread_create(&id,NULL,request,(void*)new_sock);
		pthread_detach(id);
	}
	return 0;
}
