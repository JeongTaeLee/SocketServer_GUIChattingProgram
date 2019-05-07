#include "pch.h"
#include "JIGAPStructure.h"

HANDLE_DATA::HANDLE_DATA()
	:lpIOData(nullptr), hSock(INVALID_SOCKET)
{
	memset(&SockAddr, 0, sizeof(SockAddr));
}

HANDLE_DATA::~HANDLE_DATA()
{
}

IO_DATA::IO_DATA()
{
	eOvlmode = E_OVLMODE_RECV;

	memset(szBuffer, 0, sizeof(szBuffer));

	wsaBuf.len = MAXBUFFERSIZE;
	wsaBuf.buf = szBuffer;
}

IO_DATA::~IO_DATA()
{
}
