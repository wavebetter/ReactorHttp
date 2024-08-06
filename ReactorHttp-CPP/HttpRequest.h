#pragma once
#include "Buffer.h"
#include "HttpResponse.h"
#include<map>
#include<string>
using namespace std;

// 当前的解析状态
enum class PrecessState:char
{
    ParseReqLine,
    ParseReqHeaders,
    ParseReqBody,
    ParseReqDone
};
// 定义http请求结构体
class HttpRequest
{
public:
    HttpRequest();
    ~HttpRequest();
    // 重置
    void reset();
    // 获取处理状态
    inline PrecessState getState() {
        return m_curState;
    }
    inline void setState(PrecessState state) {
        m_curState = state;
    }
    // 添加请求头
    void addHeader(const string key, const string value);
    // 根据key得到请求头的value
    string getHeader(const string key);
    // 解析请求行
    bool parseRequestLine(Buffer* readBuf);
    // 解析请求头
    bool parseRequestHeader(Buffer* readBuf);
    // 解析http请求协议
    bool parseRequest(Buffer* readBuf,HttpResponse* response, Buffer* sendBuf, int socket);
    // 处理http请求协议
    bool processRequest(HttpResponse* response);
    // 解码字符串
    string decodeMsg(string msg);
    const string getFileType(const string name);
    static void sendDir(const string dirName, Buffer* sendBuf, int cfd);
    static void sendFile(const string fileName, Buffer* sendBuf, int cfd);
    inline void setMethod(string method) {
        m_method=method;
    }
    inline void setUrl(string url) {
        m_url = url;
    }
    inline void setVersion(string version) {
        m_version = version;
    }
private:
    char* splitRequestLine(const char* start, const char* end, const char* sub
        , function<void(string)>callback);
    int hexToDec(char c);
    string m_method;
    string m_url;
    string m_version;
    map<string,string> m_reqHeaders;
    enum PrecessState m_curState;
};

