#pragma once

class TCPSocket;

enum IOMODE
{
	E_IOMODE_RECV = 0,
	E_IOMODE_SEND,
};

class TCPIOData : public OVERLAPPED
{
private:
	IOMODE eIOMode;

	char szBuffer[MAXBUFFERSIZE];
	WSABUF wsaBuf;
public:
	TCPIOData();
	virtual ~TCPIOData();


	friend TCPSocket;
};

using LPIODATA = TCPIOData *;

