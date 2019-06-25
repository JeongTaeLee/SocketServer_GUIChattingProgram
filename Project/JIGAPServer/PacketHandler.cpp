#include "pch.h"
#include "PacketHandler.h"


PacketHandler::PacketHandler()
	:iRecvStreamSize(MAXBUFFERSIZE), iSendStreamSize(MAXBUFFERSIZE),
	iRecvStreamPosition(0), iSendStreamPosition(0),
	lpRecvStream(static_cast<char*>(std::malloc(MAXBUFFERSIZE))),
	lpSendStream(static_cast<char*>(std::malloc(MAXBUFFERSIZE)))
{
}

PacketHandler::~PacketHandler()
{
	delete[] lpRecvStream;
	delete[] lpSendStream;
}

void PacketHandler::ClearSendPacket()
{
	ZeroMemory(&lpSendStream, sizeof(int) * iSendStreamSize);
	iSendStreamPosition = 4;
}

void PacketHandler::SetRecvPacket(const char* ch, int iSize)
{
	ZeroMemory(&lpRecvStream, sizeof(int) * iRecvStreamSize);
	memcpy(lpRecvStream, ch, iSize);
	iRecvStreamPosition = 4;
}

int PacketHandler::ParsingPacketSize(const char* buffer)
{
	int paketSize = 0;
	memcpy((void*)& paketSize, buffer, sizeof(paketSize));
	return paketSize;
}

void PacketHandler::ParsingPacketHeader(PacketHeader& inHeader)
{
	JIGAPPacket::Type type;
	memcpy(&type, &lpRecvStream[iRecvStreamPosition], sizeof(int));
	iRecvStreamPosition += sizeof(int);

	int size;
	memcpy(&size, &lpRecvStream[iRecvStreamPosition], sizeof(int));
	iRecvStreamPosition += sizeof(int);
	
	inHeader.packetType = type;
	inHeader.size = size;
}

void PacketHandler::SerializePacketSize(int iPacketSize)
{
	memcpy(&lpSendStream, (void*)& iPacketSize, sizeof(int));
}

