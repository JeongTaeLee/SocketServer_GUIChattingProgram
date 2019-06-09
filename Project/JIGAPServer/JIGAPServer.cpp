// JIGAPServer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "JIGAPServer.h"


JIGAPServer::JIGAPServer()
	:lpServSock(nullptr), 
	
	lpSerializeObject(new SerializeObject), 
	lpPacketHandler(new PacketHandler),
	
	hSystemLogMutex(nullptr), 
	hClientDataMutex(nullptr), 
	hRoomsMutex(nullptr),

	hCompletionHandle(nullptr),
	
	bServerOn(false)
{
}

JIGAPServer::~JIGAPServer()
{
	delete lpSerializeObject;
	delete lpPacketHandler;
}

HRESULT JIGAPServer::JIGAPInitializeServer()
{
	int iErrorCode = 0;

	lpServSock = new TCPSocket;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return E_FAIL;

	if ( (iErrorCode = lpServSock->IOCPSocket()) ) 
	{
		JIGAPPrintSystemLog("socket Error! Code : %d, Faild Create Socket", iErrorCode);
		return E_FAIL;
	}

	JIGAPPrintSystemLog("소켓을 생성했습니다!");

	if ( (iErrorCode = lpServSock->Bind(szIpAddr.c_str(), szPortAddr.c_str())) )
	{
		JIGAPPrintSystemLog("bind Error! Code : %d, Faild Bind Socket", iErrorCode);
		return E_FAIL;
	}

	JIGAPPrintSystemLog("소켓이 바인드 되었습니다.");

	if ( (iErrorCode = lpServSock->Listen(10)) )
	{
		JIGAPPrintSystemLog("listen Error! Code : %d", iErrorCode);
		return E_FAIL;
	}
	
	JIGAPPrintSystemLog("소켓이 연결 대기중입니다.");

	hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
	if (hCompletionHandle == nullptr)
	{
		JIGAPPrintSystemLog("CreateIOCompletionPort Error! Code : %d, Failed Create CompletionPort", iErrorCode);
		return E_FAIL;
	}
	
	JIGAPPrintSystemLog("CompletionPort 만들어졌습니다.");

	return S_OK;
}

void JIGAPServer::JIGAPReleaseServer()
{
	lpServSock->Closesocket();
	CloseHandle(hCompletionHandle);
	CloseHandle(hClientDataMutex);

	for (auto Iter : mClientData)
		RemoveClientInServer(Iter.second->GetSocket());
	mClientData.clear();

	delete lpServSock;
	lpServSock = nullptr;

	WSACleanup();
}

bool JIGAPServer::JIGAPServerOpen(std::string _szIpAddr, std::string _szPortAddr)
{
	szIpAddr = _szIpAddr;
	szPortAddr = _szPortAddr;

	bServerOn = true;

	if (FAILED(JIGAPServer::JIGAPInitializeServer()))
	{
		JIGAPPrintSystemLog("서버를 여는데 실패했습니다!");
		return false;
	}

	hSystemLogMutex = CreateMutex(0, FALSE, NULL);
	hClientDataMutex = CreateMutex(0, FALSE, NULL);
	hRoomsMutex = CreateMutex(0, FALSE, NULL);

	/*연결을 담당하는 Thread 입니다.*/
	connectThread = std::thread([&]() { JIGAPConnectThread(); });
	Sleep(10);
	/*Chatting을 담당하는 Thread 입니다.*/
	chattingThread = std::thread([&]() { JIGAPChattingThread(); });
	Sleep(10);

	JIGAPPrintSystemLog("서버가 열렸습니다!");
	return true;
}

void JIGAPServer::JIGAPServerClose()
{
	/*서버를 종료합니다.*/
	bServerOn = false;
	Sleep(10);

	JIGAPReleaseServer();

	/*Thread를 해제합니다.*/
	if (chattingThread.joinable())
		chattingThread.join();
	if (connectThread.joinable())
		connectThread.join();

	//ReleaseMutex(hSystemLogMutex);
	//ReleaseMutex(hClientDataMutex);
	//ReleaseMutex(hRoomsMutex);

	/*생성한 WInAPI Mutex를 해제합니다.*/
	CloseHandle(hSystemLogMutex);
	CloseHandle(hClientDataMutex);
	CloseHandle(hRoomsMutex);

	JIGAPPrintSystemLog("서버가 닫혔습니다!");
}

