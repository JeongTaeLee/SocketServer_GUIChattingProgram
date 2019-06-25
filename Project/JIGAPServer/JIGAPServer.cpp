// JIGAPServer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "JIGAPServer.h"
#include "Room.h"

JIGAPServer::JIGAPServer()
	:lpServSock(nullptr), 
	
	lpPacketHandler(new PacketHandler),
	
	hSystemLogMutex(nullptr), 
	hUsersMutex(nullptr), 
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
	CloseHandle(hUsersMutex);

	for (auto Iter : mUsers)
		RemoveClientInServer(Iter.second->GetSocket());
	mUsers.clear();

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
	hUsersMutex	= CreateMutex(0, FALSE, NULL);
	hRoomsMutex			= CreateMutex(0, FALSE, NULL);

	connectThread	= std::thread([&]() { JIGAPConnectThread(); });  Sleep(10);
	recvThread		= std::thread([&]() { JIGAPIOThread(); });

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
	if (recvThread.joinable())
		recvThread.join();
	if (connectThread.joinable())
		connectThread.join();

	/*생성한 WInAPI Mutex를 해제합니다.*/
	CloseHandle(hSystemLogMutex);
	CloseHandle(hUsersMutex);
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
		
		mUsers.insert(std::make_pair(lpClntSocket->GetSocket(), lpClntSocket));

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
		UserTCPSocket* lpClntSock = nullptr;
		TCPIOData* lpIOData = nullptr;

		DWORD dwByte = 0;
		GetQueuedCompletionStatus(hCompletionHandle, &dwByte, (PULONG_PTR)& lpClntSock, (LPOVERLAPPED*)& lpIOData,
			INFINITE);

		if (dwByte == 0) RemoveClientInServer(lpClntSock->GetSocket());

		else if (dwByte == -1) break;

		else
		{
			int dataSize = lpPacketHandler->ParsingPacketSize(lpClntSock->GetBufferData());

			if (dataSize <= dwByte)
			{
				lpPacketHandler->SetRecvPacket(lpClntSock->GetBufferData(), dataSize);

				RecvProcess();
			}
		}
	}

	JIGAPPrintSystemLog("채팅 쓰레드가 비활성화 되었습니다.");
}

void JIGAPServer::RecvProcess()
{
}

void JIGAPServer::WorkProcess()
{
}

void JIGAPServer::RemoveClientInServer(const SOCKET& hSock)
{
	if (auto find = mUsers.find(hSock); find == mUsers.end())
	{
		if (auto lpSock = find->second; lpSock != nullptr)
		{
			if (lpSock->GetRoom())
				RemoveClientInRoom(lpSock);

			delete lpSock;
		}
	
		WaitForSingleObject(hUsersMutex, INFINITE);
		mUsers.erase(find);
		ReleaseMutex(hUsersMutex);
	}

	JIGAPPrintSystemLog("소켓과 연결이 끊겼습니다. : %d", hSock);
}

void JIGAPServer::RemoveClientInRoom(UserTCPSocket* lpSock)
{
	if (!lpSock) return;

	if (auto lpRoom = lpSock->GetRoom(); lpRoom != nullptr)
	{
		lpRoom->UnRegisterUser(lpSock);

		if (lpRoom->GetUserCount() <= 0)
		{
			WaitForSingleObject(hRoomsMutex, INFINITE);

			auto find = mRooms.find(lpRoom->GetRoomName());

			if (find != mRooms.end())
			{
				JIGAPPrintSystemLog("%s 방이 삭제되었습니다.", lpRoom->GetRoomName().c_str());
				mRooms.erase(find);
			}
			delete lpRoom;
			
			ReleaseMutex(hRoomsMutex);
		}
	}
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

