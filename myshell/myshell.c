#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <pwd.h>
#include <string.h>
void GetLogName()
{
    struct passwd* pf;
    pf = getpwuid(getuid());
    printf("[%s@",pf->pw_name);
}
void GetHostName()
{
    char name[128];
    gethostname(name,sizeof(name)-1);
    printf("%s",name);
}
void GetDiretion()
{
    char pwd[128];
    getcwd(pwd,sizeof(pwd)-1);
    int len = strlen(pwd);
    char *p = pwd+len-1;
    while(*p != '/')
    {
        p--;
    }
    p++;
    printf(" %s]# ",p);
}
int main()
{
    while(1)
    {
        GetLogName();//获取用户名
        GetHostName();//获取主机名
        GetDiretion();//获取当前路径
        fflush(stdout);//刷新标准输出缓冲区
        char buf[1024];//存放命令行参数的数组
        ssize_t s = read(0,buf,sizeof(buf)-1);//从标准输入0读取命令
        if(s > 0)//读取成功
        {
            buf[s-1] = 0;//去除键盘输入时读入的回车换行
        }
        char *_argv[32];//指针数组用来存放命令
        _argv[0] = buf;
        int i = 1;
        char *start = buf;
        while(*start)
        {
            if(*start == ' ')
            {
                *start = 0;
                start++;
                _argv[i++] = start;
            }
            else
                start++;
        }
        _argv[i] = NULL;
        //解析输出重定向符号
        if(strcmp(_argv[i-2],">") ==0)
        {
            _argv[i - 2] = NULL;
            pid_t id = fork();
            if(id < 0)//创建进程失败
                perror("fork");
            else if(id == 0)//child
            {
                close(1);
                open(_argv[i-1],O_WRONLY | O_CREAT,0664);
                execvp(_argv[0],_argv);
            }
            else//father
                wait(0);//等待子进程执行完  
        }
        else
        {
            pid_t id = vfork();
            if(id < 0)//进程创建失败
            {
                perror("vfork");
            }
            else if(id == 0)//child
            {
                execvp(_argv[0],_argv);//解释命令
            }
            else//father
                wait(0);//等待子进程执行完  
        }
    }
    return 0;
}