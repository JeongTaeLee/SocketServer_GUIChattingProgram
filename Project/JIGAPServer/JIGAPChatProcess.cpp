#include "pch.h"
#include "JIGAPChatProcess.h"
#include "UserDataAdmin.h"
#include "ChatUserData.h"


void JIGAPChatProcess::OnInitialize()
{
	lpUserAdmin = new UserDataAdmin<ChatUserData>();
	lpUserAdmin->InitializeAdmin(100000);
}

void JIGAPChatProcess::OnRelease()
{
	lpUserAdmin->ReleaseAdmin();
	SAFE_DELETE(lpUserAdmin);
}

void JIGAPChatProcess::OnConnect(TCPSocket* lpInSocket)
{
	lpUserAdmin->AddUser(lpInSocket);
}

void JIGAPChatProcess::OnDisconnect(TCPSocket* lpInSocket)
{
}

void JIGAPChatProcess::OnProcess(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler)
{
	PacketHeader header;
	lpHandler->NextParsingHeader(header);

	switch (header.ePacketType)
	{
	case JIGAPPacket::Type::eLoginRequest:
		break;
	default:
		break;
	}
}

void JIGAPChatProcess::OnLoginRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);

	if (find == nullptr)
	{
		DEBUG_LOG("JIGAPChatProcess / OnLoginRequest에서 유저를 찾을 수 없습니다.");
		return;
	}

	if (find->GetLogin() == false)
	{

	}
}

