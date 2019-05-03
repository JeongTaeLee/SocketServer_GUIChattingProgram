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
