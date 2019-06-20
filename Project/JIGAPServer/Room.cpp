#include "pch.h"
#include "Room.h"

void Room::RegisterUser(UserTCPSocket* lpInUserSocket)
{
	if (!lpInUserSocket)
		return;

	for (auto&Iter : liUserList)
	{
		if (Iter == lpInUserSocket)
			break;
	}

	lpInUserSocket->SetRoom(this);

	liUserList.push_back(lpInUserSocket);
}

void Room::UnRegisterUser(UserTCPSocket* lpInUserSocket)
{
	liUserList.remove(lpInUserSocket);
	lpInUserSocket->SetRoom(nullptr);
}

void Room::SendAlluser(const char* lpInBuffer, size_t iInSize)
{
	for (auto& Iter : liUserList)
		Iter->IOCPSend(lpInBuffer, iInSize);
}
