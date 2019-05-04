#include "pch.h"
#include "JIGAPStructure.h"

HANDLE_DATA::HANDLE_DATA()
	:hSock(INVALID_SOCKET)
{
	memset(&SockAddr, 0, sizeof(SockAddr));
}

HANDLE_DATA::~HANDLE_DATA()
{
}

IO_DATA::IO_DATA()
{
	memset(&overlapped, 0, sizeof(overlapped));
	memset(szBuffer, 0, sizeof(szBuffer));

	wsaBuf.len = MAXBUFFERSIZE;
	wsaBuf.buf = szBuffer;
}

IO_DATA::~IO_DATA()
{
}
