#include "pch.h"
#include "PacketHandler.h"

#include "Room.h"

PacketHandler::PacketHandler()
{
	lpInputStream = static_cast<char*>(std::malloc(MAXBUFFERSIZE));
	lpOutputStream = static_cast<char*>(std::malloc(MAXBUFFERSIZE));

	iIStreamAllocSize = iOStreamAllocSize = MAXBUFFERSIZE;
	
	iIStreamWriteSize = 0;
	iOStreamWriteSize = 0;

	ClearWirteBuffer();
}

PacketHandler::~PacketHandler()
{
	delete[] lpInputStream;
	delete[] lpOutputStream;
}

int PacketHandler::ParsingPacketSize(char* lpInBuffer)
{
	unsigned int bufferSize = 0;
	memcpy((void*)&bufferSize, lpInBuffer, sizeof(unsigned int));
	return bufferSize;
}

void PacketHandler::WriteLoginRequest(const std::string& strInNickName)
{
	JIGAPPacket::StringPacket strPacket;
	strPacket.set_str(strInNickName);

	bool b = this->WritePacket(JIGAPPacket::PacketType::LoginRequestType, strPacket);
}

void PacketHandler::WriteLoginAnswer(bool bInSuccess)
{
	JIGAPPacket::LoginAnswerPacket loginAnswer;
	loginAnswer.set_loginsuccess(bInSuccess);

	this->WritePacket(JIGAPPacket::PacketType::LoginAnswerType, loginAnswer);
}

void PacketHandler::WriteCreateRoomPacket(const std::string& strInRoomName)
{
	JIGAPPacket::StringPacket strPacket;
	strPacket.set_str(strInRoomName);

	this->WritePacket(JIGAPPacket::PacketType::CreateRoomType, strPacket);
}

void PacketHandler::WriteJoinedRoomRequest(const std::string& strInRoomName)
{
	JIGAPPacket::StringPacket strPacket;
	strPacket.set_str(strInRoomName);

	this->WritePacket(JIGAPPacket::PacketType::JoinedRoomRequestType, strPacket);
}

void PacketHandler::WriteJoinedRoomAnswer(const std::string& strInRoomName, bool bInSuccess)
{
	JIGAPPacket::JoinedRoomAnswerPacket joinedRoomAnswer;
	joinedRoomAnswer.set_roomname(strInRoomName);
	joinedRoomAnswer.set_joinedroomsuccess(bInSuccess);

	this->WritePacket(JIGAPPacket::PacketType::JoinedRoomAnswerType, joinedRoomAnswer);
}

void PacketHandler::WriteExitRoomRequest()
{
	JIGAPPacket::TypePacket packetType;
	packetType.set_type(JIGAPPacket::PacketType::ExitRoomRequestType);

	this->WritePacket(JIGAPPacket::PacketType::ExitRoomRequestType, packetType);
}

void PacketHandler::WriteExitRoomAnswer()
{
	JIGAPPacket::TypePacket packetType;
	packetType.set_type(JIGAPPacket::PacketType::ExitRoomAnswerType);

	this->WritePacket(JIGAPPacket::PacketType::ExitRoomAnswerType, packetType);
}

void PacketHandler::WriteRoomListRequest()
{
	JIGAPPacket::TypePacket packetType;
	packetType.set_type(JIGAPPacket::PacketType::RoomListRequestType);

	this->WritePacket(JIGAPPacket::PacketType::RoomListRequestType, packetType);
}

void PacketHandler::WriteRoomListInformation(unsigned int iInRoomCount)
{
	JIGAPPacket::RoomListAnswerPacket roomListAnswer;
	roomListAnswer.set_roomcount(iInRoomCount);
	
	this->WritePacket(JIGAPPacket::PacketType::RoomListAnswerType, roomListAnswer);
}

void PacketHandler::WriteRoomListElement(const std::map<std::string, Room*>& mInRoomList)
{
	for (auto Iter : mInRoomList)
	{
		JIGAPPacket::StringPacket strPaket;
		strPaket.set_str(Iter.first);

		this->WritePacket(JIGAPPacket::PacketType::ElementOfRoomListType, strPaket);
	}
}


void PacketHandler::WriteTotalPacketSize(unsigned int size)
{
	memcpy(lpOutputStream, (char*)&size, sizeof(unsigned int));
}

void PacketHandler::ClearWirteBuffer()
{
	memset(lpOutputStream, 0, iOStreamAllocSize);

	iOStreamWriteSize = sizeof(unsigned int);
	WriteTotalPacketSize(iOStreamWriteSize);
}
