#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<sys/uio.h>
struct Buffer {
	//指向内存的指针
	char* data;
	int capacity;
	int readPos;
	int writePos;
};
//初始化
struct Buffer* bufferInit(int size);
//销毁
void bufferDestory(struct Buffer* buf);
//扩容
void bufferExtendRoom(struct Buffer* buffer, int size);
//得到剩余可写的内存容量
int bufferWriteableSize(struct Buffer* buffer);
//得到剩余可读的内存容量
int bufferReadableSize(struct Buffer* buffer);
//写内存1.直接写2.接受套接字数据
int bufferAppendData(struct Buffer* buffer, const char* data, int size);
int bufferAppendString(struct Buffer* buffer, const char* data);
int bufferSocketRead(struct Buffer* buffer, int fd);
//根据/r/n取出一行，找到其在数据块中的位置返回该位置
char* bufferFindCRLF(struct Buffer* buffer);
//发送数据
int bufferSendData(struct Buffer* buffer, int socket);


