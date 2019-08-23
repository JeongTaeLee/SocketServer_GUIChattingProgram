#include "pch.h"
#include "JIGAPChatProcess.h"
#include "JIGAPServer.h"
#include "UserDataAdmin.h"
#include "ChatRoom.h"
#include "ChatUserData.h"
#include "ChatRoomAdmin.h"
#include "ChatQuery.h"

const char* lobbyName = "Lobby";
const char* baseRoom01 = "Public01";
const char* baseRoom02 = "Public02";
const char* baseRoom03 = "Public03";
const char* baseRoom04 = "Public04";
const char* baseRoom05 = "Public05";


void JIGAPChatProcess::OnInitialize()
{
	lpUserAdmin = new UserDataAdmin<ChatUserData>();
	lpUserAdmin->InitializeAdmin(100000);

	lpChatRoomAdmin = new ChatRoomAdmin();
	lpChatRoomAdmin->CreateLobby(lobbyName)->SetBaseRoom();
	lpChatRoomAdmin->CreateRoom(baseRoom01)->SetBaseRoom();
	lpChatRoomAdmin->CreateRoom(baseRoom02)->SetBaseRoom();
	lpChatRoomAdmin->CreateRoom(baseRoom03)->SetBaseRoom();
	lpChatRoomAdmin->CreateRoom(baseRoom04)->SetBaseRoom();
	lpChatRoomAdmin->CreateRoom(baseRoom05)->SetBaseRoom();


	lpQuery = new ChatQuery();
	lpQuery->InitializeQuery();
}

void JIGAPChatProcess::OnRelease()
{
	lpUserAdmin->ReleaseAdmin();
	SAFE_DELETE(lpUserAdmin);

	lpChatRoomAdmin->Release();
	SAFE_DELETE(lpChatRoomAdmin);

	lpQuery->ReleaseQuery();
	SAFE_DELETE(lpQuery);
}

void JIGAPChatProcess::OnConnect(TCPSocket* lpInSocket)
{
	lpUserAdmin->AddUser(lpInSocket);
}

void JIGAPChatProcess::OnDisconnect(TCPSocket* lpInSocket)
{
	ChatUserData * data =lpUserAdmin->FindUser(lpInSocket);

	if (data)
		ExitRoom(lpInSocket);

	lpUserAdmin->DeleteUser(lpInSocket);
}

void JIGAPChatProcess::OnProcess(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler)
{
	PacketHeader header;
	lpHandler->NextParsingHeader(header);

	bool sendData = false;

	switch (header.ePacketType)
	{
	case JIGAPPacket::Type::eSingUpRequest:
		sendData = OnSingUpRequest(lpInTCPSocket, lpHandler, header);
		break;
	case JIGAPPacket::Type::eLoginRequest:
		sendData = OnLoginRequest(lpInTCPSocket, lpHandler, header);
		break;
	case JIGAPPacket::Type::eCreateRoomRequest:
		sendData = OnCreateRoomRequest(lpInTCPSocket, lpHandler, header);
		break;
	case JIGAPPacket::Type::eJoinRoomRequest:
		sendData = OnJoinRoomRequest(lpInTCPSocket, lpHandler, header);
		break;
	case JIGAPPacket::Type::eRoomListRequest:
		sendData = OnRoomListRequest(lpInTCPSocket, lpHandler, header);
		break;
	case JIGAPPacket::Type::eChatRequest:
		sendData = OnChatRequest(lpInTCPSocket, lpHandler, header);
		break;
	default:
		sendData = false;
		break;
	}

	if (!sendData)
		lpInTCPSocket->IOCPRecv();
}

bool JIGAPChatProcess::OnSingUpRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);

	if (find == nullptr)
		return false;

	if (find->GetLogin())
		return false;

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
	}
	else
	{
		answer.set_success(true);
		lpJIGAPServer->RegisterServerLog("'%s(id : %s)' client singup server", packet.userdata().name().c_str(), packet.userdata().id().c_str());
	}

	lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, answer);
	lpInTCPSocket->IOCPSend(lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());

	return true;
}

bool JIGAPChatProcess::OnLoginRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);

	if (find == nullptr)
		return false;

	if (find->GetLogin())
		return false;

	JIGAPPacket::LoginRequest packet;

	JIGAPPacket::LoginAnswer answer;
	JIGAPPacket::UserData* answerUserData = answer.mutable_userdata();

	lpHandler->NextParsingPacket(packet, header.iSize);

	TYPE_ROW row;

	if (lpUserAdmin->FindUserInId(packet.id()))
	{
		answer.set_success(false);
		answer.set_loginreason(JIGAPPacket::LoginFailedReason::eOverlapConnect);
	}

	else if (lpQuery->FindUserDataToDB(packet.id(), row) == false)
	{
		answer.set_success(false);
		answer.set_loginreason(JIGAPPacket::LoginFailedReason::eDontMatchId);
	}

	else if (row["password"] == packet.passward())
	{
		answer.set_success(true);
		
		answerUserData->set_id(row["id"]);
		answerUserData->set_name(row["name"]);

		lpJIGAPServer->RegisterServerLog("'%s(id : %s)' client login server", row["name"].c_str(), row["id"].c_str());;
	}

	else
	{
		answer.set_success(false);
		answer.set_loginreason(JIGAPPacket::LoginFailedReason::eDontMatchPw);
	}

	lpHandler->SerializePacket(JIGAPPacket::Type::eLoginAnswer, answer);
	lpInTCPSocket->IOCPSend(lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());

	answerUserData = answer.release_userdata();
	SAFE_DELETE(answerUserData);

	if (answer.success())
	{
		find->SetUserID(row["id"]);
		find->SetName(row["name"]);
		find->SetLogin(true);

		PutUserIntoRoom(lpHandler, find, lobbyName);
	}

	return true;
}

