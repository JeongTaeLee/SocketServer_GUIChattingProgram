#pragma once
#define MAXBUFFERSIZE 2048

typedef struct HANDLE_DATA
{
	SOCKET hSock;
	SOCKADDR_IN SockAddr;

	HANDLE_DATA();
	~HANDLE_DATA();

	
} *LPHANDLE_DATA;

typedef struct IO_DATA
{
	OVERLAPPED overlapped;
	char szBuffer[MAXBUFFERSIZE];
	WSABUF wsaBuf;

	IO_DATA();
	~IO_DATA();
} *LPIO_DATA;