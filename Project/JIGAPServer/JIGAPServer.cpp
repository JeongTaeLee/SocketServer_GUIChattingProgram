#include "pch.h"
#include "JIGAPServer.h"
#include "JIGAPChatProcess.h"

bool JIGAPServer::CreateServerSocket()
{
	int errorCode = 0;

	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	lpServerSocket = new TCPSocket();
	

	if ((errorCode = lpServerSocket->IOCPSocket()) != 0)
	{
		RegisterServerLog("서버 소켓의 생성을 실패 했습니다. (ErrorCode : %d)", errorCode);
		delete lpServerSocket;
		return false;
	}

	RegisterServerLog("서버 소켓이 생성되었습니다.");

	if ((errorCode = lpServerSocket->Bind(serverData.GetIpAdress().c_str(), serverData.GetPortAddress().c_str())) != 0)
	{
		RegisterServerLog("서버 소켓의 할당을 실패 했습니다. (ErrorCode : %d)", errorCode);
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	RegisterServerLog("서버 소켓을 할당했습니다");

	hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if (hCompletionHandle == nullptr)
	{
		RegisterServerLog("서버 소켓의 CompletionPort를 생성하지 못했습니다.");
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	RegisterServerLog("서버 소켓의 CompletionPort를 생성했습니다");

	if ((errorCode = lpServerSocket->Listen(100)) != 0)
	{
		RegisterServerLog("서버 소켓을 연결 대기 모드로 변경하지 못했습니다. (ErrorCode : %d)", errorCode);
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	RegisterServerLog("서버 소켓을 대기 모드로 변경했습니다");

	return true;
}

bool JIGAPServer::ServerInitialize(const std::string& inIpAddress, const std::string& inPortAddress)
{
	lpServerProcess = new JIGAPChatProcess();
	lpServerProcess->OnInitialize();
	
	serverData.SetServerData(inIpAddress, inPortAddress);
	
	lpPacketHandler = new PacketHandler();
	
	if (CreateServerSocket() == false)
	{	
		RegisterServerLog("서버의 초기화를 실패했습니다. 서버가 실행되지 못했습니다.");
		ServerRelease();
		return false;
	}
	else
		RegisterServerLog("서버가 실행되었습니다.");
	
	hConnectThread = std::thread([&]() { OnConnectTask(); });
	Sleep(100);
	hRecvThread = std::thread([&]() { OnRecvPacketTask(); });

	return true;
}

void JIGAPServer::ServerRelease()
{
	bServerOn = false;
	
	lpServerProcess->OnRelease();
	SAFE_DELETE(lpServerProcess);
	SAFE_DELETE(lpPacketHandler);
	
	if (lpServerSocket)
	{
		lpServerSocket->Closesocket();
		CloseHandle(hCompletionHandle);
		delete lpServerSocket;
	}
	
	if (hConnectThread.joinable())
		hConnectThread.join();
	RegisterServerLog("ConnectThread가 종료되었습니다");
	Sleep(100);
	if (hRecvThread.joinable())
		hRecvThread.join();
	RegisterServerLog("RecvThread가 종료되었습니다");
	RegisterServerLog("서버가 종료되었습니다.");
	
	lpServerSocket		= nullptr;
	hCompletionHandle	= nullptr;
	lpPacketHandler		= nullptr;
	lpServerProcess		= nullptr;
	lpLogFunc			= nullptr;
}


bool JIGAPServer::StartServer(const std::string& inIpAddress, const std::string& inPortAddress)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(1000);

	bServerOn = ServerInitialize(inIpAddress, inPortAddress);
	return bServerOn;
}

void JIGAPServer::CloseServer()
{
	ServerRelease();
}

void JIGAPServer::OnConnectTask()
{
	RegisterServerLog("Connect Thread를 실행했습니다.");

	while (true)
	{
		TCPSocket * acceptSocket = lpServerSocket->Accept<TCPSocket>();

		if (bServerOn == false)
			break;

		if (acceptSocket)
		{
			HANDLE hHandle = CreateIoCompletionPort((HANDLE)acceptSocket->GetSocket(), hCompletionHandle, (ULONG_PTR)acceptSocket, NULL);
		
			if (hCompletionHandle == nullptr)
			{	
				RegisterServerLog("JIGAPServer.cpp  / CompletionPort의 연결의 심각한 문제가 생겼습니다. (SOCKET : %d)", acceptSocket->GetSocket());
				acceptSocket->Closesocket();
				SAFE_DELETE(acceptSocket);
				continue;
			}
	
			int iError = 0;
			if ((iError = acceptSocket->IOCPRecv()) != 0)
			{
				RegisterServerLog("JIGAPServer.cpp / 연결한 소켓을 수신상태로 변경하지 못했습니다 (ErrorCode : %d) ", iError);
				continue;
			}

			RegisterServerLog("클라이언트 소켓이 연결되었습니다 (SOCKET : %d)", acceptSocket->GetSocket());

			if (lpServerProcess)
				lpServerProcess->OnConnect(acceptSocket);
		}
		else
		{
			RegisterServerLog("JIGAPServer.cpp / 소켓 연결의 심각한 문제가 생겼습니다.");
			continue;
		}
	}
}

void JIGAPServer::OnRecvPacketTask()
{
	RegisterServerLog("Recv Thread를 실행했습니다");

	while (true)
	{
		DWORD iRecvByte = 0;
		
		TCPSocket* lpTCPSocket = nullptr;
		TCPIOData* lpTCPIoData = nullptr;

		GetQueuedCompletionStatus(hCompletionHandle, &iRecvByte, (PULONG_PTR)& lpTCPSocket,
			(LPOVERLAPPED*)& lpTCPIoData, INFINITE);
		//{
		//	RegisterServerLog("JIGAPServer.cpp / 패킷을 받는 도중 심각한 문제가 생겼습니다.");
		//	continue;
		//}

		if (bServerOn == false)
			break;
		
		if (iRecvByte == 0)
		{
			lpServerProcess->OnDisconnect(lpTCPSocket);
			closesocket(lpTCPSocket->GetSocket());
			RegisterServerLog("클라이언트 소켓이 연결해제되었습니다(SOCKET : %d)", lpTCPSocket->GetSocket());
			SAFE_DELETE(lpTCPSocket);
		}
		else
		{
			int iRealRecvSize = lpPacketHandler->ParsingBufferSize(lpTCPSocket->GetBufferData());
			lpPacketHandler->ClearParsingBuffer(lpTCPSocket->GetBufferData(), iRealRecvSize);
			lpPacketHandler->ClearSerializeBuffer();

			lpServerProcess->OnProcess(lpTCPSocket, lpPacketHandler);

			lpTCPSocket->IOCPSend(lpPacketHandler->GetSerializeBufferData(), lpPacketHandler->GetSerializeBufferSize());
		}
	}
}

void JIGAPServer::RegisterServerLog(const char* fmt, ...)
{
	char buf[1024] = { 0 };
	va_list ap;
	
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	wchar_t tBuf[1024];
	MultiByteToWideChar(CP_ACP, 0, buf, -1, tBuf, 1024);

	//lpLogFunc((int*)tBuf);
}

void JIGAPServer::RegisterLogFunc(void* lpFuncPointer)
{
	 lpLogFunc = (LogFunc)lpFuncPointer;
}
