#include "pch.h"
#include "JIGAPServer.h"
#include "JIGAPChatProcess.h"
#include "MySqlDB.h"

#include "LogManager.h"

JIGAPServer::JIGAPServer()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2811);
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
			throw CustomException(__LINE__, __FILEW__, "ServerSocket fail create socket");

		LINGER LingerStruct;
		LingerStruct.l_onoff = 1;
		LingerStruct.l_linger = 0;
		::setsockopt(lpServerSocket->GetSocket(), SOL_SOCKET, SO_LINGER, (char*)& LingerStruct, sizeof(LingerStruct));

		if ((errorCode = lpServerSocket->Bind(serverData.GetPortAddress().c_str())) != 0)
			throw CustomException(__LINE__, __FILEW__, "ServerSocket fail bind");

		hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
		if (hCompletionHandle == nullptr)
			throw CustomException(__LINE__, __FILEW__, "ServerSocket fail connect iocp");

		if ((errorCode = lpServerSocket->Listen(100)) != 0)
			throw CustomException(__LINE__, __FILEW__, "ServerSocket fail listen");
	}
	catch (std::exception & ex)
	{
		LOGMANAGER.LogError(__FILEW__, __LINE__, ex.what());

		ServerRelease();
		return false;
	}

	return true;
}

bool JIGAPServer::ServerInitialize(const std::string& inPortAddress)
{
	

	lpServerProcess = new JIGAPChatProcess(this);
	lpServerProcess->OnInitialize();
	
	serverData.SetServerData(inPortAddress);
	
	if (CreateServerSocket() == false)
		return false;

	LOGMANAGER.Log("Start Server");
	
	hConnectThread = std::thread([&]() { OnConnectTask(); });
	Sleep(100);
	hRecvThread = std::thread([&]() { OnRecvPacketTask(); });

	return true;
}

void JIGAPServer::ServerRelease()
{
	bServerOn = false;
	
	lpServerProcess->OnRelease();

	if (hCompletionHandle != INVALID_HANDLE_VALUE)
		CloseHandle(hCompletionHandle);

	if (lpServerSocket)
		lpServerSocket->Closesocket();

	if (hConnectThread.joinable())
		hConnectThread.join();

	if (hRecvThread.joinable())
		hRecvThread.join();

	SAFE_DELETE(lpServerProcess);
	SAFE_DELETE(lpServerSocket);
	
}

bool JIGAPServer::StartServer(const std::string& inPortAddress)
{
	bServerOn = ServerInitialize(inPortAddress);
	return bServerOn;
}

void JIGAPServer::CloseServer()
{
	ServerRelease();
	LOGMANAGER.Log("End Server");
}

void JIGAPServer::OnConnectTask()
{
	LOGMANAGER.Log("Start connect thread");

	while (true)
	{
		TCPSocket* acceptSocket = nullptr;

		try
		{
			acceptSocket = lpServerSocket->Accept<TCPSocket>();

			HANDLE hHandle = CreateIoCompletionPort((HANDLE)acceptSocket->GetSocket(), hCompletionHandle, (ULONG_PTR)acceptSocket, NULL);

			if (hHandle == nullptr)
				throw CustomException(__LINE__, __FILEW__, "[JIGAPServer.OnConnectTask] Fail Register Socket in CreateIoCompletionPort");

			if (acceptSocket->IOCPRecv() != 0)
				throw CustomException(__LINE__, __FILEW__, "[JIGAPServer.OnConnectTask] Fail Start IOCPRecv");

			if (lpServerProcess)
				lpServerProcess->OnConnect(acceptSocket);
		}
		catch (CustomException& ex)
		{
			if (acceptSocket)
			{
				acceptSocket->Closesocket();
				SAFE_DELETE(acceptSocket);
			}

			if (!bServerOn)
				break;

			LOGMANAGER.LogError(ex.WhatPath().c_str(), ex.WhatLine(), ex.what());
			continue;
		}
		catch (std::exception& ex)
		{
			if (acceptSocket)
			{
				acceptSocket->Closesocket();
				SAFE_DELETE(acceptSocket);
			}

			if (!bServerOn)
				break;

			LOGMANAGER.LogError(__FILEW__, __LINE__, ex.what());
		}
		
		

	}

	LOGMANAGER.Log("End connect thread");
}

void JIGAPServer::OnRecvPacketTask()
{
	LOGMANAGER.Log("Start recv thread");

	PacketHandler *packetHandler = new PacketHandler();

	while (true)
	{
		DWORD iRecvByte = 0;

		TCPSocket* lpTCPSocket = nullptr;
		TCPIOData* lpTCPIoData = nullptr;

		GetQueuedCompletionStatus(hCompletionHandle, &iRecvByte, (PULONG_PTR)& lpTCPSocket,
			(LPOVERLAPPED*)& lpTCPIoData, INFINITE);

		if (!bServerOn)
			break;

		try
		{
			if (iRecvByte == 0)
			{
				lpTCPSocket->DownReferenceCount();
				
				if (lpTCPSocket->GetReferenceCount() == 0)
					lpServerProcess->OnDisconnect(lpTCPSocket);

				continue;
			}
			else if (iRecvByte <= 4 && lpTCPSocket->GetIOMode() == IOMODE::E_IOMODE_RECV)
				throw CustomException(__LINE__, __FILEW__, "The packet is not normal.");
			else if (iRecvByte >= 1024 && lpTCPSocket->GetIOMode() == IOMODE::E_IOMODE_RECV)
				throw CustomException(__LINE__, __FILEW__, "Allowed packet communication size exceeded.");
			else
			{
				if (lpTCPSocket->GetIOMode() == IOMODE::E_IOMODE_RECV)
				{
					lpTCPSocket->DownReferenceCount();
					lpTCPSocket->OffRecvingFlag();

					int iRealRecvSize = packetHandler->ParsingBufferSize(lpTCPSocket->GetBufferData());

					if (iRealRecvSize >= 1024)
						throw CustomException(__LINE__, __FILEW__, "Allowed packet communication size exceeded.");
		
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
		catch (CustomException & ex)
		{
			lpTCPSocket->DownReferenceCount();
			lpTCPSocket->IOCPRecv();
			LOGMANAGER.LogError(ex.WhatPath().c_str(), ex.WhatLine(), ex.what());
		}
		catch  (std::exception & ex)
		{
			lpTCPSocket->DownReferenceCount();
			lpTCPSocket->IOCPRecv();
			LOGMANAGER.LogError(__FILEW__, __LINE__, ex.what());
		}
		
	
	}

	SAFE_DELETE(packetHandler);
	LOGMANAGER.Log("End recv thread");
}

void JIGAPServer::RegisterLogFunc(void* lpFuncPointer)
{
	LOGMANAGER.Initialize((LogFunc)lpFuncPointer);
}