void JIGAPServer::JIGAPConnectThread()
{
	int iErrorCode = 0;

	JIGAPPrintSystemLog("연결 쓰레드가 활성화 되었습니다.");	

	while (true)
	{
		TCPSocket* lpClntData = lpServSock->Accept(); // 연결을 대기합니다.

		if (bServerOn == false) // 서버가 종료되었습니다.
			break;
		
		if (lpClntData == nullptr) 
		{
			JIGAPPrintSystemLog("클라이언트 소켓과 연결에 실패했습니다");
			continue;
		}
		
		if (lpClntData->ConnectionCompletionPort(hCompletionHandle) == NULL)
		{
			JIGAPPrintSystemLog("에러! 컴플렉션 포트에 연결하지 못했습니다 code : %d, socket : %d", WSAGetLastError(), lpClntData->GetSocket());

			lpClntData->Closesocket();
			delete lpClntData;
			continue;
		}

		JIGAPPrintSystemLog("클라이언트 소켓이 연결되었습니다 : %d", lpClntData->GetSocket());
		
		mClientData.insert(std::make_pair(lpClntData->GetSocket(), lpClntData));

		if ((iErrorCode = lpClntData->IOCPRecv()))
		{
			JIGAPPrintSystemLog("에러! IOCP 수신에 실패했습니다. code : %d, socket : %d", WSAGetLastError(), lpClntData->GetSocket());

			lpClntData->Closesocket();
			delete lpClntData;
		}
	}

	JIGAPPrintSystemLog("연결 쓰레드가 비활성화 되었습니다.");
}

void JIGAPServer::JIGAPChattingThread()
{
	JIGAPPrintSystemLog("채팅 쓰레드가 활성화 되었습니다.");

	while (true)
	{
		TCPSocket* lpClntSock = nullptr;
		TCPIOData* lpIOData = nullptr;

		int iCheckIOResult = CheckIOCompletionSocket(lpClntSock, lpIOData);

		if (iCheckIOResult == 0) // 클라이언트가 종료를 요청했습니다.
			RemoveClientInServer(lpClntSock->GetSocket());

		else if (iCheckIOResult == -1) // 서버가 종료되었습니다.
			break;

		else
		{
			if (lpClntSock->GetIOMode() == IOMODE::E_IOMODE_RECV)
				RecvProcessing(lpClntSock, iCheckIOResult);
			else
				lpClntSock->IOCPRecv();

			/*
			int literal = 0;
			
			lpSerializeObject->ClearSendStreamBuffer();
			lpSerializeObject->SetRecvStreamBuffer(lpClntSock->GetBufferData());
			lpSerializeObject->DeserializeRecvBuffer(literal);

			WaitForSingleObject(hClientDataMutex, INFINITE);
			switch (literal)
			{
			case requestLoginLiteral:
				OnLogin(lpClntSock);
				break;

			case requestRoomListLiteral:
				OnRequestRoomList(lpClntSock);
				break;

			case requestCreateRoomLiteral:
				OnRequestCreateRoom(lpClntSock);
				break;

			case requestJoinedRoomLiteral:
				OnRequestJoinedRoom(lpClntSock);
				break;

			case requestExitRoomLiteral:
				OnRequestExtiRoom(lpClntSock);
				break;

			case requestChattingLiteral:
				OnRequestChatting(lpClntSock);
				break;

			default:
				if (lpClntSock->GetIOMode() == IOMODE::E_IOMODE_SEND)
					lpClntSock->IOCPRecv();
				break;
			}
			ReleaseMutex(hClientDataMutex);

			lpSerializeObject->ClearRecvStreamBuffer();
			*/
		}
	}

	JIGAPPrintSystemLog("채팅 쓰레드가 비활성화 되었습니다.");
}

