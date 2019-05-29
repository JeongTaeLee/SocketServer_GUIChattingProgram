// JIGAPServer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "JIGAPServer.h"



JIGAPServer::JIGAPServer()
	:lpServSock(nullptr), 
	
	lpSerializeObject(new SerializeObject), 
	
	hSystemLogMutex(nullptr), 
	hClientDataMutex(nullptr), 
	hRoomsMutex(nullptr),

	hCompletionHandle(nullptr),
	
	bServerOn(false)
{
}

JIGAPServer::~JIGAPServer()
{
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
		RemoveClient(Iter.second->GetSocket());
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
		LPTCPSOCK lpClntData = lpServSock->Accept(); // 연결을 대기합니다.

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
		LPTCPSOCK lpClntSock = nullptr;
		LPIODATA lpIOData = nullptr;

		int iCheckIOResult = CheckIOCompletionSocket(lpClntSock, lpIOData);

		if (iCheckIOResult == 0) // 클라이언트가 종료를 요청했습니다.
		{
			
			RemoveClient(lpClntSock->GetSocket());
		}

		else if (iCheckIOResult == -1) // 서버가 종료되었습니다.
			break;

		else // 성공적으로 IO가 끝났습니다.
		{
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
		}
	}

	JIGAPPrintSystemLog("채팅 쓰레드가 비활성화 되었습니다.");
}

void JIGAPServer::OnLogin(LPTCPSOCK & lpClntSock)
{
	if (lpClntSock->GetIOMode() == E_IOMODE_RECV)
	{
		char name[MAXNAMESIZE];
		lpSerializeObject->DeserializeRecvBuffer(name, sizeof(name));

		bool loginSuccess = true;
		WaitForSingleObject(hClientDataMutex, INFINITE);
		for (auto Iter : mClientData)
		{
			if (Iter.second->GetMyUserName() == name)
				loginSuccess = false;
		}
		ReleaseMutex(hClientDataMutex);

		if (loginSuccess)
		{
			lpClntSock->SetUserName(name);
			
			lpSerializeObject->SerializeDataSendBuffer(answerLoginLiteral);
			lpSerializeObject->SerializeDataSendBuffer(true);

			JIGAPPrintSystemLog("%d 소켓이 닉네임 %s 로 로그인 했습니다.", lpClntSock->GetSocket(), lpClntSock->GetMyUserName().c_str());
		}
		else
		{
			lpSerializeObject->SerializeDataSendBuffer(answerLoginLiteral);
			lpSerializeObject->SerializeDataSendBuffer(false);
		
			JIGAPPrintSystemLog("%d 소켓의 로그인 요청이 거부되었습니다.");
		}

		lpClntSock->SetBufferData(lpSerializeObject->GetSendStreamBuffer(), lpSerializeObject->GetSendStreamSize());
		lpClntSock->IOCPSend();
	}
}

void JIGAPServer::OnRequestRoomList(LPTCPSOCK& lpClntSock)
{
	if (lpClntSock->GetIOMode() == E_IOMODE_RECV)
	{
		WaitForSingleObject(hRoomsMutex, INFINITE);
		
		int roomCount = mRooms.size();
		
		lpSerializeObject->SerializeDataSendBuffer(answerRoomListLiteral);
		lpSerializeObject->SerializeDataSendBuffer(roomCount);

		for (auto& Iter : mRooms)
		{
			char roomName[MAXROOMNAMESIZE] = { 0 };
			memcpy(roomName, Iter.first.c_str() , Iter.first.size());
			lpSerializeObject->SerializeDataSendBuffer(roomName, sizeof(roomName));
		}

		ReleaseMutex(hRoomsMutex);

		lpClntSock->SetBufferData(lpSerializeObject->GetSendStreamBuffer(), lpSerializeObject->GetSendStreamSize());
		lpClntSock->IOCPSend();
	}
}

