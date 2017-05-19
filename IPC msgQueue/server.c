//server.c
#include "comm.h"
int main()
{
    int msgid = getMsgQueue();
    char buf[1024];
    while(1)
    {
        buf[0] = 0;
        printf("Please Enter# ");
        fflush(stdout);
        ssize_t s = read(0,buf,sizeof(buf));
        if(s>0)
        {
            buf[s-1] = 0;
            sendMsg(msgid,CLIENT_TYPE,buf);
            printf("send done,wait recv...\n");
         }
        recvMsg(msgid,SERVER_TYPE,buf);
        printf("server# %s\n",buf);
    }
    return 0;
}