#include "Dispatcher.h"
#include<sys/epoll.h>
#include<stdlib.h>
#include<stdio.h>

#define Max 1024
struct SelectData {
	fd_set readSet;
	fd_set writeSet;
};
static void* selectInit();
static int selectAdd(struct Channel* channel, struct EventLoop* evLoop);
static int selectRemove(struct Channel* channel, struct EventLoop* evLoop);
static int selectModify(struct Channel* channel, struct EventLoop* evLoop);
static int selectDispatch(struct EventLoop* evLoop, int timeout);
static int selectClear(struct EventLoop* evLoop);
static void setFdset(struct Channel* channel, struct SelectData* data);
static void clearFdset(struct Channel* channel, struct SelectData* data);

struct Dispatcher SelectDispatcher = {
	selectInit,
	selectAdd,
	selectRemove,
	selectModify,
	selectDispatch,
	selectClear
};
static void* selectInit() {
	struct SelectData* data = (struct SelectData*)malloc(sizeof(struct SelectData));
	FD_ZERO(&data->readSet);
	FD_ZERO(&data->writeSet);

	return data;
}

static void setFdset(struct Channel* channel, struct SelectData* data) {
	if (channel->events & ReadEvent) {
		FD_SET(channel->fd, &data->readSet);
	}
	if (channel->events & WriteEvent) {
		FD_SET(channel->fd, &data->writeSet);
	}
}
static void clearFdset(struct Channel* channel, struct SelectData* data) {
	if (channel->events & ReadEvent) {
		FD_CLR(channel->fd, &data->readSet);
	}
	if (channel->events & WriteEvent) {
		FD_CLR(channel->fd, &data->writeSet);
	}

}

static int selectAdd(struct Channel* channel, struct EventLoop* evLoop) {
	struct SelectData* data = (struct SelectData*)evLoop->dispatcherData;
	if (channel->fd >= Max) {
		return -1;
	}
	setFdset(channel, data);
	
	return 0;
}
static int selectRemove(struct Channel* channel, struct EventLoop* evLoop) {
	struct SelectData* data = (struct SelectData*)evLoop->dispatcherData;
	clearFdset(channel, data);
	//通过channel释放对应的TcpConnection资源
	channel->destoryCallback(channel->arg);
	return 0;
}
static int selectModify(struct Channel* channel, struct EventLoop* evLoop) {
	struct SelectData* data = (struct SelectData*)evLoop->dispatcherData;
	setFdset(channel, data);
	clearFdset(channel, data);
	return 0;
}
static int selectDispatch(struct EventLoop* evLoop, int timeout) {
	struct SelectData* data = (struct SelectData*)evLoop->dispatcherData;
	struct timeval val;
	val.tv_sec = timeout;
	val.tv_usec = 0;
	fd_set rdtmp= data->readSet;
	fd_set wrtmp= data->writeSet;
	int count = select(Max, &rdtmp, &wrtmp,NULL, &val);
	if (count == -1) {
		perror("select");
		exit(0);
	}
	for (int i = 0;i <Max;++i) {
		if (FD_ISSET(i,&rdtmp)) {
			eventActivate(evLoop, i, ReadEvent);
		}
		if (FD_ISSET(i, &wrtmp)){
			eventActivate(evLoop, i, WriteEvent);
		}
	}
	return 0;
}
static int selectClear(struct EventLoop* evLoop) {
	struct SelectData* data = (struct SelectData*)evLoop->dispatcherData;
	free(data);
	return 0;
}