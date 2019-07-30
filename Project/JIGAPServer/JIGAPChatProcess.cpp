#include "pch.h"
#include "JIGAPChatProcess.h"
#include "JIGAPServer.h"
#include "UserDataAdmin.h"
#include "ChatRoom.h"
#include "ChatUserData.h"
#include "ChatRoomAdmin.h"
#include "ChatQuery.h"

void JIGAPChatProcess::OnInitialize()
{
	lpUserAdmin = new UserDataAdmin<ChatUserData>();
	lpUserAdmin->InitializeAdmin(100000);

	lpChatRoomAdmin = new ChatRoomAdmin();
	lpChatRoomAdmin->CreateLobby("Lobby");
	lpChatRoomAdmin->CreateRoom("Public01");
	lpChatRoomAdmin->CreateRoom("Public02");
	lpChatRoomAdmin->CreateRoom("Public03");
	lpChatRoomAdmin->CreateRoom("Public04");
	lpChatRoomAdmin->CreateRoom("Public05");


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
	case JIGAPPacket::Type::eCreateRoomRequest:
		break;
	case JIGAPPacket::Type::eJoinRoomRequest:
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

	TYPE_ROW row;
	TYPE_ROW nameRow;
	
	if (packet.userdata().id().size() > 20
		|| packet.userdata().name().size() > 20
		|| packet.passward().size() > 20)
	{
		answer.set_success(false);
		answer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDontCondition);
	}
	else if (lpQuery->FindUserDataToDB(packet.userdata().id(), row))
	{
		answer.set_success(false);
		answer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDuplicateId);
	}
	else if (lpQuery->CheckDuplicationUserName(packet.userdata().name()))
	{
		answer.set_success(false);
		answer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDuplicateName);
	}
	else if (lpQuery->InsertUserDataToDB(packet.userdata().id(), packet.passward(), packet.userdata().name()) == false)
	{
		answer.set_success(false);
		answer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDuplicateId);
		return;
	}
	else
	{
		answer.set_success(true);
		lpJIGAPServer->RegisterServerLog("'%s(id : %s)' client singup server", packet.userdata().name().c_str(), packet.userdata().id().c_str());
	}

	lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);
	lpInTCPSocket->IOCPSend(lpHandler, lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());
	return;
}

void JIGAPChatProcess::OnLoginRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);

	if (find == nullptr) return;
	if (find->GetLogin()) return;

	JIGAPPacket::LoginAnswer answer;
	JIGAPPacket::LoginRequest packet;
	JIGAPPacket::UserData* userdata = answer.mutable_userdata();

	lpHandler->NextParsingPacket(packet, header.iSize);

	TYPE_ROW row;
	if (lpQuery->FindUserDataToDB(packet.id(), row) == false)
	{
		answer.set_success(false);
		answer.set_loginreason(JIGAPPacket::LoginFailedReason::eDontMatchId);
	}
	else if (row["password"] == packet.passward())
	{
		answer.set_success(true);
		userdata->set_id(row["id"]);
		userdata->set_name(row["name"]);

		lpJIGAPServer->RegisterServerLog("'%s(id : %s)' client login server", row["name"].c_str(), row["id"].c_str());
}
	else
	{
		answer.set_success(false);
		answer.set_loginreason(JIGAPPacket::LoginFailedReason::eDontMatchPw);
	}

	userdata = answer.release_userdata();
	SAFE_DELETE(userdata);

	lpHandler->SerializePacket(JIGAPPacket::Type::eLoginAnswer, answer);
	lpInTCPSocket->IOCPSend(lpHandler, lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());
}

void JIGAPChatProcess::OnJoinRoomRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
}

void JIGAPChatProcess::PutUserIntoRoom(PacketHandler * inLpHandler, ChatUserData* inLpChatUserData, const std::string& inStrRoomName)
{
	if (inLpChatUserData->GetCurrentRoom())
	{
		ChatRoom * room = inLpChatUserData->GetCurrentRoom();
		room->DeleteUser(inLpChatUserData);
	}
	
	ChatRoom* findRoom = lpChatRoomAdmin->FindRoom(inStrRoomName);

	if (findRoom)
	{
		findRoom->AddUser(inLpChatUserData);


	}
}

