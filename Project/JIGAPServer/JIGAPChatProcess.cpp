#include "pch.h"
#include "JIGAPChatProcess.h"
#include "JIGAPServer.h"
#include "UserDataAdmin.h"
#include "ChatRoom.h"
#include "ChatUserData.h"
#include "ChatRoomAdmin.h"
#include "ChatQuery.h"

#include "LogManager.h"


const char* lobbyName = "Lobby";
const char* baseRoom01 = "Public01";
const char* baseRoom02 = "Public02";
const char* baseRoom03 = "Public03";
const char* baseRoom04 = "Public04";
const char* baseRoom05 = "Public05";


bool JIGAPChatProcess::OnInitialize()
{
	try
	{
		lpUserAdmin = new UserDataAdmin<ChatUserData>();
		lpUserAdmin->InitializeAdmin(100000);

		lpChatRoomAdmin = new ChatRoomAdmin();
		lpChatRoomAdmin->Initialize(100000);

		lpQuery = new ChatQuery();
		lpQuery->InitializeQuery();
	}
	catch(CustomException & ex)
	{
		LOGMANAGER.LogError(ex.WhatPath().c_str(), ex.WhatLine(), ex.what());
		return false;
	}
	catch (std::exception& ex)
	{
		LOGMANAGER.LogError(__FILEW__, __LINE__, ex.what());
		return false;
	}

	lpChatRoomAdmin->CreateLobby(lobbyName);
	lpChatRoomAdmin->CreateRoom(baseRoom01, true);
	lpChatRoomAdmin->CreateRoom(baseRoom02, true);
	lpChatRoomAdmin->CreateRoom(baseRoom03, true);
	lpChatRoomAdmin->CreateRoom(baseRoom04, true);
	lpChatRoomAdmin->CreateRoom(baseRoom05, true);

	return true;
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
	ChatUserData * data = lpUserAdmin->AddUser(lpInSocket);
	LOGMANAGER.Log("User(%d) connect in server.", data->GetTCPSocket()->GetSocket());
}

void JIGAPChatProcess::OnDisconnect(TCPSocket* lpInSocket)
{
	ChatUserData * data =lpUserAdmin->FindUser(lpInSocket);

	if (data)
		ExitRoom(lpInSocket);

	LOGMANAGER.Log("User(%d) disconnect in server.", data->GetTCPSocket()->GetSocket());
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

	JIGAPPacket::SingUpRequest singUpRequest;
	JIGAPPacket::SingUpAnswer singUpAnswer;
	lpHandler->NextParsingPacket(singUpRequest, header.iSize);
	
	LOGMANAGER.Log("User(%s) start sign up to server", singUpRequest.userdata().name());

	TYPE_ROW row;
	TYPE_ROW nameRow;
	
	if (singUpRequest.userdata().id().size() > 20
		|| singUpRequest.userdata().name().size() > 20
		|| singUpRequest.passward().size() > 20)
	{
		singUpAnswer.set_success(false);
		singUpAnswer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDontCondition);
	}
	else if (lpQuery->FindUserDataInDB(singUpRequest.userdata().id(), row))
	{
		singUpAnswer.set_success(false);
		singUpAnswer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDuplicateId);
	}
	else if (lpQuery->CheckDuplicationUserName(singUpRequest.userdata().name()))
	{
		singUpAnswer.set_success(false);
		singUpAnswer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDuplicateName);
	}
	else if (lpQuery->InsertUserDataInDB(singUpRequest.userdata().id(), singUpRequest.passward(), singUpRequest.userdata().name()) == false)
	{
		singUpAnswer.set_success(false);
		singUpAnswer.set_singupreason(JIGAPPacket::SingUpFailedReason::eDuplicateId);
	}
	else
		singUpAnswer.set_success(true);

	if (singUpAnswer.success())
		LOGMANAGER.Log("User(%s) sign up successed to server", singUpRequest.userdata().name().c_str());
	else
		LOGMANAGER.Log("User(%s) sign up failed to server", singUpRequest.userdata().name().c_str());

	lpHandler->SerializePacket(JIGAPPacket::eSingUpAnswer, singUpAnswer);
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

	JIGAPPacket::LoginRequest loginRequest;
	lpHandler->NextParsingPacket(loginRequest, header.iSize);
	
	JIGAPPacket::LoginAnswer loginAnswer;
	JIGAPPacket::UserData* answerUserData = loginAnswer.mutable_userdata();
	
	LOGMANAGER.Log("Id(%s) start login to server", loginRequest.id().c_str());

	TYPE_ROW row;

	if (lpUserAdmin->FindUserInId(loginRequest.id()))
	{
		loginAnswer.set_success(false);
		loginAnswer.set_loginreason(JIGAPPacket::LoginFailedReason::eOverlapConnect);
	}

	else if (lpQuery->FindUserDataInDB(loginRequest.id(), row) == false)
	{
		loginAnswer.set_success(false);
		loginAnswer.set_loginreason(JIGAPPacket::LoginFailedReason::eDontMatchId);
	}
	else if (row["password"] == loginRequest.passward())
	{
		loginAnswer.set_success(true);
		
		answerUserData->set_id(row["id"]);
		answerUserData->set_name(row["name"]);
	}
	else
	{
		loginAnswer.set_success(false);
		loginAnswer.set_loginreason(JIGAPPacket::LoginFailedReason::eDontMatchPw);
	}

	lpHandler->SerializePacket(JIGAPPacket::Type::eLoginAnswer, loginAnswer);
	lpInTCPSocket->IOCPSend(lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());

	if (loginAnswer.success())
	{
		find->SetUserID(row["id"]);
		find->SetName(row["name"]);
		find->SetLogin(true);

		PutUserIntoRoom(lpHandler, find, lobbyName);
		
		LOGMANAGER.Log("Id(%s) login successed to server", loginRequest.id().c_str());
	}
	else
		LOGMANAGER.Log("Id(%s) login failed to server", loginRequest.id().c_str());

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
	
	LOGMANAGER.Log("Id(%s) start join to Room(%s)", find->GetUserID().c_str(), joinRoomRequest.roominfo().roomname().c_str());

	if (find->GetCurrentRoom())
		ExitRoom(lpInTCPSocket);

	ChatRoom* findRoom = lpChatRoomAdmin->FindRoom(joinRoomRequest.roominfo().roomname());

	if (findRoom == nullptr)
	{
		joinRoomAnswer.set_success(false);
		answerRoomInfo->set_roomname("Null");

		LOGMANAGER.Log("Id(%s) join failed to Room(%s)", find->GetUserID().c_str(), joinRoomRequest.roominfo().roomname());
	}
	else
	{
		if (find->GetCurrentRoom() != findRoom)
			findRoom->AddUserInRoom(find);

		joinRoomAnswer.set_success(true);
		answerRoomInfo->set_roomname(findRoom->GetRoomName());

		LOGMANAGER.Log("Id(%s) join success to Room(%s)", find->GetUserID().c_str(), answerRoomInfo->roomname().c_str());
	}
	
	lpHandler->SerializePacket(JIGAPPacket::Type::eJoinRoomAnswer, joinRoomAnswer);
	lpInTCPSocket->IOCPSend(lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());

	return true;
}

