//comm.h
#ifndef _COMM_H_
#define _COMM_H_
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#define PATHNAME "."
#define PROJ_ID 0x6666
#define SERVER_TYPE 1
#define CLIENT_TYPE 2
struct msgbuf
{
    long mtype;
    char mtext[1024]; 
};
int createMsgQueue(); //创建消息队列
int getMsgQueue();    //接受消息队列
int destroyMstQueue(int msgid);  //销毁消息队列
int senMsg(int msgid,int who,char* msg); //发送消息
int recvMsg(int msgid,int recvType,char out[]);//接收消息
#endif