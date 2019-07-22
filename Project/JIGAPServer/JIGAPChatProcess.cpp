#include "pch.h"
#include "JIGAPChatProcess.h"
#include "ObjectPool.h"


void JIGAPChatProcess::OnInitialize()
{
}

void JIGAPChatProcess::OnRelease()
{
}

void JIGAPChatProcess::OnConnect(TCPSocket* lpInSocket)
{
}

void JIGAPChatProcess::OnDisconnect(TCPSocket* lpInSocket)
{
}

void JIGAPChatProcess::OnProcess(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler)
{
	PacketHeader header;
	lpHandler->NextParsingHeader(header);

	JIGAPPacket::LoginRequest loginRequest;
	lpHandler->NextParsingPacket(loginRequest, header.iSize);

	JIGAPPacket::LoginAnswer loginAnswer;
	loginAnswer.set_success(true);

	lpHandler->SerializePacket(JIGAPPacket::Type::eLoginAnswer, loginAnswer);
}

