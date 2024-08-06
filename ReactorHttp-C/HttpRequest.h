#pragma once
#include "HttpRequest.h"
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include "TcpConnection.h"
#include <assert.h>
#include <ctype.h>
#include "Buffer.h"
#include <stdbool.h>
#include "HttpResponse.h"

//����ͷ��ֵ��
struct RequestHeader {
	char* key;
	char* value;
};
//��ǰ�Ľ���״̬
enum HttpRequestState {
	ParseReqLine,
	ParseReqHeaders,
	ParseReqBody,
	ParseReqDone,
};
//����http����ṹ��
struct HttpRequest {
	char* method;
	char* url;
	char* version;
	struct RequestHeader* reqHeaders;
	int reqHeadersNum;
	enum HttpRequestState curState;
};
//��ʼ��
struct HttpRequest* httpRequestInit();
//����
void httpRequestReset(struct HttpRequest*req);
void httpRequestResetEx(struct HttpRequest* req);
void httpRequestResetDestory(struct HttpRequest* req);
// ��ȡ����״̬
enum HttpRequestState httpRequestState(struct HttpRequest* request);
// �������ͷ
void httpRequestAddHeader(struct HttpRequest* request, const char* key, const char* value);
// ����key�õ�����ͷ��value
char* httpRequestGetHeader(struct HttpRequest* request, const char* key);
// ����������
bool parseHttpRequestLine(struct HttpRequest* request, struct Buffer* readBuf);
// ��������ͷ
bool parseHttpRequestHeader(struct HttpRequest* request, struct Buffer* readBuf);
// ����http����Э��
bool parseHttpRequest(struct HttpRequest* request, struct Buffer* readBuf,
	struct HttpResponse* response, struct Buffer* sendBuf, int socket);
//����http����Э��
bool processHttpRequest(struct HttpRequest* request, struct HttpResponse* response);
//�����ļ�
void sendFile(const char* fileName, struct Buffer* sendBuf, int cfd);
//����Ŀ¼
void sendDir(const char* dirName, struct Buffer* sendBuf, int cfd);
