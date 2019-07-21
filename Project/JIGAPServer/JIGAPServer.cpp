#include "pch.h"
#include "JIGAPServer.h"
#include "JIGAPChatServer.h"

bool JIGAPServer::CreateServerSocket()
{
	int errorCode = 0;

	lpServerSocket = new TCPSocket();
	//Server의 소켓을 생성합니다.
	if ((errorCode = lpServerSocket->IOCPSocket()) != 0)
	{
		RegisterServerLog("서버 소켓의 생성을 실패 했습니다. (ErrorCode : %d)", errorCode);
		delete lpServerSocket;
		return false;
	}

	if ((errorCode = lpServerSocket->Bind(serverData.GetIpAdress().c_str(), serverData.GetPortAddress().c_str())) != 0)
	{
		RegisterServerLog("서버 소켓의 할당을 실패 했습니다. (ErrorCode : %d)", errorCode);
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if (hCompletionHandle == nullptr)
	{
		RegisterServerLog("서버 소켓의 CompletionPort를 생성하지 못했습니다.");
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	if ((errorCode = lpServerSocket->Listen(100)) != 0)
	{
		RegisterServerLog("서버 소켓을 연결 대기 모드로 변경하지 못했습니다. (ErrorCode : %d)", errorCode);
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
		RegisterServerLog("서버의 초기화를 실패했습니다. 서버가 실행되지 못했습니다.");
		ServerRelease();
		return false;
	}
	else
		RegisterServerLog("서버 소켓이 성공적으로 초기화 되었습니다. 서버가 실행되었습니다.");

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
				RegisterServerLog("JIGAPServer.cpp  / CompletionPort의 연결의 심각한 문제가 생겼습니다. (SOCKET : %d)", acceptSocket->GetSocket());
				acceptSocket->Closesocket();
				SAFE_DELETE(acceptSocket);
				continue;
			}
	
			int iError = 0;
			if ((iError == acceptSocket->IOCPRecv()) != 0)
			{
				RegisterServerLog("JIGAPServer.cpp / 연결한 소켓을 수신상태로 변경하지 못했습니다 (ErrorCode : %d) ", iError);
				continue;
			}

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
	while (true)
	{
		DWORD iRecvByte = 0;
		
		TCPSocket* lpTCPSocket = nullptr;
		TCPIOData* lpTCPIoData = nullptr;

		if (GetQueuedCompletionStatus(hCompletionHandle, &iRecvByte, (PULONG_PTR)& lpTCPSocket,
			(LPOVERLAPPED*)&lpTCPIoData, INFINITE) == false)
		{
			RegisterServerLog("JIGAPServer.cpp / 패킷을 받는 도중 심각한 문제가 생겼습니다.");
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
	/*Mutex를 소유합니다. 해당핸들은 non-signaled 상태가 됩니다 반면 이미 핸들이 Non-signaled 상태이면 블록킹합니다. */
	StartMutex(hServerLogMutex);
	{
		char buf[512] = { 0 };
		va_list ap;

		va_start(ap, fmt);
		vsprintf(buf, fmt, ap);
		va_end(ap);

		qServerLog.push(buf);
	} 
	/*Mutex 소유하지 않게 바꿔줍니다. 뮤텍스를 signaled 상태로 바꿉니다*/
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
			/*가장 오래된 메시지를 가져옵니다.*/
			strSystemMessage = qServerLog.front();

			/*가장 오래된 메시지를 Queue에서 지웁니다.*/
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
