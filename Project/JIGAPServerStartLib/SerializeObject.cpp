#include "pch.h"
#include "SerializeObject.h"


SerializeObject::SerializeObject()
	:sendStreamSize(0), recvStreamSize(0)
{
	memset(sendStreamBuffer, NULL, sizeof(sendStreamBuffer));
	memset(recvStreamBuffer, NULL, sizeof(recvStreamBuffer));

	sendStreamHead = sendStreamBuffer;
	recvStreamHead = recvStreamBuffer;
}


SerializeObject::~SerializeObject()
{
}

void SerializeObject::SetRecvStreamBuffer(char* lpBuffer)
{
	memcpy(recvStreamBuffer, lpBuffer, sizeof(recvStreamBuffer));
}

void SerializeObject::ClearSendStreamBuffer()
{
	memset(sendStreamBuffer, 0, sizeof(sendStreamBuffer));
	sendStreamHead = sendStreamBuffer;
	sendStreamSize = 0;
}

void SerializeObject::ClearRecvStreamBuffer()
{
	memset(recvStreamBuffer, 0, sizeof(recvStreamBuffer));
	recvStreamHead = recvStreamBuffer;
	recvStreamSize = 0;
}
