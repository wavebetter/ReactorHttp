#pragma once
#include "Buffer.h"
#include<map>
#include<functional>
using namespace std;

// 定义状态码枚举
enum class StatusCode
{
    Unknown,
    OK = 200,
    MovedPermanently = 301,
    MovedTemporarily = 302,
    BadRequest = 400,
    NotFound = 404
};

class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();
    // 添加响应头
    void addHeader(const string key, const string value);
    // 组织http响应数据
    void prepareMsg(Buffer* sendBuf, int socket);
    inline void setFileName(string name) {
        m_fileName = name;
    }
    inline void setStatusCode(StatusCode code) {
        m_statusCode = code;
    }
    function<void(const string, struct Buffer*, int)> sendDataFunc;

private:
    // 状态行: 状态码, 状态描述
    StatusCode m_statusCode;
    string m_fileName;
    // 响应头 - 键值对
    map<string, string> m_headers;
    //定义状态码和描述的对应关系
    const map<int, string>m_info = {
        {200,"OK"},
        {301,"MovedPermanently"},
        {302,"MovedTemporarily"},
        {400,"BadRequest"},
        {404,"NotFound"},
    };
};

