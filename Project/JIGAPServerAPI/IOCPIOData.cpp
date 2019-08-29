#include "pch.h"
#include "IOCPIOData.h"

JIGAPServerAPI::IOCPIOData::IOCPIOData()
{
	OVERLAPPED::hEvent = NULL;
	OVERLAPPED::Internal = NULL;
	OVERLAPPED::InternalHigh = NULL;
	OVERLAPPED::Offset = NULL;
	OVERLAPPED::OffsetHigh = NULL;
	OVERLAPPED::Pointer = NULL;

	eIoMode = IOMODE::E_IOMODE_RECV;

	memset(szBuffer, 0, sizeof(szBuffer));
	memset(&wsaBuf, 0, sizeof(wsaBuf));

	wsaBuf.buf = szBuffer;
	wsaBuf.len = MAXBUFFERLENGTH;
}
