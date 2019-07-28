#include "pch.h"
#include "JIGAPChatProcess.h"
#include "UserDataAdmin.h"
#include "ChatUserData.h"
#include "ChatQuery.h"

void JIGAPChatProcess::OnInitialize()
{
	lpUserAdmin = new UserDataAdmin<ChatUserData>();
	lpUserAdmin->InitializeAdmin(100000);

	lpQuery = new ChatQuery();
	lpQuery->InitializeQuery();
}

void JIGAPChatProcess::OnRelease()
{
	lpUserAdmin->ReleaseAdmin();
	SAFE_DELETE(lpUserAdmin);

	lpQuery->ReleaseQuery();
	SAFE_DELETE(lpQuery);
}

void JIGAPChatProcess::OnConnect(TCPSocket* lpInSocket)
{
	lpUserAdmin->AddUser(lpInSocket);
}

void JIGAPChatProcess::OnDisconnect(TCPSocket* lpInSocket)
{
	lpUserAdmin->DeleteUser(lpInSocket);
}

void JIGAPChatProcess::OnProcess(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler)
{
	PacketHeader header;
	lpHandler->NextParsingHeader(header);

	switch (header.ePacketType)
	{
	case JIGAPPacket::Type::eSingUpRequest:
		OnSingUpRequest(lpInTCPSocket, lpHandler, header);
		break;
	case JIGAPPacket::Type::eLoginRequest:
		OnLoginRequest(lpInTCPSocket, lpHandler, header);
		break;
	default:
		break;
	}
}

void JIGAPChatProcess::OnSingUpRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);

	if (find == nullptr)
	{
		DEBUG_LOG("유저를 찾을 수 없습니다.");
		return;
	}

	if (find->GetLogin() == false)
	{
		JIGAPPacket::SingUpRequest packet;
		lpHandler->NextParsingPacket(packet, header.iSize);


		
		//packet.userdata().id


		JIGAPPacket::UserData * data = packet.release_userdata();
		SAFE_DELETE(data);
	}
}

void JIGAPChatProcess::OnLoginRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader & header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);

	if (find == nullptr)
	{
		DEBUG_LOG("유저를 찾을 수 없습니다.");
		return;
	}

	if (find->GetLogin() == false)
	{
		JIGAPPacket::LoginRequest packet;
		lpHandler->NextParsingPacket(packet, header.iSize);



	}
}

