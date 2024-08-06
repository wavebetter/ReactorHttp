#include "HttpRequest.h"


#define HeaderSize 12
struct HttpRequest* httpRequestInit()
{
	struct HttpRequest* request = (struct HttpRequest*)malloc(sizeof(struct HttpRequest));
	httpRequestReset(request);
	request->reqHeaders = (struct RequestHeader*)malloc(sizeof(struct RequestHeader));
	request->reqHeadersNum = 0;
	return request;
}

void httpRequestReset(struct HttpRequest* req)
{
	req->curState = ParseReqLine;
	req->method = NULL;
	req->url = NULL;
	req->version = NULL;
	req->reqHeadersNum = 0;

}

void httpRequestResetEx(struct HttpRequest* req)
{
	free(req->url);
	free(req->method);
	free(req->version);
	for (int i = 0;i < req->reqHeadersNum;++i) {
		free(req->reqHeaders[i].key);
		free(req->reqHeaders[i].value);
	}
	httpRequestReset(req);
}

void httpRequestResetDestory(struct HttpRequest* req)
{
	if (req != NULL) {
		httpRequestResetEx(req);
		if (req->reqHeaders != NULL) {
			httpRequestResetEx(req);
			free(req);
		}
	}
}

enum HttpRequestState httpRequestState(struct HttpRequest* request)
{
	return request->curState;
}

void httpRequestAddHeader(struct HttpRequest* request, const char* key, const char* value)
{
	request->reqHeaders[request->reqHeadersNum].key = (char*)key;
	request->reqHeaders[request->reqHeadersNum].value = (char*)value;
	request->reqHeadersNum++;
}

char* httpRequestGetHeader(struct HttpRequest* request, const char* key)
{
	if (request != NULL){
		for (int i = 0; i < request->reqHeadersNum; ++i){
			if (strncasecmp(request->reqHeaders[i].key, key, strlen(key)) == 0){
				return request->reqHeaders[i].value;
			}
		}
	}
	return NULL;
}

char* splitRequestLine(const char* start, const char* end, const char* sub, char** ptr)
{
    char* space = end;
    if (sub != NULL)
    {
        space = memmem(start, end - start, sub, strlen(sub));
        assert(space != NULL);
    }
    int length = space - start;
    char* tmp = (char*)malloc(length + 1);
    strncpy(tmp, start, length);
    tmp[length] = '\0';
    *ptr = tmp;
    return space + 1;
}

bool parseHttpRequestLine(struct HttpRequest* request, struct Buffer* readBuf)
{
    // 读出请求行, 保存字符串结束地址
    char* end = bufferFindCRLF(readBuf);
    // 保存字符串起始地址
    char* start = readBuf->data + readBuf->readPos;
    // 请求行总长度
    int lineSize = end - start;

    if (lineSize)
    {
        start = splitRequestLine(start, end, " ", &request->method);
        start = splitRequestLine(start, end, " ", &request->url);
        splitRequestLine(start, end, NULL, &request->version);
#if 0
        // get /xxx/xx.txt http/1.1
        // 请求方式
        char* space = memmem(start, lineSize, " ", 1);
        assert(space != NULL);
        int methodSize = space - start;
        request->method = (char*)malloc(methodSize + 1);
        strncpy(request->method, start, methodSize);
        request->method[methodSize] = '\0';

        // 请求的静态资源
        start = space + 1;
        char* space = memmem(start, end - start, " ", 1);
        assert(space != NULL);
        int urlSize = space - start;
        request->url = (char*)malloc(urlSize + 1);
        strncpy(request->url, start, urlSize);
        request->method[urlSize] = '\0';

        // http 版本
        start = space + 1;
        //char* space = memmem(start, end - start, " ", 1);
        //assert(space != NULL);
        //int urlSize = space - start;
        request->version = (char*)malloc(end - start + 1);
        strncpy(request->url, start, end - start);
        request->method[end - start] = '\0';
#endif

        // 为解析请求头做准备
        readBuf->readPos += lineSize;
        readBuf->readPos += 2;
        // 修改状态
        request->curState = ParseReqHeaders;
        return true;
    }
    return false;
}

