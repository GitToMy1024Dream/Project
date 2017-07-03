#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static void usage(const char* proc)
{
	printf("%s [local_ip] [local_port]\n",proc);
}

int main(int argc,char* argv[])
{
	if(argc != 3){
		usage(argv[0]);
		return 1;
	}
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0){
		perror("socket");
		exit(2);
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[2]);
	if(connect(sock,(struct sockaddr*)&server,sizeof(server)) < 0){
		perror("connect");
		return 3;
	}
	//不需要绑定和监听
	//客户端需要先写后读
	char buf[1024];
	while(1)
	{
		printf("Please Enter# ");
		fflush(stdout);
		ssize_t s = read(0,buf,sizeof(buf)-1);
		if(s > 0){
			buf[s-1] = 0;
			write(sock,buf,strlen(buf));
			ssize_t _s = read(sock,buf,sizeof(buf) - 1);
			if(_s > 0){
				buf[_s] = 0;
				printf("server echo$ %s\b",buf);
			}
		}
	}
	close(sock);
	return 0;
}
