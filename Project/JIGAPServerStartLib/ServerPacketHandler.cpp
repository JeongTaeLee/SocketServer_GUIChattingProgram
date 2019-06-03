#include "pch.h"
#include "ServerPacketHandler.h"


ServerPacketHandler::ServerPacketHandler()
	:bLoginAnswerPacket(false),
	bJoinedRoomAnswerPacket(false),
	bRoomListAnswerPacket(false),
	bExitRoomAnswer(false)
{
}


ServerPacketHandler::~ServerPacketHandler()
{
}

void ServerPacketHandler::SetLoginAnswerPacket(bool bInLoginSuccess)
{
	bLoginAnswerPacket = true;
	loginAnswerPacket.Clear();
	loginAnswerPacket.set_loginsuccess(bInLoginSuccess);
}

void ServerPacketHandler::SetJoinedRoomAnswerPacket(bool bInJoinedRoomSuccess, const std::string& strInRoomName)
{
	bJoinedRoomAnswerPacket = true;
	joinedRoomAnswerPacket.Clear();
	joinedRoomAnswerPacket.set_joinedroomsuccess(bInJoinedRoomSuccess);
	joinedRoomAnswerPacket.set_roomname(strInRoomName);

}

void ServerPacketHandler::SetRoomListAnswer(unsigned int iInRoomCount)
{
	bRoomListAnswerPacket = true;
	roomListAnswerPacket.Clear();
	roomListAnswerPacket.set_roomcount(iInRoomCount);
}

void ServerPacketHandler::AddRoomElement(const std::string& strInRoomName)
{
	JIGAPPacket::RoomElementPacket packet;
	packet.set_roomname(strInRoomName);

	liRoomElementPackets.push_back(packet);
}

void ServerPacketHandler::SerializeSend(LPTCPSOCK& lpTcpSocket)
{
	if (bLoginAnswerPacket)
	{
		bLoginAnswerPacket = false;
	}

	if (bJoinedRoomAnswerPacket)
	{
		bJoinedRoomAnswerPacket = false;
	}

	if (bRoomListAnswerPacket)
	{
		bRoomListAnswerPacket = false;
	}
}
