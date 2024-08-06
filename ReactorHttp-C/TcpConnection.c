#include "TcpConnection.h"

//#define MSG_SEND_AUTO
int processRead(void* arg) {
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	//��������
	int count = bufferSocketRead (conn->readBuf, conn->channel->fd);
	if (count > 0) {
		//���յ���http���󲢽���
		int socket = conn->channel->fd;
#ifdef MSG_SEND_AUTO
		writeEventEnable(conn->channel, true);
		eventLoopAddTask(conn->evLoop, conn->channel, MODIFY);
#endif
		bool flag=parseHttpRequest(conn->request, conn->readBuf, conn->response, conn->writeBuf, socket);
		if (!flag) {
			//����ʧ��
			char* errMsg = "Http/1.1 400 Bad Request\r\n\r\n ";
			bufferAppendString(conn->writeBuf, errMsg);
		}
	}
	//�Ͽ�����
	eventLoopAddTask(conn->evLoop, conn->channel, DELETE);
	return 0;
}

int processWrite(void* arg) 
{
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	//��������
	int count = bufferSendData(conn->writeBuf, conn->channel->fd);
	if (count > 0) {
		//�ж������Ƿ�ȫ�����ͳ�ȥ��
		if (bufferReadableSize(conn->writeBuf) == 0) {
			//1.���ڼ��д�¼�--�޸�channel�б���ʱ���¼�
			writeEventEnable(conn->channel, false);
			//2.�޸�dispatcher���ļ���--�������ڵ�
			eventLoopAddTask(conn->evLoop, conn->channel, MODIFY);
			//3.ɾ������ڵ�
			eventLoopAddTask(conn->evLoop, conn->channel, DELETE);
		}
	}
	return 0;
}
struct TcpConnection* tcpConnectionInit(int fd, struct EventLoop* evLoop)
{
	struct TcpConnection* conn = (struct TcpConnection*)malloc(sizeof(struct TcpConnection));
	conn->evLoop = evLoop;
	conn->readBuf = bufferInit(10240);
	conn->writeBuf = bufferInit(10240);
	conn->request = httpRequestInit();
	conn->response = httpResponseInit();
	sprintf(conn->name, "Connection-%d", fd);
	conn->channel = channelInit(fd, ReadEvent, processRead, processWrite,tcpConnectionDestory, conn);
	eventLoopAddTask(evLoop,conn->channel,ADD);
	return conn;
}

int tcpConnectionDestory(void* arg)
{
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	if (conn != NULL)
	{
		if (conn->readBuf && bufferReadableSize(conn->readBuf) == 0 &&
			conn->writeBuf && bufferReadableSize(conn->writeBuf) == 0)
		{
			destroyChannel(conn->evLoop, conn->channel);
			bufferDestroy(conn->readBuf);
			bufferDestroy(conn->writeBuf);
			httpRequestDestroy(conn->request);
			httpResponseDestroy(conn->response);
			free(conn);
		}
	}
	Debug("���ӶϿ�, �ͷ���Դ, gameover, connName: %s", conn->name);
	return 0;
}
