#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define BUFSIZE 1024

typedef struct epoll_msg
{
	int fd;
	char* buf;
}epoll_t,*epoll_p;

int startup(char* ip,int port)
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0){
		perror("socket");
		exit(1);
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	if(bind(sockfd,(struct sockaddr*)&local,sizeof(local)) < 0){
		perror("bind");
		exit(2);
	}
	if(listen(sockfd,5) < 0){
		perror("listen");
		exit(3);
	}
	return sockfd;

}

void usage(char* arg)
{
	printf("usage:%s [local_ip] [local_prot]\n",arg);
}


void alloc_fd(epoll_p ep,int fd)
{
	ep->fd = fd;
	ep->buf = (char*)malloc(BUFSIZE);
	printf("ep->buf:%p\n",ep->buf);
	if(ep->buf == NULL){
		perror("malloc");
		exit(5);
	}
}

int main(int argc,char* argv[])
{
	if(argc != 3){
		usage(argv[0]);
		exit(0);
	}
	int listen_sock = startup(argv[1],atoi(argv[2]));
	int epoll_fd = epoll_create(256);
	if(epoll_fd < 0){
		perror("epoll_create");
		exit(4);
	}
	printf("epoll_fd: %d,listen_sock: %d\n",epoll_fd,listen_sock);

	epoll_t fb;
	alloc_fd(&fb,listen_sock);
	struct epoll_event _ev;
	_ev.events = EPOLLIN;
	_ev.data.ptr = (void *)&fb;

	if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_sock,&_ev) < 0){
		perror("epoll_ctl");
		exit(6);
	}
	while(1)
	{
		struct epoll_event evs[64];
		int num;
		switch((num = epoll_wait(epoll_fd,evs,1024,-1)))
		{
			case -1:
				perror("epoll_wait");
				break;
			case 0:
				printf("timeout...\n");
				break;
			default:
				{
					int i = 0;
					for(; i < num; ++i)
					{
						epoll_p p = (epoll_p)(evs[i].data.ptr);
						int fd = p->fd;
						if(fd == listen_sock && (evs[i].events & EPOLLIN))
						{
							struct sockaddr_in client;
							socklen_t len = sizeof(client);
							int new_sockfd = accept(fd,(struct sockaddr*)&client,&len);
							if(new_sockfd < 0){
								perror("acccept");
								continue;
							}

							printf("get a connect [%s:%d]...\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
							_ev.events = EPOLLIN;
							epoll_t _fd;
							alloc_fd(&_fd,new_sockfd);
							_ev.data.ptr = (void*)&_fd;
							epoll_ctl(epoll_fd,EPOLL_CTL_ADD,new_sockfd,&_ev);

						}
						else if(fd != listen_sock && (evs[i].events & EPOLLIN))
						{
							char* buf = p->buf;
							printf("buf:%p\n",buf);
							int rd = read(fd,buf,BUFSIZE - 1);
							if(rd > 0){
								buf[rd] = 0;
								printf("client: %s\n",buf);
								_ev.events = EPOLLIN;
								epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&_ev);

							}
							else if( rd == 0){
								printf("client quit...\n");
								epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,NULL);
								close(fd);
								free(buf);
							}
							else
							{
								perror("read");
								exit(8);
							}
						}
						else if(fd != listen_sock && (evs[i].events & EPOLLOUT)){
							{
								char* buf = p->buf;
								char* msg="HTTP/1.1 200 OK\r\nComtent-length: 11\r\n\r\nhello epoll";
								strcpy(buf,msg);
								write(fd,buf,strlen(buf)+1);
								_ev.events = EPOLLIN;
								epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&_ev);
							//  close(fd);
							}
						}
					}
					break;
				}
		}
	}
}













