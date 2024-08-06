#pragma once
#include"Dispatcher.h"
#include<stdbool.h>
#include "ChannelMap.h"
#include<pthread.h>

extern struct Dispatcher EpollDispatcher;
extern struct Dispatcher PollDispatcher;
extern struct Dispatcher SelectDispatcher;
//����ڵ���channel�ķ�ʽ
enum ElementType{ADD,DELETE,MODIFY};
//����������еĽڵ�
struct ChannelElement {
	int type;//��δ���ýڵ��е�channel
	struct Channel* channel;
	struct ChannelElement* next;
};
struct EventLoop {
	bool isQuit;
	struct Dispatcher* dispatcher;
	void* dispatcherData;
	//�������
	struct ChannelElement* head;
	struct ChannelElement* tail;
	//map
	struct ChannelMap* channelMap;
	//�߳�id,name,mutex
	pthread_t threadID;
	char threadName[32];
	pthread_mutex_t mutex;
	int socketPair[2];//�洢����ͨ�ŵ��ļ�������
};
//��ʼ��
struct EventLoop* eventLoopInit();//������߳�
struct EventLoop* eventLoopInitEx(const char*threadName);//������߳�
//������Ӧ��ģ��
int eventLoopRun(struct EventLoop* evLoop);
//��������ļ�������
int eventActivate(struct EventLoop* evLoop, int fd, int event);
//��������������
int eventLoopAddTask(struct EventLoop* evLoop, struct Channel* channel, int type);
//��������е�����
int eventLoopProcessTask(struct EventLoop* evLoop);
//����Dispatcher�еĽڵ�
int eventLoopAdd(struct EventLoop* evLoop, struct Channel* channel);
int eventLoopRemove(struct EventLoop* evLoop, struct Channel* channel);
int eventLoopModify(struct EventLoop* evLoop, struct Channel* channel);
//�ͷ�channel
int destroyChannel(struct EventLoop* evLoop, struct Channel* channel);