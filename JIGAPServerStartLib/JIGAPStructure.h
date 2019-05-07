#pragma once
#define MAXBUFFERSIZE 2048

enum OVLMODE
{
	E_OVLMODE_RECV = 0,
	E_OVLMODE_SEND,
};
typedef struct IO_DATA : public OVERLAPPED
{
	OVLMODE eOvlmode;
	
	char szBuffer[MAXBUFFERSIZE];
	WSABUF wsaBuf;

	IO_DATA();
	~IO_DATA();
} *LPIO_DATA;

typedef struct HANDLE_DATA
{
	LPIO_DATA lpIOData;
	
	SOCKET hSock;
	SOCKADDR_IN SockAddr;

	HANDLE_DATA();
	~HANDLE_DATA();


} *LPHANDLE_DATA;