void JIGAPServer::RecvProcessing(TCPSocket * lpInClntSocket, int iInRecvByte)
{
	unsigned int packetSize = lpPacketHandler->ParsingPacketSize(lpInClntSocket->GetBufferData());
	JIGAPPrintSystemLog("패킷이 도착했습니다 패킷의 총 사이즈 : %d, 전달 받은 사이즈 : %d", packetSize, iInRecvByte);
	if (packetSize > iInRecvByte)
		return;

	lpPacketHandler->ClearSendStream();
	lpPacketHandler->SetPacketInRecvStream(lpInClntSocket->GetBufferData(), packetSize);

	PacketHeader packetHeader;
	lpPacketHandler->ParsingPacketHeader(packetHeader);

	switch (packetHeader.packetType)
	{
	case JIGAPPacket::PacketType::LoginRequestType:
		OnRequestLogin(lpInClntSocket, packetHeader.size);
		break;

	case JIGAPPacket::PacketType::RoomListRequestType:
		OnRequestRoomList(lpInClntSocket, packetHeader.size);
		break;

	case JIGAPPacket::PacketType::CreateRoomRequestType:
		OnRequestCreateRoom(lpInClntSocket, packetHeader.size);
		break;

	case JIGAPPacket::PacketType::ExitRoomRequestType:
		OnRequestExitRoom(lpInClntSocket, packetHeader.size);
		break;
	}
}

