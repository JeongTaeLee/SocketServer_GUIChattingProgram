#include "pch.h"
#include "Room.h"
#include "TCPSocket.h"

Room::Room()
{
}


Room::~Room()
{
}

void Room::SendRoomMessage(const char* inMessage)
{
	for (auto& Iter : liUserList)
		Iter->IOCPSend();
}

void Room::AddUser(TCPSocket* lpSocket)
{
	liUserList.push_back(lpSocket);
	lpSocket->SetRoom(this);
}

void Room::DeleteUser(TCPSocket* lpSocket)
{
	liUserList.remove(lpSocket);
	lpSocket->SetRoom(nullptr);
}

void Room::SendToAllUser(const char* lpBuffer, size_t tSize)
{
	for (auto Iter : liUserList)
	{
		Iter->SetBufferData(lpBuffer, tSize);
		Iter->IOCPSend();
	}
}
