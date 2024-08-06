#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include"Server.h"
int main(int argc,char*argv[])
{
    if (argc < 3) {//参数个数小于3，提示用户输入正确的命令
        printf("./a.out port path\n");
        return -1;
    }
    unsigned short port = atoi(argv[1]);
    //切换服务器的工作路径
    chdir(argv[2]);
    //初始化用于监听的套接字
    int lfd = initListenFd(10000);
    //启动服务器程序
    epollRun(lfd);
    return 0;
}