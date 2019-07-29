#include "pch.h"
#include "JIGAPChatProcess.h"
#include "JIGAPServer.h"
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

	if (find->GetLogin()) return;

	JIGAPPacket::SingUpRequest packet;
	JIGAPPacket::SingUpAnswer answer;
	lpHandler->NextParsingPacket(packet, header.iSize);

	if (packet.userdata().id().size() > 20
		|| packet.userdata().name().size() > 20
		|| packet.passward().size() > 20)
	{
		answer.set_success(false);
		answer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDontCondition);

		lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);
		return;
	}

	TYPE_ROW row;
	if (lpQuery->FindUserDataToDB(packet.userdata().id(), row))
	{
		if (row["name"] == packet.userdata().name())
		{
			answer.set_success(false);
			answer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDuplicateName);

			lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);

			return;
		}

		answer.set_success(false);
		answer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDuplicateId);

		lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);
		return;
	}


	if (!lpQuery->InsertUserDataToDB(packet.userdata().id(), packet.passward(), packet.userdata().name()))
	{
		answer.set_success(false);
		answer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDuplicateId);

		lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);
		return;
	}

	answer.set_success(true);
	lpJIGAPServer->RegisterServerLog("'%s(id : %s)' client singup server", packet.userdata().name().c_str(), packet.userdata().id().c_str());

	lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);

	return;
}

void JIGAPChatProcess::OnLoginRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);

	if (find == nullptr)
	{
		DEBUG_LOG("유저를 찾을 수 없습니다.");
		return;
	}

	if (find->GetLogin()) return;

	JIGAPPacket::LoginAnswer answer;
	JIGAPPacket::LoginRequest packet;
	lpHandler->NextParsingPacket(packet, header.iSize);

	TYPE_ROW row;
	if (lpQuery->FindUserDataToDB(packet.id(), row) == false)
	{
		answer.set_success(false);
		answer.set_loginreason(JIGAPPacket::LoginFailedReason::eDontMatchId);

		lpHandler->SerializePacket(JIGAPPacket::Type::eLoginAnswer, answer);
		return;
	}

	if (row["password"] == packet.passward())
	{
		JIGAPPacket::UserData* userdata = answer.mutable_userdata();

		answer.set_success(true);
		userdata->set_id(row["id"]);
		userdata->set_name(row["name"]);

		lpJIGAPServer->RegisterServerLog("'%s(id : %s)' client login server", row["name"].c_str(), row["id"].c_str());

		lpHandler->SerializePacket(JIGAPPacket::Type::eLoginAnswer, answer);

		userdata = answer.release_userdata();
		SAFE_DELETE(userdata);
		return;
	}
	else
	{
		answer.set_success(false);
		answer.set_loginreason(JIGAPPacket::LoginFailedReason::eDontMatchPw);

		lpHandler->SerializePacket(JIGAPPacket::Type::eLoginAnswer, answer);
		return;
	}

}

