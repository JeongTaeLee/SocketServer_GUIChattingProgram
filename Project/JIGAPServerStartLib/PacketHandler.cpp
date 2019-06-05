#include "pch.h"
#include "PacketHandler.h"

PacketHandler::PacketHandler()
{
	lpInputStream = static_cast<char*>(std::malloc(BASEALLOCSIZE));
	lpOutputStream = static_cast<char*>(std::malloc(BASEALLOCSIZE));

	iIStreamAllocSize = iOStreamAllocSize = BASEALLOCSIZE;
	
	iIStreamWriteSize = 0;
	iOStreamWriteSize += sizeof(unsigned int);
}

PacketHandler::~PacketHandler()
{
}

bool PacketHandler::Write(const PacketHeader& header, google::protobuf::Message* lpInPacket, unsigned int iInSizes)
{
	memcpy(&lpOutputStream[iOStreamWriteSize], )

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
	JIGAPPacket::LoginRequestPacket packet;
	packet.set_nickname(strInNickName);

	PacketHeader header;
	header.packetType = JIGAPPacket::PacketType::JoinedRoomRequestType;
	header.size = packet.ByteSize();


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

void PacketHandler::WriteTotalPacketSize(unsigned int size)
{
	memcpy(lpOutputStream, (char*)&size, sizeof(unsigned int));
}

void PacketHandler::ClearWirteBuffer()
{
	memset(&lpOutputStream, 0, iOStreamAllocSize);

	iOStreamWriteSize = sizeof(unsigned int);
	WriteTotalPacketSize(0);
}
