#include "pch.h"
#include "PacketHandler.h"

#include "Room.h"

PacketHandler::PacketHandler()
	:iRecvStreamSize(MAXBUFFERSIZE), iSendStreamSize(MAXBUFFERSIZE),
	iRecvStreamPosition(0), iSendStreamPosition(0),
	lpRecvStream(static_cast<char*>(std::malloc(MAXBUFFERSIZE))),
	lpSendStream(static_cast<char*>(std::malloc(MAXBUFFERSIZE))),
	iSizeOfRecvPacket(0)
{
}

PacketHandler::~PacketHandler()
{
	delete[] lpRecvStream;
	delete[] lpSendStream;
}

void PacketHandler::ClearRecvStream()
{
	memset(lpRecvStream, 0, iRecvStreamSize);
	iRecvStreamPosition = sizeof(unsigned int);
}

void PacketHandler::SetPacketInRecvStream(char* lpInBuffer, int size)
{
	ClearRecvStream();
	iSizeOfRecvPacket = size;
	memcpy(lpRecvStream, lpInBuffer, iRecvStreamSize);
}

int PacketHandler::ParsingPacketSize(const char* lpInBuffer)
{
	unsigned int bufferSize = 0;
	memcpy((void*)& bufferSize, lpInBuffer, sizeof(unsigned int));
	return bufferSize;
}

void PacketHandler::ParsingPacketHeader(PacketHeader& inHeader)
{
	int headerSize = sizeof(PacketHeader);
	memcpy((void*)& inHeader, &lpRecvStream[iRecvStreamPosition], headerSize);
	iRecvStreamPosition += headerSize;
}

bool PacketHandler::SerializeHader(JIGAPPacket::PacketType eInType)
{
	PacketHeader header;
	header.packetType = eInType;
	header.size = 8;

	unsigned int headerSize = sizeof(header);
	memcpy(&lpSendStream[iSendStreamPosition], (char*)& header, headerSize);
	iSendStreamPosition += headerSize;
	SerializeTotalPacketSize(iSendStreamPosition);

	return true;
}

void PacketHandler::ClearSendStream()
{
	memset(lpSendStream, 0, iSendStreamSize);
	iSendStreamPosition = sizeof(unsigned int);
	SerializeTotalPacketSize(iSendStreamPosition);
}

void PacketHandler::SerializeTotalPacketSize(unsigned int iInSize)
{
	memcpy(lpSendStream, (void*)&iInSize, sizeof(unsigned int));
}

/*
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
*/
