#include "pch.h"
#include "UserTCPSocket.h"

UserTCPSocket::UserTCPSocket()
	:TCPSocket()
{
}

UserTCPSocket::UserTCPSocket(SOCKET InSocket)
	:TCPSocket(InSocket)
{
}

UserTCPSocket::UserTCPSocket(SOCKET hInSock, const SocketAddress& InAddr)
	:TCPSocket(hInSock, InAddr)
{
}

void UserTCPSocket::SetRoom(Room* lpInRoom)
{
	lpRoom = lpInRoom;
}

void UserTCPSocket::SetUserName(const std::string& strInName)
{
	strUserName = strInName;
}
