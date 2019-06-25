// JIGAPServer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "JIGAPServer.h"

JIGAPServer::JIGAPServer()
	:lpServSock(nullptr), 
	
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
	if (bServerOn)
		return false;

	szIpAddr	= _szIpAddr;
	szPortAddr	= _szPortAddr;

	bServerOn	= true;

	if (FAILED(JIGAPServer::JIGAPInitializeServer()))
	{
		JIGAPPrintSystemLog("서버를 여는데 실패했습니다!");
		return false;
	}

	hSystemLogMutex		= CreateMutex(0, FALSE, NULL);
	hClientDataMutex	= CreateMutex(0, FALSE, NULL);
	hRoomsMutex			= CreateMutex(0, FALSE, NULL);

	connectThread	= std::thread([&]() { JIGAPConnectThread(); });  Sleep(10);
	ioThread		= std::thread([&]() { JIGAPIOThread(); });

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
	if (ioThread.joinable())
		ioThread.join();
	if (connectThread.joinable())
		connectThread.join();

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
		UserTCPSocket * lpClntSocket = lpServSock->Accept<UserTCPSocket>(); // 연결을 대기합니다.

		if (bServerOn == false) // 서버가 종료되었습니다.
			break;
		
		if (lpClntSocket == nullptr) 
		{
			JIGAPPrintSystemLog("클라이언트 소켓과 연결에 실패했습니다");
			continue;
		}
		
		HANDLE hHandle = CreateIoCompletionPort((HANDLE)lpClntSocket->GetSocket(), hCompletionHandle, (ULONG_PTR)lpClntSocket, NULL);
		if (hHandle == NULL)
		{
			JIGAPPrintSystemLog("에러! 컴플렉션 포트에 연결하지 못했습니다 code : %d, socket : %d", WSAGetLastError(), lpClntSocket->GetSocket());

			lpClntSocket->Closesocket();
			delete lpClntSocket;
			continue;
		}

		JIGAPPrintSystemLog("클라이언트 소켓이 연결되었습니다 : %d", lpClntSocket->GetSocket());
		
		mClientData.insert(std::make_pair(lpClntSocket->GetSocket(), lpClntSocket));

		if ((iErrorCode = lpClntSocket->IOCPRecv()))
		{
			JIGAPPrintSystemLog("에러! IOCP 수신에 실패했습니다. code : %d, socket : %d", WSAGetLastError(), lpClntSocket->GetSocket());

			lpClntSocket->Closesocket();
			delete lpClntSocket;
		}
	}

	JIGAPPrintSystemLog("연결 쓰레드가 비활성화 되었습니다.");
}

void JIGAPServer::JIGAPIOThread()
{
	JIGAPPrintSystemLog("채팅 쓰레드가 활성화 되었습니다.");

	while (true)
	{
		UserTCPSocket * lpClntSock	= nullptr;
		TCPIOData* lpIOData			= nullptr;

		int iCheckIOResult = CheckIOCompletionSocket(lpClntSock, lpIOData);

		if (iCheckIOResult == 0) // 클라이언트가 종료를 요청했습니다.
		{
			RemoveClientInServer(lpClntSock->GetSocket());
		}

		else if (iCheckIOResult == -1) // 서버가 종료되었습니다.
			break;
		else
		{

		}
	}

	JIGAPPrintSystemLog("채팅 쓰레드가 비활성화 되었습니다.");
}

DWORD JIGAPServer::CheckIOCompletionSocket(UserTCPSocket* & inSocket, TCPIOData* & inIOData)
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

void JIGAPServer::RemoveClientInServer(const SOCKET& hSock)
{
	auto find = mClientData.find(hSock);

	if (find == mClientData.end())
		return;


	UserTCPSocket* lpSock = find->second;

	if (lpSock->GetRoom())
		RemoveClientInRoom(lpSock);

	JIGAPPrintSystemLog("소켓과 연결이 끊겼습니다. : %d", hSock);

	delete lpSock;

	WaitForSingleObject(hClientDataMutex, INFINITE);

	mClientData.erase(find);
	
	ReleaseMutex(hClientDataMutex);

}

void JIGAPServer::RemoveClientInRoom(UserTCPSocket* lpSock)
{
	if (!lpSock) return;

	Room* lpRoom = lpSock->GetRoom();
	if (!lpRoom) return;

	if (lpRoom)
	{
		lpRoom->UnRegisterUser(lpSock);

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

void JIGAPServer::RecvProcess()
{
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

