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
	
	try
	{
		WSADATA data;
		WSAStartup(MAKEWORD(2, 0), &data);

		lpServerSocket = new TCPSocket();
		if ((errorCode = lpServerSocket->IOCPSocket()) != 0)
			throw "IOCPSocket ErrorCode - %d";

		LINGER LingerStruct;
		LingerStruct.l_onoff = 1;
		LingerStruct.l_linger = 0;
		::setsockopt(lpServerSocket->GetSocket(), SOL_SOCKET, SO_LINGER, (char*)& LingerStruct, sizeof(LingerStruct));

		if ((errorCode = lpServerSocket->Bind(serverData.GetPortAddress().c_str())) != 0)
			throw "Bind ErrorCode - %d";

		hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
		if (hCompletionHandle == nullptr)
			throw "CreateIoCompletionPort ErrorCode - %d";

		if ((errorCode = lpServerSocket->Listen(100)) != 0)
			throw "Listen ErrorCode - %d";
	}
	catch (const char* functionName)
	{
		ServerRelease();
		
		char ch[256];
		sprintf(ch, functionName, WSAGetLastError());
		throw std::exception(ch);

		return false;
	}

	RegisterServerLog("Success to Start Server");
	return true;
}

bool JIGAPServer::ServerInitialize(const std::string& inPortAddress)
{
	lpServerProcess = new JIGAPChatProcess(this);
	lpServerProcess->OnInitialize();
	
	serverData.SetServerData(inPortAddress);
	
	if (CreateServerSocket() == false)
		return false;
	
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
	
	if (hCompletionHandle != INVALID_HANDLE_VALUE)
		CloseHandle(hCompletionHandle);

	if (lpServerSocket)
	{
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
	}
	
	if (hConnectThread.joinable())
		hConnectThread.join();
	if (hRecvThread.joinable())
		hRecvThread.join();
	
	RegisterServerLog("Success End Server");
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
	RegisterServerLog("Success To Start ConnectTask");

	while (true)
	{
		TCPSocket * acceptSocket = lpServerSocket->Accept<TCPSocket>();

		if (bServerOn == false)
			break;

		if (acceptSocket)
		{
			try
			{
				HANDLE hHandle = CreateIoCompletionPort((HANDLE)acceptSocket->GetSocket(), hCompletionHandle, (ULONG_PTR)acceptSocket, NULL);

				if (hHandle == nullptr)
					throw "CreateIoCompletionPort ErrorCode - %d";

				if (acceptSocket->IOCPRecv() != 0)
					throw "IOCPRecv ErrorCode - %d";
			
				RegisterServerLog("Success To Connect Client (Socket : %d)", acceptSocket->GetSocket());

				if (lpServerProcess)
					lpServerProcess->OnConnect(acceptSocket);
			}
			catch (const char* errorMessage)
			{
				acceptSocket->Closesocket();
				SAFE_DELETE(acceptSocket);

				char ch[256];
				sprintf(ch, errorMessage, WSAGetLastError());
				continue;
			}
		}
		else
		{
			RegisterServerLog("Failed To Connect Client");
			continue;
		}
	}

	RegisterServerLog("Success To End ConnectTask");

}

void JIGAPServer::OnRecvPacketTask()
{
	RegisterServerLog("Success To Start RecvTask");

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
			lpTCPSocket->DownReferenceCount();
			if (lpTCPSocket->GetReferenceCount() <= 0)
			{	
				lpServerProcess->OnDisconnect(lpTCPSocket);

				RegisterServerLog("Disconnect Client Socket(SOCKET : %d)", lpTCPSocket->GetSocket());

				lpTCPSocket->Shutdownsocket(SD_SEND);
				lpTCPSocket->Closesocket();

				
				SAFE_DELETE(lpTCPSocket);
				lpTCPSocket = nullptr;
			}
		}
		else
		{	
			if (lpTCPSocket->GetIOMode() == IOMODE::E_IOMODE_RECV)
			{
				lpTCPSocket->OffRecvingFlag();
				lpTCPSocket->DownReferenceCount();

				int iRealRecvSize = packetHandler->ParsingBufferSize(lpTCPSocket->GetBufferData());
				packetHandler->ClearParsingBuffer(lpTCPSocket->GetBufferData(), iRealRecvSize);
				packetHandler->ClearSerializeBuffer();

				lpServerProcess->OnProcess(lpTCPSocket, packetHandler);
			}
			else
			{
				lpTCPSocket->DownReferenceCount();
				lpTCPSocket->IOCPRecv();
			}
		}

	}

	SAFE_DELETE(packetHandler);

	RegisterServerLog("Success To End RecvTask");
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
