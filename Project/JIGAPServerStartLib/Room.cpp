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
	{
		Iter->WriteBuffer(inMessage);
		Iter->IOCPSend();
	}
}

void Room::AddUser(LPTCPSOCK lpSocket)
{
	liUserList.push_back(lpSocket);
}

void Room::DeleteUser(LPTCPSOCK lpSocket)
{
	liUserList.remove(lpSocket);
}
