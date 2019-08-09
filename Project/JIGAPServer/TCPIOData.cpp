#include "pch.h"
#include "TCPIOData.h"


TCPIOData::TCPIOData()
{
	OVERLAPPED::hEvent = NULL;
	OVERLAPPED::Internal = NULL;
	OVERLAPPED::InternalHigh = NULL;
	OVERLAPPED::Offset = NULL;
	OVERLAPPED::OffsetHigh = NULL;
	OVERLAPPED::Pointer = NULL;
	
	eIOMode = IOMODE::E_IOMODE_RECV;

	memset(szBuffer, 0, sizeof(szBuffer));
	memset(&wsaBuf, 0, sizeof(wsaBuf));
	wsaBuf.buf = szBuffer;
	wsaBuf.len = MAXBUFFERSIZE;
}


TCPIOData::~TCPIOData()
{
}
