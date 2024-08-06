#include "ThreadPool.h"

struct ThreadPool* threadPoolInit(struct EventLoop* mainLoop, int count)
{
	struct ThreadPool* pool = (struct ThreadPool*)maloc(sizeof(struct ThreadPool));
	pool->index = 0;
	pool->isStart = false;
	pool->mainLoop = mainLoop;
	pool->threadNum = count;
	pool->workerThreads= (struct WorkerThread*)maloc(sizeof(struct WorkerThread)*count);
	return pool;
}

void threadPoolRun(struct ThreadPool* pool)
{
	//如果传入的指针为空或者线程池已经启动要停止
	assert(pool && !pool->isStart);
	//当前线程不是主线程则退出
	if (pool->mainLoop->threadID != pthread_self()) {
		exit(0);
	}
	pool->isStart = true;
	if (pool->threadNum) {
		for (int i = 0;i < pool->threadNum;++i) {
			workerThreadInit(&pool->workerThreads[i],i);
			workerThreadRun(&pool->workerThreads[i]);
		}
	}
}

struct EventLoop* takeWorkerEventLoop(struct ThreadPool* pool)
{
	assert(pool->isStart);
	if (pool->mainLoop->threadID != pthread_self()) {
		exit(0);
	}
	//从线程池中找一个子线程，然后取出里面的反应堆实例
	struct EventLoop* evLoop = pool->mainLoop;
	if (pool->threadNum > 0) {
		evLoop = pool->workerThreads[pool->index].evLoop;
		pool->index = ++pool->index % pool -> threadNum;//确保不会超出范围
	}
	return evLoop;
}
