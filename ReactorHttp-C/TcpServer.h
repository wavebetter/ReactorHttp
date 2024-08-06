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
//��ʼ��
struct TcpServer* tcpServerInit(unsigned short port, int threadNum);
//��ʼu������
struct Listener* listenerInit(unsigned short port);
//����������
void tcpServerRun(struct TcpServer* server);