bool JIGAPChatProcess::OnJoinRoomRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);

	if (find->GetLogin() == false)
		return false;

	JIGAPPacket::JoinRoomRequest joinRoomRequest;
	lpHandler->NextParsingPacket(joinRoomRequest, header.iSize);

	JIGAPPacket::JoinRoomAnswer joinRoomAnswer;
	JIGAPPacket::RoomInfo * answerRoomInfo =  joinRoomAnswer.mutable_roominfo();

	if (find->GetCurrentRoom())
		ExitRoom(lpInTCPSocket);

	ChatRoom* lpFindRoom = lpChatRoomAdmin->FindRoom(joinRoomRequest.roominfo().roomname());

	if (lpFindRoom == nullptr)
	{
		joinRoomAnswer.set_success(false);
		answerRoomInfo->set_roomname("Null");
	}
	else
	{
		if (find->GetCurrentRoom() != lpFindRoom)
			lpFindRoom->AddUserInRoom(find);

		joinRoomAnswer.set_success(true);
		answerRoomInfo->set_roomname(lpFindRoom->GetRoomName());

	}
	
	lpHandler->SerializePacket(JIGAPPacket::Type::eJoinRoomAnswer, joinRoomAnswer);
	lpInTCPSocket->IOCPSend(lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());

	answerRoomInfo = joinRoomAnswer.release_roominfo();
	SAFE_DELETE(answerRoomInfo);

	return true;
}

bool JIGAPChatProcess::OnRoomListRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);
	
	if (find->GetLogin() == false)
		return false;
	
	if (find->GetCurrentRoom() == nullptr)
		return false;
	
	JIGAPPacket::EmptyPacket emptyPacket;
	lpHandler->NextParsingPacket(emptyPacket, header.iSize);

	if (emptyPacket.type() == JIGAPPacket::Type::eRoomListRequest)
	{
		JIGAPPacket::RoomListAnswer answer;

		answer.set_roomcount(lpChatRoomAdmin->GetRooms().size());
		lpHandler->SerializePacket(JIGAPPacket::Type::eRoomListAnswer, answer);

		lpChatRoomAdmin->SerialieRoomList(lpHandler);

	}

	lpInTCPSocket->IOCPSend(lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());

	return true;
}

bool JIGAPChatProcess::OnCreateRoomRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);

	if (find == nullptr)
		return false;

	if (!find->GetLogin())
		return false;

	JIGAPPacket::RoomInfo request;
	
	JIGAPPacket::CreateRoomAnswer answer;
	lpHandler->NextParsingPacket(request, header.iSize);

	JIGAPPacket::RoomInfo * answerInfo = answer.mutable_roominfo();

	ChatRoom* lpFindRoom = lpChatRoomAdmin->FindRoom(request.roomname());

	if (lpFindRoom != nullptr)
	{
		answer.set_success(false);
		answerInfo->set_roomname("None");
	}
	else
	{
		lpChatRoomAdmin->CreateRoom(request.roomname());

		answer.set_success(true);
		answerInfo->set_roomname(request.roomname());
	}

	lpHandler->SerializePacket(JIGAPPacket::Type::eCreateRoomAnswer, answer);
	lpInTCPSocket->IOCPSend(lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());

	answerInfo = answer.release_roominfo();
	SAFE_DELETE(answerInfo);

	return true;
}

bool JIGAPChatProcess::OnChatRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);
	auto findRoom = find->GetCurrentRoom();
	
	if (find == nullptr)
		return false;

	else if (find->GetLogin() == false)
		return false;

	else if (findRoom == nullptr)
		return false;

	JIGAPPacket::ChatRequest request;
	lpHandler->NextParsingPacket(request, header.iSize);

	JIGAPPacket::ChatData answer;
	JIGAPPacket::UserData* answerUserData = answer.mutable_userdata();

	answer.set_strmessage(request.strmessage());
	answerUserData->set_id(request.userdata().id());
	answerUserData->set_name(request.userdata().name());
	
	lpHandler->SerializePacket(JIGAPPacket::Type::eChatData, answer);

	findRoom->SendMessageInUsers(lpHandler);

	answerUserData = answer.release_userdata();
	SAFE_DELETE(answerUserData);

	return true;
}

void JIGAPChatProcess::ExitRoom(TCPSocket* lpInTCPSocket)
{
	auto findUser = lpUserAdmin->FindUser(lpInTCPSocket);
	if (findUser == nullptr)
		return;

	auto findRoom = findUser->GetCurrentRoom();
	if (findRoom == nullptr)
		return;

	findRoom->DeleteUser(findUser);

	if (findRoom->GetBaseRoom() == false)
	{
		if (findRoom->GetRoomUserCount() <= 0)
			lpChatRoomAdmin->DeleteRoom(findRoom->GetRoomName());
	}
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
		findRoom->AddUserInRoom(inLpChatUserData);
		
		JIGAPPacket::JoinRoomAnswer answer;
		JIGAPPacket::RoomInfo* info = answer.mutable_roominfo();

		answer.set_success(true);
		info->set_roomname(inStrRoomName);

		inLpHandler->ClearSerializeBuffer();
		inLpHandler->SerializePacket(JIGAPPacket::Type::eJoinRoomAnswer, answer);
		inLpChatUserData->GetTCPSocket()->IOCPSend(inLpHandler->GetSerializeBufferData(), inLpHandler->GetSerializeRealSize());
	}
}

