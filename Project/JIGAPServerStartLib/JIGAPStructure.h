#pragma once
#define MAXBUFFERSIZE 1024 // �̸� ����Ʈ

enum JIGAPSTATE
{
	E_LOGIN,
	E_CHATTING,
};

#include "SocketAddress.h"
#include "TCPIOData.h"
#include "TCPSocket.h"
