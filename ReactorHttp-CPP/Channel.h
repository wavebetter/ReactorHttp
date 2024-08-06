#pragma once
#include<functional>
using namespace std;
// 定义函数指针
//typedef int(*handleFunc)(void* arg);
//using handleFunc = int(*)(void*);

// 定义文件描述符的读写事件
enum class FDEvent
{
    TimeOut = 0x01,
    ReadEvent = 0x02,
    WriteEvent = 0x04
};

class Channel
{
public:
    using handleFunc = function<int(void*)>;//可调用对象包装器类型
    Channel(int fd, FDEvent events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg);
    // 回调函数
    handleFunc readCallback;
    handleFunc writeCallback;
    handleFunc destroyCallback;
    // 修改fd的写事件(检测 or 不检测)
    void writeEventEnable(bool flag);
    // 判断是否需要检测文件描述符的写事件
    bool isWriteEventEnable();
    //取出私有成员
    inline int getEvent() const {
        return m_events;
    }
    inline int getSocket() const{
        return m_fd;
    }
    inline void* getArg() const {
        return m_arg;
    }
private:
    // 文件描述符
    int m_fd;
    // 事件
    int m_events;
    // 回调函数的参数
    void* m_arg;
};

