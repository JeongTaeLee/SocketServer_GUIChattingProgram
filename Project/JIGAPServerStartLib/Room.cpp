#include "pch.h"
#include "Room.h"


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