void JIGAPServer::OnRequestCreateRoom(LPTCPSOCK& lpClntSock)
{
	if (lpClntSock->GetIOMode() == E_IOMODE_RECV)
	{
		char roomName[MAXROOMNAMESIZE] = { 0 };
		lpSerializeObject->DeserializeRecvBuffer(roomName, sizeof(roomName));
		lpSerializeObject->SerializeDataSendBuffer(answerCreateRoomLiteral);
		
		WaitForSingleObject(hRoomsMutex, INFINITE);

		auto find = mRooms.find(roomName);
		if (find != mRooms.end())
			lpSerializeObject->SerializeDataSendBuffer(false);
		else
		{
			lpSerializeObject->SerializeDataSendBuffer(true);
			
			Room* lpRoom = new Room;
			lpRoom->SetRoomName(roomName);
			mRooms.insert(std::make_pair(roomName, lpRoom));
			lpRoom->AddUser(lpClntSock);

			JIGAPPrintSystemLog("%s 방이 생성되었습니다.", lpRoom->GetRoomName().c_str());
		}

		ReleaseMutex(hRoomsMutex);

		lpClntSock->SetBufferData(lpSerializeObject->GetSendStreamBuffer(), lpSerializeObject->GetSendStreamSize());
		lpClntSock->IOCPSend();
	}
}

void JIGAPServer::OnRequestJoinedRoom(LPTCPSOCK & lpClntSock)
{
	if (lpClntSock->GetIOMode() == E_IOMODE_RECV)
	{
		char buffer[MAXROOMNAMESIZE];
		lpSerializeObject->DeserializeRecvBuffer(buffer, sizeof(buffer));
		lpSerializeObject->SerializeDataSendBuffer(answerJoinedRoomLiteral);


		WaitForSingleObject(hRoomsMutex, INFINITE);

		auto find = mRooms.find(buffer);
		if (find != mRooms.end())
		{
			find->second->AddUser(lpClntSock);
			lpSerializeObject->SerializeDataSendBuffer(true);
			lpSerializeObject->SerializeDataSendBuffer(find->first.c_str(), find->first.size());
		}
		else
			lpSerializeObject->SerializeDataSendBuffer(false);

		ReleaseMutex(hRoomsMutex);

		lpClntSock->SetBufferData(lpSerializeObject->GetSendStreamBuffer(), lpSerializeObject->GetSendStreamSize());
		lpClntSock->IOCPSend();
	}
}

void JIGAPServer::OnRequestExtiRoom(LPTCPSOCK& lpClntSock)
{
	if (lpClntSock->GetIOMode() == E_IOMODE_RECV)
	{
		lpSerializeObject->SerializeDataSendBuffer(answerExitRoomLiteral);

		RemoveClientToRoom(lpClntSock);

		lpClntSock->SetBufferData(lpSerializeObject->GetSendStreamBuffer(), lpSerializeObject->GetSendStreamSize());
		lpClntSock->IOCPSend();
	}
}

void JIGAPServer::OnRequestChatting(LPTCPSOCK & lpClntSock)
{
	if (lpClntSock->GetIOMode() == E_IOMODE_RECV)
	{
		char message[MAXSTRSIZE] = { 0 };
		lpSerializeObject->DeserializeRecvBuffer(message);

		char userName[MAXNAMESIZE] = { 0 };
		memcpy(userName, lpClntSock->GetMyUserName().c_str(), lpClntSock->GetMyUserName().size());
		lpSerializeObject->SerializeDataSendBuffer(anwserChattingLiteral);
		lpSerializeObject->SerializeDataSendBuffer(userName, sizeof(userName));
		lpSerializeObject->SerializeDataSendBuffer(message, sizeof(message));

		if (lpClntSock->GetRoom())
		{
			lpClntSock->GetRoom()->SendToAllUser(lpSerializeObject->GetSendStreamBuffer(), lpSerializeObject->GetSendStreamSize());
		}
	}
}

int JIGAPServer::CheckIOCompletionSocket(LPTCPSOCK & inSocket, LPIODATA & inIOData)
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

	return 1;
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

void JIGAPServer::RemoveClient(const SOCKET& hSock)
{
	auto find = mClientData.find(hSock);

	if (find == mClientData.end())
		return;


	LPTCPSOCK lpSock = find->second;
	

	RemoveClientToRoom(lpSock);
	


	JIGAPPrintSystemLog("소켓과 연결이 끊겼습니다. : %d", hSock);
	
	delete lpSock;

	WaitForSingleObject(hClientDataMutex, INFINITE);
	mClientData.erase(find);
	ReleaseMutex(hClientDataMutex);

}

void JIGAPServer::RemoveClientToRoom(LPTCPSOCK& lpSock)
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
