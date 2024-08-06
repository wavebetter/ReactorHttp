#pragma once
#include"EventLoop.h"
#include"ThreadPool.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include"TcpConnection.h"

struct Listener {
	int lfd;
	unsigned short port;
};
struct TcpServer {
	int threadNum;
	struct EventLoop* mainLoop;
	struct ThreadPool* threadPool;
	struct Listener* listener;
};
//初始化
struct TcpServer* tcpServerInit(unsigned short port, int threadNum);
//初始u话监听
struct Listener* listenerInit(unsigned short port);
//启动服务器
void tcpServerRun(struct TcpServer* server);

