#include "pch.h"
#include "JIGAPServer.h"
#include "JIGAPChatServer.h"

bool JIGAPServer::CreateServerSocket()
{
	int errorCode = 0;

	lpServerSocket = new TCPSocket();
	//Server�� ������ �����մϴ�.
	if ((errorCode = lpServerSocket->IOCPSocket()) != 0)
	{
		RegisterServerLog("���� ������ ������ ���� �߽��ϴ�. (ErrorCode : %d)", errorCode);
		delete lpServerSocket;
		return false;
	}

	if ((errorCode = lpServerSocket->Bind(serverData.GetIpAdress().c_str(), serverData.GetPortAddress().c_str())) != 0)
	{
		RegisterServerLog("���� ������ �Ҵ��� ���� �߽��ϴ�. (ErrorCode : %d)", errorCode);
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if (hCompletionHandle == nullptr)
	{
		RegisterServerLog("���� ������ CompletionPort�� �������� ���߽��ϴ�.");
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	if ((errorCode = lpServerSocket->Listen(100)) != 0)
	{
		RegisterServerLog("���� ������ ���� ��� ���� �������� ���߽��ϴ�. (ErrorCode : %d)", errorCode);
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	return true;
}

bool JIGAPServer::ServerInitialize(const std::string& inIpAddress, const std::string& inPortAddress)
{
	lpServerProcess = new JIGAPChatServer();

	serverData.SetServerData(inIpAddress, inPortAddress);

	hServerLogMutex = CreateMutex(nullptr, false, nullptr);
	lpPacketHandler = new PacketHandler();

	hConnectThread	= std::thread([&]() { OnConnectTask(); });
	hRecvThread		= std::thread([&]() { OnRecvPacketTask(); });

	if (CreateServerSocket() == false)
	{
		RegisterServerLog("������ �ʱ�ȭ�� �����߽��ϴ�. ������ ������� ���߽��ϴ�.");
		ServerRelease();
		return false;
	}
	else
		RegisterServerLog("���� ������ ���������� �ʱ�ȭ �Ǿ����ϴ�. ������ ����Ǿ����ϴ�.");

	return true;
}

void JIGAPServer::ServerRelease()
{
	bServerOn = false;

	CloseHandle(hServerLogMutex);

	if (lpServerSocket)
	{
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
	}

	if (hConnectThread.joinable())
		hConnectThread.join();
	if (hRecvThread.joinable())
		hRecvThread.join();
}

bool JIGAPServer::StartServer(const std::string& inIpAddress, const std::string& inPortAddress)
{
	bServerOn = ServerInitialize(inIpAddress, inPortAddress);
	return bServerOn;
}

void JIGAPServer::CloseServer()
{
	ServerRelease();
}

void JIGAPServer::OnConnectTask()
{
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
			if ((iError == acceptSocket->IOCPRecv()) != 0)
			{
				RegisterServerLog("JIGAPServer.cpp / ������ ������ ���Ż��·� �������� ���߽��ϴ� (ErrorCode : %d) ", iError);
				continue;
			}

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
	while (true)
	{
		DWORD iRecvByte = 0;
		
		TCPSocket* lpTCPSocket = nullptr;
		TCPIOData* lpTCPIoData = nullptr;

		if (GetQueuedCompletionStatus(hCompletionHandle, &iRecvByte, (PULONG_PTR)& lpTCPSocket,
			(LPOVERLAPPED*)&lpTCPIoData, INFINITE) == false)
		{
			RegisterServerLog("JIGAPServer.cpp / ��Ŷ�� �޴� ���� �ɰ��� ������ ������ϴ�.");
			continue;
		}

		if (bServerOn == false)
			break;
		
		if (iRecvByte == 0)
		{
			lpServerProcess->OnDisconnect(lpTCPSocket);
			closesocket(lpTCPSocket->GetSocket());
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
	/*Mutex�� �����մϴ�. �ش��ڵ��� non-signaled ���°� �˴ϴ� �ݸ� �̹� �ڵ��� Non-signaled �����̸� ���ŷ�մϴ�. */
	StartMutex(hServerLogMutex);
	{
		char buf[512] = { 0 };
		va_list ap;

		va_start(ap, fmt);
		vsprintf(buf, fmt, ap);
		va_end(ap);

		qServerLog.push(buf);
	} 
	/*Mutex �������� �ʰ� �ٲ��ݴϴ�. ���ؽ��� signaled ���·� �ٲߴϴ�*/
	EndMutex(hServerLogMutex);
}

std::string JIGAPServer::PopServerLog()
{
	std::string strSystemMessage;
	bool bPopSuccess = false;

	StartMutex(hServerLogMutex);
	{	
		if (qServerLog.size() >= 0)
		{
			/*���� ������ �޽����� �����ɴϴ�.*/
			strSystemMessage = qServerLog.front();

			/*���� ������ �޽����� Queue���� ����ϴ�.*/
			qServerLog.pop();

			bPopSuccess = true;
		}
		else
			bPopSuccess = false;
	}
	EndMutex(hServerLogMutex);

	if (bPopSuccess)
		return strSystemMessage;

	return std::string();
}
