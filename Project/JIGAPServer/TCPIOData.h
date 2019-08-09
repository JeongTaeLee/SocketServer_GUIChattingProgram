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
	char szBuffer[MAXBUFFERSIZE];
	WSABUF wsaBuf;
	
	IOMODE eIOMode;
public:
	TCPIOData();
	virtual ~TCPIOData();

	IOMODE GetIOMode() { return eIOMode; }
	void SetIOMode(IOMODE mode) { eIOMode = mode; }

	friend TCPSocket;
};

using LPIODATA = TCPIOData *;