bool parseHttpRequestLine(struct HttpRequest* request, struct Buffer* readBuf)
{
    // 读出请求行, 保存字符串结束地址
    char* end = bufferFindCRLF(readBuf);
    // 保存字符串起始地址
    char* start = readBuf->data + readBuf->readPos;
    // 请求行总长度
    int lineSize = end - start;

    if (lineSize)
    {
        start = splitRequestLine(start, end, " ", &request->method);
        start = splitRequestLine(start, end, " ", &request->url);
        splitRequestLine(start, end, NULL, &request->version);
#if 0
        // get /xxx/xx.txt http/1.1
        // 请求方式
        char* space = memmem(start, lineSize, " ", 1);
        assert(space != NULL);
        int methodSize = space - start;
        request->method = (char*)malloc(methodSize + 1);
        strncpy(request->method, start, methodSize);
        request->method[methodSize] = '\0';

        // 请求的静态资源
        start = space + 1;
        char* space = memmem(start, end - start, " ", 1);
        assert(space != NULL);
        int urlSize = space - start;
        request->url = (char*)malloc(urlSize + 1);
        strncpy(request->url, start, urlSize);
        request->method[urlSize] = '\0';

        // http 版本
        start = space + 1;
        //char* space = memmem(start, end - start, " ", 1);
        //assert(space != NULL);
        //int urlSize = space - start;
        request->version = (char*)malloc(end - start + 1);
        strncpy(request->url, start, end - start);
        request->method[end - start] = '\0';
#endif

        // 为解析请求头做准备
        readBuf->readPos += lineSize;
        readBuf->readPos += 2;
        // 修改状态
        request->curState = ParseReqHeaders;
        return true;
    }
    return false;
}

// 该函数处理请求头中的一行
bool parseHttpRequestHeader(struct HttpRequest* request, struct Buffer* readBuf)
{
    char* end = bufferFindCRLF(readBuf);
    if (end != NULL)
    {
        char* start = readBuf->data + readBuf->readPos;
        int lineSize = end - start;
        // 基于: 搜索字符串
        char* middle = memmem(start, lineSize, ": ", 2);
        if (middle != NULL)
        {
            char* key = malloc(middle - start + 1);
            strncpy(key, start, middle - start);
            key[middle - start] = '\0';

            char* value = malloc(end - middle - 2 + 1);
            strncpy(value, middle + 2, end - middle - 2);
            value[end - middle - 2] = '\0';

            httpRequestAddHeader(request, key, value);
            // 移动读数据的位置
            readBuf->readPos += lineSize;
            readBuf->readPos += 2;
        }
        else
        {
            // 请求头被解析完了, 跳过空行
            readBuf->readPos += 2;
            // 修改解析状态
            // 忽略 post 请求, 按照 get 请求处理
            request->curState = ParseReqDone;
        }
        return true;
    }
    return false;
}

bool parseHttpRequest(struct HttpRequest* request, struct Buffer* readBuf,
    struct HttpResponse* response, struct Buffer* sendBuf, int socket)
{
    bool flag = true;
    while (request->curState != ParseReqDone)
    {
        switch (request->curState)
        {
        case ParseReqLine:
            flag = parseHttpRequestLine(request, readBuf);
            break;
        case ParseReqHeaders:
            flag = parseHttpRequestHeader(request, readBuf);
            break;
        case ParseReqBody:
            break;
        default:
            break;
        }
        if (!flag)
        {
            return flag;
        }
        // 判断是否解析完毕了, 如果完毕了, 需要准备回复的数据
        if (request->curState == ParseReqDone)
        {
            // 1. 根据解析出的原始数据, 对客户端的请求做出处理
            processHttpRequest(request, response);
            // 2. 组织响应数据并发送给客户端
            httpResponsePrepareMsg(response, sendBuf, socket);
        }
    }
    request->curState = ParseReqLine;   // 状态还原, 保证还能继续处理第二条及以后的请求
    return flag;
}

