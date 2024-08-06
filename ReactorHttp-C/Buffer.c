#include "Buffer.h"


struct Buffer* bufferInit(int size)
{
	struct Buffer* buffer = (struct Buffer*)malloc(sizeof(struct Buffer));
	if (buffer != NULL) {
		buffer->data = (char*)malloc(size);
		buffer->capacity = size;
		buffer->writePos = buffer -> readPos = 0;
		memset(buffer->data, 0, size);
	}
	return buffer;
}

void bufferDestory(struct Buffer* buf)
{
	if (buf != NULL) {
		if (buf->data != NULL) {
			free(buf->data);
		}
	}
}

void bufferExtendRoom(struct Buffer* buffer, int size)
{
	//������������1.�ڴ湻�� ����Ҫ����
	if (bufferWriteableSize(buffer)>=size) {
		return;
	}
	//2.�ڴ���Ҫ�ϲ��Ź��� ����Ҫ���� ʣ��Ŀ�д���ڴ�+�Ѷ��ڴ�>size
	else if(buffer->readPos + bufferWriteableSize(buffer) > size) {
		//�õ�δ���ڴ�Ĵ�С
		int readable = bufferReadableSize(buffer);
		//�ƶ��ڴ�
		memcpy(buffer->data, buffer->data + buffer->readPos, readable);
		//����λ��
		buffer->readPos = 0;
		buffer->writePos = readable;
	}
	
	//3.�ڴ治���� ����
	else {
		void*temp=realloc(buffer->data, buffer->capacity + size);
		if (temp == NULL) {//����ʧ��
			return;
		}
		memset(temp + buffer->capacity, 0, size);
		//��������
		buffer->data = temp;
		buffer->capacity += size;
	}
}

int bufferWriteableSize(struct Buffer* buffer)
{

	return buffer->capacity-buffer->writePos;
}

int bufferReadableSize(struct Buffer* buffer)
{

	return buffer->writePos-buffer->readPos;
}

int bufferAppendData(struct Buffer* buffer, const char* data, int size)
{
	if (buffer == NULL || data == NULL || data <= 0) {
		return -1;
	}
	//����
	bufferExtendRoom(buffer, size);
	//���ݿ���
	memcpy(buffer->data + buffer->writePos, data, size);
	buffer->writePos += size;
	return 0;
}

int bufferAppendString(struct Buffer* buffer, const char* data)
{
	int size = strlen(data);
	int ret = bufferAppendData(buffer, data, size);
	return ret;
}

int bufferSocketRead(struct Buffer* buffer, int fd)
{
	//read recv readv
	struct iovec vec[2];
	//��ʼ������Ԫ��
	int writeable = bufferWriteableSize(buffer);
	vec[0].iov_base = buffer->data+buffer->writePos;
	vec[0].iov_len = writeable;
	char* tmpbuf = (char*)malloc(40960);
	vec[1].iov_base = buffer->data + buffer->writePos;
	vec[1].iov_len = 40960;
	int result = readv(fd, vec, 2);
	if (result == -1) {
		return -1;
	}
	else if (result <= writeable) {
		buffer->writePos += result;
	}
	else {
		buffer->writePos = buffer->capacity;
		bufferAppendData(buffer, tmpbuf, result-writeable);
	}
	free(tmpbuf);
	return result;
}

char* bufferFindCRLF(struct Buffer* buffer)
{
	//strstr  ���ַ���ƥ�����ַ��� \0����
	//memmem  �����ݿ���ƥ�������ݿ�
	char* ptr=memmem(buffer->data + buffer->readPos, bufferReadableSize(buffer), "/r/n", 2);
	return ptr;
}

int bufferSendData(struct Buffer* buffer, int socket)
{
	//�ж���������
	int readable = bufferReadableSize(buffer);
	if (readable > 0) {
		int count = send(socket, buffer->data + buffer->readPos, readable);
		if (count) {
			buffer->readPos += count;
			usleep(1);
		}
		return count;
	}
	return 0;
}
