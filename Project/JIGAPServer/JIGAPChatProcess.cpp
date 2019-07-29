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
		JIGAPPacket::SingUpAnswer answer;
		JIGAPPacket::SingUpRequest packet;
		lpHandler->NextParsingPacket(packet, header.iSize);

		if (lpQuery->CheckUserDataToDB(packet.userdata().id()))
		{
			answer.set_success(false);
			lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);

			JIGAPPacket::UserData* lpUser = packet.release_userdata();
			SAFE_DELETE(lpUser);
			return;
		}

		if (!lpQuery->InsertUserDataToDB(packet.userdata().id(), packet.passward(), packet.userdata().name()))
		{
			answer.set_success(false);
			lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);
			
			JIGAPPacket::UserData* lpUser = packet.release_userdata();
			SAFE_DELETE(lpUser);
			return;
		}

		DEBUG_LOG("Insert User" << packet.userdata().id() << " " << packet.passward() << " " << packet.userdata().name());
		answer.set_success(true);
		lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);

		JIGAPPacket::UserData* lpUser = packet.release_userdata();
		SAFE_DELETE(lpUser);
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
		JIGAPPacket::LoginAnswer answer;
		JIGAPPacket::LoginRequest packet;
		lpHandler->NextParsingPacket(packet, header.iSize);

		TYPE_ROW row;
		if (lpQuery->FindUserDataToDB(packet.id(), row))
		{
			if (row["password"] == packet.passward())
			{
				JIGAPPacket::UserData* userdata = answer.mutable_userdata();
				answer.set_success(true);
				userdata->set_id(row["id"]);
				userdata->set_name(row["name"]);
				
				lpHandler->SerializePacket(JIGAPPacket::Type::eLoginAnswer, answer);

				userdata = answer.release_userdata();
				SAFE_DELETE(userdata);
				return;
			}
		}

		answer.set_success(false);
		lpHandler->SerializePacket(JIGAPPacket::Type::eLoginAnswer, answer);
	}
}

