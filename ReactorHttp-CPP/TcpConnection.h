#pragma once
#include "EventLoop.h"
#include "Buffer.h"
#include "Channel.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

struct TcpConnection
{
public:
    TcpConnection(int fd,EventLoop* evLoop);
    ~TcpConnection();
    static int processRead(void* arg);
    static int processWrite(void* arg);
    static int destory(void* arg);
private:
    string m_name;
    EventLoop* m_evLoop;
    Channel* m_channel;
    Buffer* m_readBuf;
    Buffer* m_writeBuf;
    // http 协议
    HttpRequest* m_request;
    HttpResponse* m_response;
};

