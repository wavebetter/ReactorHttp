#pragma once
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include "Channel.h"
struct ChannelMap {
	int size;//��¼ָ��ָ��������Ԫ�ظ���
	//struct Channel* list[]
	struct Channel** list;
};
//��ʼ��
struct ChannelMap* channelMapInit(int size);
//���map
void ChannelMapClear(struct ChannelMap* map);
//���·����ڴ�ռ�
bool makeMapRoom(struct ChannelMap* map, int newSize, int unitSize);
