#pragma once
#include<stdbool.h>
#include<stdlib.h>
//定义函数指针
typedef int(*handleFunc)(void* arg);
//定义文件描述符的读写事件
enum FDEvent {
	TimeOut=0x01,
	ReadEvent=0x02,
	WriteEvent=0x04
};
struct Channel {
	//文件描述符
	int fd;
	//事件
	int events;
	//回调函数
	handleFunc readCallback;
	handleFunc writeCallback;
	handleFunc destoryCallback;
	//回调函数的参数
	void* arg;
};
//初始化一个Channel
struct Channel* channelInit(int fd,int events,handleFunc readFunc,
	handleFunc writeCallback, handleFunc destoryCallback, void* arg);
//修改fd的写事件
void writeEventEnable(struct Channel* channel, bool flag);
//判断是否需要检测文件描述符的写事件
bool isWriteEventEnable(struct Channel* channel);
