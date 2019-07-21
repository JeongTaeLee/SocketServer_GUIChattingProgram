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
		RegisterServerLog("���� ������ ������ ���� �߽��ϴ�. (ErrorCode : %d)", errorCode);
		delete lpServerSocket;
		return false;
	}

	RegisterServerLog("���� ������ �����Ǿ����ϴ�.");

	if ((errorCode = lpServerSocket->Bind(serverData.GetIpAdress().c_str(), serverData.GetPortAddress().c_str())) != 0)
	{
		RegisterServerLog("���� ������ �Ҵ��� ���� �߽��ϴ�. (ErrorCode : %d)", errorCode);
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	RegisterServerLog("���� ������ �Ҵ��߽��ϴ�");

	hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if (hCompletionHandle == nullptr)
	{
		RegisterServerLog("���� ������ CompletionPort�� �������� ���߽��ϴ�.");
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	RegisterServerLog("���� ������ CompletionPort�� �����߽��ϴ�");

	if ((errorCode = lpServerSocket->Listen(100)) != 0)
	{
		RegisterServerLog("���� ������ ���� ��� ���� �������� ���߽��ϴ�. (ErrorCode : %d)", errorCode);
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	RegisterServerLog("���� ������ ��� ���� �����߽��ϴ�");

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
		RegisterServerLog("������ �ʱ�ȭ�� �����߽��ϴ�. ������ ������� ���߽��ϴ�.");
		ServerRelease();
		return false;
	}
	else
		RegisterServerLog("������ ����Ǿ����ϴ�.");
	
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
	RegisterServerLog("ConnectThread�� ����Ǿ����ϴ�");
	Sleep(100);
	if (hRecvThread.joinable())
		hRecvThread.join();
	RegisterServerLog("RecvThread�� ����Ǿ����ϴ�");
	RegisterServerLog("������ ����Ǿ����ϴ�.");
	
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
	RegisterServerLog("Connect Thread�� �����߽��ϴ�.");

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
				RegisterServerLog("JIGAPServer.cpp  / CompletionPort�� ������ �ɰ��� ������ ������ϴ�. (SOCKET : %d)", acceptSocket->GetSocket());
				acceptSocket->Closesocket();
				SAFE_DELETE(acceptSocket);
				continue;
			}
	
			int iError = 0;
			if ((iError = acceptSocket->IOCPRecv()) != 0)
			{
				RegisterServerLog("JIGAPServer.cpp / ������ ������ ���Ż��·� �������� ���߽��ϴ� (ErrorCode : %d) ", iError);
				continue;
			}

			RegisterServerLog("Ŭ���̾�Ʈ ������ ����Ǿ����ϴ� (SOCKET : %d)", acceptSocket->GetSocket());

			if (lpServerProcess)
				lpServerProcess->OnConnect(acceptSocket);
		}
		else
		{
			RegisterServerLog("JIGAPServer.cpp / ���� ������ �ɰ��� ������ ������ϴ�.");
			continue;
		}
	}
}

void JIGAPServer::OnRecvPacketTask()
{
	RegisterServerLog("Recv Thread�� �����߽��ϴ�");

	while (true)
	{
		DWORD iRecvByte = 0;
		
		TCPSocket* lpTCPSocket = nullptr;
		TCPIOData* lpTCPIoData = nullptr;

		GetQueuedCompletionStatus(hCompletionHandle, &iRecvByte, (PULONG_PTR)& lpTCPSocket,
			(LPOVERLAPPED*)& lpTCPIoData, INFINITE);
		//{
		//	RegisterServerLog("JIGAPServer.cpp / ��Ŷ�� �޴� ���� �ɰ��� ������ ������ϴ�.");
		//	continue;
		//}

		if (bServerOn == false)
			break;
		
		if (iRecvByte == 0)
		{
			lpServerProcess->OnDisconnect(lpTCPSocket);
			closesocket(lpTCPSocket->GetSocket());
			RegisterServerLog("Ŭ���̾�Ʈ ������ ���������Ǿ����ϴ�(SOCKET : %d)", lpTCPSocket->GetSocket());
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
