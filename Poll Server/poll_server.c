#include <stdio.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
int main()
{
	struct pollfd evs;
	evs.fd = 0;
	evs.events = POLLIN;
	evs.revents = 0;
	int timeout = -1;//阻塞等待事件就绪
	while(1){
		switch(poll(&evs,1,timeout)){
                //超时
		        case 0:
				printf("timeout...\n");
				break;
                //失败
			case -1:
				perror("poll");
				break;
			default:
				{
                  //查看特定事件是否发生
					if(evs.revents & POLLIN){
						char buf[1024];
                       //read
						ssize_t s = read(evs.fd,buf,sizeof(buf)-1);
                       //读取成功回显
						if(s > 0){
							buf[s] = 0;
							printf("echo# %s\n",buf);
						}
						else{
							perror("read");
						}
					}
				}
				break;
		}
	}
}