#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<sys/uio.h>
struct Buffer {
	//ָ���ڴ��ָ��
	char* data;
	int capacity;
	int readPos;
	int writePos;
};
//��ʼ��
struct Buffer* bufferInit(int size);
//����
void bufferDestory(struct Buffer* buf);
//����
void bufferExtendRoom(struct Buffer* buffer, int size);
//�õ�ʣ���д���ڴ�����
int bufferWriteableSize(struct Buffer* buffer);
//�õ�ʣ��ɶ����ڴ�����
int bufferReadableSize(struct Buffer* buffer);
//д�ڴ�1.ֱ��д2.�����׽�������
int bufferAppendData(struct Buffer* buffer, const char* data, int size);
int bufferAppendString(struct Buffer* buffer, const char* data);
int bufferSocketRead(struct Buffer* buffer, int fd);
//����/r/nȡ��һ�У��ҵ��������ݿ��е�λ�÷��ظ�λ��
char* bufferFindCRLF(struct Buffer* buffer);
//��������
int bufferSendData(struct Buffer* buffer, int socket);