bool JIGAPChatProcess::OnRoomListRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header)
{
	auto find = lpUserAdmin->FindUser(lpInTCPSocket);
	
	if (find->GetLogin() == false)
		return false;
	
	if (find->GetCurrentRoom() == nullptr)
		return false;

	LOGMANAGER.Log("Id(%s) start Send RoomList", find->GetUserID().c_str());
	
	JIGAPPacket::EmptyPacket RoomListRequest;
	lpHandler->NextParsingPacket(RoomListRequest, header.iSize);

	if (RoomListRequest.type() == JIGAPPacket::Type::eRoomListRequest)
		lpChatRoomAdmin->SerialieRoomList(lpHandler);

	LOGMANAGER.Log("Id(%s) success Send RoomList", find->GetUserID().c_str());

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

	JIGAPPacket::RoomInfo createRoomRequest;
	lpHandler->NextParsingPacket(createRoomRequest, header.iSize);

	JIGAPPacket::CreateRoomAnswer createRoomAnswer;
	JIGAPPacket::RoomInfo * answerRoomInfo = createRoomAnswer.mutable_roominfo();

	LOGMANAGER.Log("Id(%s) start create room(%s)", find->GetUserID().c_str(), createRoomRequest.roomname().c_str());

	ChatRoom* findRoom = lpChatRoomAdmin->FindRoom(createRoomRequest.roomname());

	if (findRoom != nullptr)
	{
		createRoomAnswer.set_success(false);
		answerRoomInfo->set_roomname("None");

		LOGMANAGER.Log("Id(%s) failed create room(%s)", find->GetUserID().c_str(), createRoomRequest.roomname().c_str());
	}
	else
	{
		lpChatRoomAdmin->CreateRoom(createRoomRequest.roomname());

		createRoomAnswer.set_success(true);
		answerRoomInfo->set_roomname(createRoomRequest.roomname());

		LOGMANAGER.Log("Id(%s) success create room(%s)", find->GetUserID().c_str(), createRoomRequest.roomname().c_str());
	}

	lpHandler->SerializePacket(JIGAPPacket::Type::eCreateRoomAnswer, createRoomAnswer);
	lpInTCPSocket->IOCPSend(lpHandler->GetSerializeBufferData(), lpHandler->GetSerializeRealSize());

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