void JIGAPServer::OnRequestLogin(TCPSocket*& lpInClntData, unsigned int iInSize)
{
	JIGAPPacket::StringPacket requestPacket;
	lpPacketHandler->ParsingPacket(requestPacket, iInSize);

	bool bLoginSuccess = true;

	for (auto Iter : mClientData)
	{
		if (Iter.second->GetMyUserName() == requestPacket.str())
			bLoginSuccess = false;
	}

	JIGAPPacket::LoginAnswerPacket answerPacket;
	answerPacket.set_loginsuccess(bLoginSuccess);
	lpPacketHandler->SerializePacket(JIGAPPacket::LoginAnswerType, answerPacket);

	lpInClntData->IOCPSend(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
}

void JIGAPServer::OnRequestRoomList(TCPSocket*& lpInClntData, unsigned int iInSize)
{
	WaitForSingleObject(hRoomsMutex, INFINITE);
	
	JIGAPPacket::RoomListAnswerPacket roomListAnswerPacket;
	roomListAnswerPacket.set_roomcount(mRooms.size());
	lpPacketHandler->SerializePacket(JIGAPPacket::RoomListAnswerType, roomListAnswerPacket);

	for (auto Iter : mRooms)
	{
		JIGAPPacket::StringPacket roomNamePaket;
		roomNamePaket.set_str(Iter.first);
		lpPacketHandler->SerializePacket(JIGAPPacket::ElementOfRoomListType, roomNamePaket);
	}

	ReleaseMutex(hRoomsMutex);

	lpInClntData->IOCPSend(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
}

void JIGAPServer::OnRequestCreateRoom(TCPSocket*& lpInClntData, unsigned int iInSize)
{	
	JIGAPPacket::StringPacket roomNamePacket;
	lpPacketHandler->ParsingPacket(roomNamePacket, iInSize);

	WaitForSingleObject(hRoomsMutex, INFINITE);
	
	bool bCreateSuccess = true;

	auto find = mRooms.find(roomNamePacket.str());
	if (find != mRooms.end())
		bCreateSuccess = false;

	if (bCreateSuccess)
	{
		Room* room = new Room();
		room->SetRoomName(roomNamePacket.str());
		mRooms.insert(make_pair(roomNamePacket.str(), room));
		room->AddUser(lpInClntData);
	}
	ReleaseMutex(hRoomsMutex);

	JIGAPPacket::CreateRoomAnswerPacket joinedRoomAnswerPacket;
	joinedRoomAnswerPacket.set_createroomsuccess(bCreateSuccess);

	lpPacketHandler->SerializePacket(JIGAPPacket::CreateRoomAnswerType, joinedRoomAnswerPacket);

	lpInClntData->IOCPSend(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
}

void JIGAPServer::OnRequestJoinedRoom(TCPSocket*& lpInClntData, unsigned int iInSize)
{
	JIGAPPacket::StringPacket roomNamePacket;
	lpPacketHandler->ParsingPacket(roomNamePacket, iInSize);

	WaitForSingleObject(hRoomsMutex, INFINITE);
	
	auto find = mRooms.find(roomNamePacket.str());
	
	bool bJoinedSuccess = true;
	if (find == mRooms.end())
		bJoinedSuccess = false;

	if (bJoinedSuccess)
		find->second->AddUser(lpInClntData);
	ReleaseMutex(hRoomsMutex);
	
	JIGAPPacket::JoinedRoomAnswerPacket joinedAnswerPacket;
	joinedAnswerPacket.set_roomname(roomNamePacket.str());
	joinedAnswerPacket.set_joinedroomsuccess(bJoinedSuccess);
	

	lpPacketHandler->SerializePacket(JIGAPPacket::JoinedRoomAnswerType, joinedAnswerPacket);

	lpInClntData->IOCPSend(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
}

void JIGAPServer::OnRequestExitRoom(TCPSocket*& lpInClntData, unsigned int iInSize)
{
	if (lpInClntData->GetRoom())
		RemoveClientInRoom(lpInClntData);

	lpPacketHandler->SerializeHader(JIGAPPacket::PacketType::ExitRoomAnswerType);

	lpInClntData->IOCPSend(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
}

void JIGAPServer::OnRequestChatting(TCPSocket*& lpInClntData, unsigned int iInSize)
{
	JIGAPPacket::StringPacket msgRequestPacket;
	lpPacketHandler->ParsingPacket(msgRequestPacket, iInSize);
		
	JIGAPPacket::ChattingSpreadPacket spreadPacket;
	spreadPacket.set_sender(lpInClntData->GetMyUserName());
	spreadPacket.set_msg(msgRequestPacket.str());

	lpInClntData->GetRoom()->SendToAllUser(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
}



DWORD JIGAPServer::CheckIOCompletionSocket(TCPSocket* & inSocket, TCPIOData* & inIOData)
{
	DWORD dwByte = 0;

	GetQueuedCompletionStatus(hCompletionHandle,
		&dwByte,
		(PULONG_PTR)& inSocket,
		(LPOVERLAPPED*)& inIOData,
		INFINITE);

	if (dwByte == 0)
	{
		if (bServerOn == false)
			return -1;
		else
			return 0;
	}

	return dwByte;
}

void JIGAPServer::JIGAPPrintSystemLog(const char* fmt, ...)
{
	/*Mutex를 소유합니다. 해당핸들은 non-signaled 상태가 됩니다 반면 이미 핸들이 Non-signaled 상태이면 블록킹합니다. */
	WaitForSingleObject(hSystemLogMutex, INFINITE);
	char buf[512] = { 0 };
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	qSystemMsg.push(buf);
	
	/*Mutex 소유하지 않게 바꿔줍니다. 뮤텍스를 signaled 상태로 바꿉니다*/
	ReleaseMutex(hSystemLogMutex);	
}

std::string JIGAPServer::JIGAPGetSystemMsg()
{
	/*가장 오래된 메시지를 가져옵니다.*/
	WaitForSingleObject(hSystemLogMutex, INFINITE);
	std::string strSystemMessage = qSystemMsg.front();

	/*가장 오래된 메시지를 Queue에서 지웁니다.*/
	qSystemMsg.pop();
	ReleaseMutex(hSystemLogMutex);
	return strSystemMessage;
}

void JIGAPServer::RemoveClientInServer(const SOCKET& hSock)
{
	auto find = mClientData.find(hSock);

	if (find == mClientData.end())
		return;


	TCPSocket* lpSock = find->second;
	
	if(lpSock->GetRoom())
		RemoveClientInRoom(lpSock);

	JIGAPPrintSystemLog("소켓과 연결이 끊겼습니다. : %d", hSock);
	
	delete lpSock;

	WaitForSingleObject(hClientDataMutex, INFINITE);
	mClientData.erase(find);
	ReleaseMutex(hClientDataMutex);

}

void JIGAPServer::RemoveClientInRoom(TCPSocket*& lpSock)
{
	if (!lpSock) return;

	Room* lpRoom = lpSock->GetRoom();
	if (!lpRoom) return;

	if (lpRoom)
	{
		lpRoom->DeleteUser(lpSock);
		WaitForSingleObject(hRoomsMutex, INFINITE);
		if (lpRoom->GetUserCount() <= 0)
		{
			auto find = mRooms.find(lpRoom->GetRoomName());

			if (find != mRooms.end())
			{
				JIGAPPrintSystemLog("%s 방이 삭제되었습니다.", lpRoom->GetRoomName().c_str());
				mRooms.erase(find);
			}
			delete lpRoom;
		}

		ReleaseMutex(hRoomsMutex);
	}
}
