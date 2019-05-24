#pragma once
#define MAXBUFFERSIZE 1024 // 이름 바이트

#define MAXNAMESIZE 64
#define MAXSTRSIZE 512
#define MAXROOMNAMESIZE 512

const uint32_t loginLiteral = 'LGIN';
const uint32_t joinedRoomLiteral = 'JNRM';


enum JIGAPSTATE
{
	E_LOGIN,
	E_NOTROOM,
	E_ROOM,
	E_CHATTING,
};

#include "SocketAddress.h"
#include "TCPIOData.h"
#include "TCPSocket.h"
#include "SerializeObject.h"
#include "Room.h"
