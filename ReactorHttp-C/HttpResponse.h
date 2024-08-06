#pragma once
#include "Buffer.h"
#include "HttpResponse.h"
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//����״̬��ö��
enum HttpStatusCode {
    Unknown,
    OK = 200,
    MovedPermanently = 301,
    MovedTemporarily = 302,
    BadRequest = 400,
    NotFound = 404
};
// ������Ӧ�Ľṹ��
struct ResponseHeader
{
    char key[32];
    char value[128];
};

// ����һ������ָ��, ������֯Ҫ�ظ����ͻ��˵����ݿ�
typedef void (*responseBody)(const char* fileName, struct Buffer* sendBuf, int socket);

// ����ṹ��
struct HttpResponse
{
    // ״̬��: ״̬��, ״̬����
    enum HttpStatusCode statusCode;
    char statusMsg[128];
    char fileName[128];
    // ��Ӧͷ - ��ֵ��
    struct ResponseHeader* headers;
    int headerNum;
    responseBody sendDataFunc;
};

// ��ʼ��
struct HttpResponse* httpResponseInit();
// ����
void httpResponseDestroy(struct HttpResponse* response);
// ������Ӧͷ
void httpResponseAddHeader(struct HttpResponse* response, const char* key, const char* value);
// ��֯http��Ӧ����
void httpResponsePrepareMsg(struct HttpResponse* response, struct Buffer* sendBuf, int socket);
//�õ��ļ�����
const char* getFileType(const char* name);