// 处理基于get的http请求
bool processHttpRequest(struct HttpRequest* request, struct HttpResponse* response)
{
    if (strcasecmp(request->method, "get") != 0)
    {
        return -1;
    }
    decodeMsg(request->url, request->url);
    // 处理客户端请求的静态资源(目录或者文件)
    char* file = NULL;
    if (strcmp(request->url, "/") == 0)
    {
        file = "./";
    }
    else
    {
        file = request->url + 1;
    }
    // 获取文件属性
    struct stat st;
    int ret = stat(file, &st);
    if (ret == -1)
    {
        // 文件不存在 -- 回复404
        //sendHeadMsg(cfd, 404, "Not Found", getFileType(".html"), -1);
        //sendFile("404.html", cfd);
        strcpy(response->fileName, "404.html");
        response->statusCode = NotFound;
        strcpy(response->statusMsg, "Not Found");
        // 响应头
        httpResponseAddHeader(response, "Content-type", getFileType(".html"));
        response->sendDataFunc = sendFile;
        return 0;
    }

    strcpy(response->fileName, file);
    response->statusCode = OK;
    strcpy(response->statusMsg, "OK");
    // 判断文件类型
    if (S_ISDIR(st.st_mode))
    {
        // 把这个目录中的内容发送给客户端
        //sendHeadMsg(cfd, 200, "OK", getFileType(".html"), -1);
        //sendDir(file, cfd);
        // 响应头
        httpResponseAddHeader(response, "Content-type", getFileType(".html"));
        response->sendDataFunc = sendDir;
    }
    else
    {
        // 把文件的内容发送给客户端
        //sendHeadMsg(cfd, 200, "OK", getFileType(file), st.st_size);
        //sendFile(file, cfd);
        // 响应头
        char tmp[12] = { 0 };
        sprintf(tmp, "%ld", st.st_size);
        httpResponseAddHeader(response, "Content-type", getFileType(file));
        httpResponseAddHeader(response, "Content-length", tmp);
        response->sendDataFunc = sendFile;
    }

    return false;
}

void sendFile(const char* fileName, struct Buffer* sendBuf, int cfd)
{
    // 1. 打开文件
    int fd = open(fileName, O_RDONLY);
    assert(fd > 0);
#if 1
    while (1)
    {
        char buf[1024];
        int len = read(fd, buf, sizeof buf);
        if (len > 0)
        {
            // send(cfd, buf, len, 0);
            bufferAppendData(sendBuf, buf, len);
#ifndef MSG_SEND_AUTO
            bufferSendData(sendBuf, cfd);
#endif
        }
        else if (len == 0)
        {
            break;
        }
        else
        {
            close(fd);
            perror("read");
    }
}
#else
    off_t offset = 0;
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    while (offset < size)
    {
        int ret = sendfile(cfd, fd, &offset, size - offset);
        printf("ret value: %d\n", ret);
        if (ret == -1 && errno == EAGAIN)
        {
            printf("没数据...\n");
        }
    }
#endif
    close(fd);
}

void sendDir(const char* dirName, struct Buffer* sendBuf, int cfd)
{
    char buf[4096] = { 0 };
    sprintf(buf, "<html><head><title>%s</title></head><body><table>", dirName);
    struct dirent** namelist;
    int num = scandir(dirName, &namelist, NULL, alphasort);
    for (int i = 0; i < num; ++i)
    {
        // 取出文件名 namelist 指向的是一个指针数组 struct dirent* tmp[]
        char* name = namelist[i]->d_name;
        struct stat st;
        char subPath[1024] = { 0 };
        sprintf(subPath, "%s/%s", dirName, name);
        stat(subPath, &st);
        if (S_ISDIR(st.st_mode))
        {
            // a标签 <a href="">name</a>
            sprintf(buf + strlen(buf),
                "<tr><td><a href=\"%s/\">%s</a></td><td>%ld</td></tr>",
                name, name, st.st_size);
        }
        else
        {
            sprintf(buf + strlen(buf),
                "<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
                name, name, st.st_size);
        }
        // send(cfd, buf, strlen(buf), 0);
        bufferAppendString(sendBuf, buf);
#ifndef MSG_SEND_AUTO
        bufferSendData(sendBuf, cfd);
#endif
        memset(buf, 0, sizeof(buf));
        free(namelist[i]);
    }
    sprintf(buf, "</table></body></html>");
    // send(cfd, buf, strlen(buf), 0);
    bufferAppendString(sendBuf, buf);
#ifndef MSG_SEND_AUTO
    bufferSendData(sendBuf, cfd);
#endif
    free(namelist);
}