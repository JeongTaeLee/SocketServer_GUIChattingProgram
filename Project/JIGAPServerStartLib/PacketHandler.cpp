#include "pch.h"
#include "PacketHandler.h"

PacketHandler::PacketHandler()
{
	lpInputStream = static_cast<char*>(std::malloc(BASEALLOCSIZE));
	lpOutputStream = static_cast<char*>(std::malloc(BASEALLOCSIZE));

	iIStreamAllocSize = iOStreamAllocSize = BASEALLOCSIZE;
	
	iIStreamWriteSize = iOStreamWriteSize = 0;
}

PacketHandler::~PacketHandler()
{
}

bool PacketHandler::Write(const PacketHeader& header, google::protobuf::Message* lpInPacket, unsigned int iInSizes)
{
	return false;
}

void PacketHandler::WriteBaseRequest(JIGAPPacket::PacketType eInPacketType)
{
}

void PacketHandler::WriteBaseAnswer(JIGAPPacket::PacketType eInPacketType)
{
}

void PacketHandler::WriteLoginRequest(const std::string& strInNickName)
{
}

void PacketHandler::WriteLoginAnswer(bool bInSuccess)
{
}

void PacketHandler::WriteJoinedRoomRequest(const std::string& strInRoomName)
{
}

void PacketHandler::WriteJoinedRoomAnswer(bool bInSuccess, const std::string& strInRoomName)
{
}

void PacketHandler::WriteRoomListAnswer(int iInRoomCount)
{
}

void PacketHandler::WriteRoomElement(const std::string& strInRoomName)
{
}
