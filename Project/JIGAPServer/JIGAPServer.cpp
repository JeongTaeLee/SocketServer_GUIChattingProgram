#include "pch.h"
#include "JIGAPServer.h"
#include "JIGAPChatProcess.h"
#include "MySqlDB.h"

JIGAPServer::JIGAPServer()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(302926);
}

JIGAPServer::~JIGAPServer()
{
	google::protobuf::ShutdownProtobufLibrary();
}

bool JIGAPServer::CreateServerSocket()
{
	int errorCode = 0;

	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	lpServerSocket = new TCPSocket();
	

	if ((errorCode = lpServerSocket->IOCPSocket()) != 0)
	{
		RegisterServerLog("Socket create failed (ErrorCode : %d)", errorCode);
		delete lpServerSocket;
		return false;
	}

	RegisterServerLog("Socket Create Success");

	if ((errorCode = lpServerSocket->Bind(serverData.GetPortAddress().c_str())) != 0)
	{
		RegisterServerLog("Socket bind failed(ErrorCode : %d)", errorCode);
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	RegisterServerLog("Socket bind success");

	LINGER LingerStruct;

	LingerStruct.l_onoff = 1;
	LingerStruct.l_linger = 0;
	::setsockopt(lpServerSocket->GetSocket(), SOL_SOCKET, SO_LINGER, (char*)& LingerStruct, sizeof(LingerStruct));


	hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if (hCompletionHandle == nullptr)
	{
		RegisterServerLog("CompletionPort create failed");
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	RegisterServerLog("CompletePort create success");

	if ((errorCode = lpServerSocket->Listen(100)) != 0)
	{
		RegisterServerLog("Socket Listen failed (ErrorCode : %d)", errorCode);
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	RegisterServerLog("Socket listen Success");

	return true;
}

bool JIGAPServer::ServerInitialize(const std::string& inPortAddress)
{
	lpServerProcess = new JIGAPChatProcess(this);
	lpServerProcess->OnInitialize();
	
	serverData.SetServerData(inPortAddress);
	
	if (CreateServerSocket() == false)
	{	
		RegisterServerLog("Server initiliaze failed");
		ServerRelease();
		return false;
	}
	else
		RegisterServerLog("Server intialize success");
	
	
	RegisterServerLog("Start Server");

	
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
	
	if (lpServerSocket)
	{
		lpServerSocket->Closesocket();
		CloseHandle(hCompletionHandle);
		delete lpServerSocket;
	}
	
	if (hConnectThread.joinable())
		hConnectThread.join();
	RegisterServerLog("End ConnecThread");
	
	Sleep(100);
	
	if (hRecvThread.joinable())
		hRecvThread.join();
	RegisterServerLog("End RecvThread");
	
	RegisterServerLog("End Server");
}


bool JIGAPServer::StartServer(const std::string& inPortAddress)
{

	bServerOn = ServerInitialize(inPortAddress);
	return bServerOn;
}

void JIGAPServer::CloseServer()
{
	ServerRelease();
}

void JIGAPServer::OnConnectTask()
{
	RegisterServerLog("Start ConnectThread");

	while (true)
	{
		TCPSocket * acceptSocket = lpServerSocket->Accept<TCPSocket>();

		if (bServerOn == false)
			break;

		if (acceptSocket)
		{
			HANDLE hHandle = CreateIoCompletionPort((HANDLE)acceptSocket->GetSocket(), hCompletionHandle, (ULONG_PTR)acceptSocket, NULL);
		
			if (hHandle == nullptr)
			{	
				RegisterServerLog("JIGAPServer.cpp  / CompletionPort Connect NewClient failed (SOCKET : %d)", acceptSocket->GetSocket());
				acceptSocket->Closesocket();
				SAFE_DELETE(acceptSocket);
				continue;
			}
	
			int iError = 0;
			if ((iError = acceptSocket->IOCPRecv()) != 0)
			{
				RegisterServerLog("JIGAPServer.cpp / NewClient Change RecvMode failed (ErrorCode : %d) ", iError);
				continue;
			}

			RegisterServerLog("NewClient Connected Server (SOCKET : %d)", acceptSocket->GetSocket());

			if (lpServerProcess)
				lpServerProcess->OnConnect(acceptSocket);
		}
		else
		{
			RegisterServerLog("JIGAPServer.cpp / NewClient Connected Server failed");
			continue;
		}
	}
}

void JIGAPServer::OnRecvPacketTask()
{
	RegisterServerLog("Start Recv Thread");

	PacketHandler *packetHandler = new PacketHandler();

	while (true)
	{
		DWORD iRecvByte = 0;
		
		TCPSocket* lpTCPSocket = nullptr;
		TCPIOData* lpTCPIoData = nullptr;

		GetQueuedCompletionStatus(hCompletionHandle, &iRecvByte, (PULONG_PTR)& lpTCPSocket,
			(LPOVERLAPPED*)& lpTCPIoData, INFINITE);

		if (bServerOn == false)
			break;
		
		if (iRecvByte == 0)
		{
			if (lpTCPSocket->GetSocket() == INVALID_SOCKET)
				continue;

			lpServerProcess->OnDisconnect(lpTCPSocket);

			lpTCPSocket->Shutdownsocket(SD_SEND);
			lpTCPSocket->Closesocket();
			
			RegisterServerLog("Disconnect Client Socket(SOCKET : %d)", lpTCPSocket->GetSocket());
			SAFE_DELETE(lpTCPSocket);
		}
		else
		{	
			if (lpTCPSocket->GetIOMode() == IOMODE::E_IOMODE_RECV)
			{
				int iRealRecvSize = packetHandler->ParsingBufferSize(lpTCPSocket->GetBufferData());
				packetHandler->ClearParsingBuffer(lpTCPSocket->GetBufferData(), iRealRecvSize);
				packetHandler->ClearSerializeBuffer();

				lpServerProcess->OnProcess(lpTCPSocket, packetHandler);
			}
			else
				lpTCPSocket->IOCPRecv();
		}

	}

	SAFE_DELETE(packetHandler);
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

	lpLogFunc((int*)tBuf);
}

void JIGAPServer::RegisterLogFunc(void* lpFuncPointer)
{
	 lpLogFunc = (LogFunc)lpFuncPointer;
}
