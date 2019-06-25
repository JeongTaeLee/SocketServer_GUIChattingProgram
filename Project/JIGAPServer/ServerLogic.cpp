#include "pch.h"
#include "ServerLogic.h"
#include "JIGAPServer.h"
#include "JIGAPPacket.pb.h"


void ServerLogic::PacketProcess(JIGAPPacket::LoginRequest& packet)
{
	auto userList = lpServer->GetUsers();

	bool success = true;
	for (auto Iter : userList)
	{
		if (Iter.second->GetUserNickName() == packet.id)
			success = false;
	}

	if (success)
	{

	}
	else
	{

	}
}
