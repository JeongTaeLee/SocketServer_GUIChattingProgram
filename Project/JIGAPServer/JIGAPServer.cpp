#include "pch.h"
#include "JIGAPServer.h"
#include "TCPSocket.h"

bool JIGAPServer::CreateServerSocket()
{
	lpServerSocket = new TCPSocket();
	//Server의 소켓을 생성합니다.
	if (lpServerSocket->IOCPSocket() != 0)
	{
		RegisterServerLog("서버 소켓의 생성을 실패 했습니다.");
		delete lpServerSocket;
		return false;
	}

	if (lpServerSocket->Bind(serverData.GetIpAdress().c_str(), serverData.GetPortAddress().c_str()) != 0)
	{
		RegisterServerLog("서버 소켓의 할당을 실패 했습니다. (SOCKET : %d)", lpServerSocket->GetSocket());
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	if (lpServerSocket->Listen(100) != 0)
	{
		RegisterServerLog("서버 소켓을 연결 대기 모드로 변경하지 못했습니다. (SOCKET : %d)", lpServerSocket->GetSocket());
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

	return true;
}

bool JIGAPServer::ServerInitialize(const std::string& inIpAddress, const std::string& inPortAddress)
{
	serverData.SetServerData(inIpAddress, inPortAddress);

	hServerLogMutex = CreateMutex(nullptr, false, nullptr);
	hUsersMapMutex	= CreateMutex(nullptr, false, nullptr);
	hWorkMutex		= CreateMutex(nullptr, false, nullptr);

	hConnectThread	= std::thread([&]() { OnConnectTask(); });
	hRecvThread		= std::thread([&]() { OnRecvPacketTask(); });
	hSendThread		= std::thread([&]() { OnSendPacketTask(); });

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
	CloseHandle(hServerLogMutex);
	CloseHandle(hUsersMapMutex);
	CloseHandle(hWorkMutex);

	if (hConnectThread.joinable())
		hConnectThread.join();
	if (hRecvThread.joinable())
		hRecvThread.join();
	if (hSendThread.joinable())
		hSendThread.join();

	if (lpServerSocket)
	{
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
	}
}

void JIGAPServer::OnConnectTask()
{
	while (true)
	{
		TCPSocket * acceptSocket = lpServerSocket->Accept<TCPSocket>();
	
		if (acceptSocket)
		{
			HANDLE hHandle = CreateIoCompletionPort((HANDLE)acceptSocket->GetSocket(), hCompletionHandle, (ULONG_PTR)acceptSocket, NULL);
		
			if (hCompletionHandle == nullptr)
			{	
				RegisterServerLog("JIGAPServer.cpp : 104줄 / CompletionPort의 연결의 심각한 문제가 생겼습니다. (SOCKET : %d)", acceptSocket->GetSocket());
				acceptSocket->Closesocket();
				delete acceptSocket;
				continue;
			}
			
			WaitForSingleObject(hUsersMapMutex, INFINITE);
			{
				mUsers.insert(std::make_pair(acceptSocket->GetSocket(), acceptSocket));
			}
			ReleaseMutex(hUsersMapMutex);
		}
		else
		{
			RegisterServerLog("JIGAPServer.cpp : 68줄 / 소켓 연결의 심각한 문제가 생겼습니다.");
			continue;
		}
	}
}

void JIGAPServer::OnRecvPacketTask()
{
}

void JIGAPServer::OnSendPacketTask()
{
}

void JIGAPServer::RegisterServerLog(const char* fmt, ...)
{
	/*Mutex를 소유합니다. 해당핸들은 non-signaled 상태가 됩니다 반면 이미 핸들이 Non-signaled 상태이면 블록킹합니다. */
	WaitForSingleObject(hServerLogMutex, INFINITE);
	{
		char buf[512] = { 0 };
		va_list ap;

		va_start(ap, fmt);
		vsprintf(buf, fmt, ap);
		va_end(ap);

		qServerLog.push(buf);
	} 
	/*Mutex 소유하지 않게 바꿔줍니다. 뮤텍스를 signaled 상태로 바꿉니다*/
	ReleaseMutex(hServerLogMutex);
}

std::string JIGAPServer::PopServerLog()
{
	std::string strSystemMessage;
	bool bPopSuccess = false;

	WaitForSingleObject(hServerLogMutex, INFINITE);
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
	ReleaseMutex(hServerLogMutex);

	if (bPopSuccess)
		return strSystemMessage;

	return std::string();
}
