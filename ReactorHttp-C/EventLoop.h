#pragma once
#include"Dispatcher.h"
#include<stdbool.h>
#include "ChannelMap.h"
#include<pthread.h>

extern struct Dispatcher EpollDispatcher;
extern struct Dispatcher PollDispatcher;
extern struct Dispatcher SelectDispatcher;
//处理节点中channel的方式
enum ElementType{ADD,DELETE,MODIFY};
//定义任务队列的节点
struct ChannelElement {
	int type;//如何处理该节点中的channel
	struct Channel* channel;
	struct ChannelElement* next;
};
struct EventLoop {
	bool isQuit;
	struct Dispatcher* dispatcher;
	void* dispatcherData;
	//任务队列
	struct ChannelElement* head;
	struct ChannelElement* tail;
	//map
	struct ChannelMap* channelMap;
	//线程id,name,mutex
	pthread_t threadID;
	char threadName[32];
	pthread_mutex_t mutex;
	int socketPair[2];//存储本地通信的文件描述符
};
//初始化
struct EventLoop* eventLoopInit();//针对主线程
struct EventLoop* eventLoopInitEx(const char*threadName);//针对子线程
//启动反应堆模型
int eventLoopRun(struct EventLoop* evLoop);
//处理激活的文件描述符
int eventActivate(struct EventLoop* evLoop, int fd, int event);
//添加任务到任务队列
int eventLoopAddTask(struct EventLoop* evLoop, struct Channel* channel, int type);
//处理队列中的任务
int eventLoopProcessTask(struct EventLoop* evLoop);
//处理Dispatcher中的节点
int eventLoopAdd(struct EventLoop* evLoop, struct Channel* channel);
int eventLoopRemove(struct EventLoop* evLoop, struct Channel* channel);
int eventLoopModify(struct EventLoop* evLoop, struct Channel* channel);
//释放channel
int destroyChannel(struct EventLoop* evLoop, struct Channel* channel);