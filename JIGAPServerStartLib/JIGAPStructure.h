#pragma once
#define MAXBUFFERSIZE 2048

enum JIGAPSTATE
{
	E_LOGIN,
	E_CHATTING,
};

#include "SocketAddress.h"
#include "TCPIOData.h"
#include "TCPSocket.h"
