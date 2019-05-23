#pragma once
#define MAXBUFFERSIZE 2048

const int iHelloLiteral = 'HLLO';
const int iWelcomeLiteral = 'WECM';


enum JIGAPSTATE
{
	E_LOGIN,
	E_CHATTING,
};

#include "SocketAddress.h"
#include "TCPIOData.h"
#include "TCPSocket.h"
