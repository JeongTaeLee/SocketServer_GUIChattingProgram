#include "pch.h"
#include "JIGAPServer.h"
#include "TCPSocket.h"

bool JIGAPServer::CreateServerSocket()
{
	lpServerSocket = new TCPSocket();
	//Server�� ������ �����մϴ�.
	if (lpServerSocket->IOCPSocket() != 0)
	{
		RegisterServerLog("���� ������ ������ ���� �߽��ϴ�.");
		delete lpServerSocket;
		return false;
	}

	if (lpServerSocket->Bind(serverData.GetIpAdress().c_str(), serverData.GetPortAddress().c_str()) != 0)
	{
		RegisterServerLog("���� ������ �Ҵ��� ���� �߽��ϴ�. (SOCKET : %d)", lpServerSocket->GetSocket());
		lpServerSocket->Closesocket();
		SAFE_DELETE(lpServerSocket);
		return false;
	}

	if (lpServerSocket->Listen(100) != 0)
	{
		RegisterServerLog("���� ������ ���� ��� ���� �������� ���߽��ϴ�. (SOCKET : %d)", lpServerSocket->GetSocket());
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
				RegisterServerLog("JIGAPServer.cpp : 104�� / CompletionPort�� ������ �ɰ��� ������ ������ϴ�. (SOCKET : %d)", acceptSocket->GetSocket());
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
			RegisterServerLog("JIGAPServer.cpp : 68�� / ���� ������ �ɰ��� ������ ������ϴ�.");
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
	/*Mutex�� �����մϴ�. �ش��ڵ��� non-signaled ���°� �˴ϴ� �ݸ� �̹� �ڵ��� Non-signaled �����̸� ���ŷ�մϴ�. */
	WaitForSingleObject(hServerLogMutex, INFINITE);
	{
		char buf[512] = { 0 };
		va_list ap;

		va_start(ap, fmt);
		vsprintf(buf, fmt, ap);
		va_end(ap);

		qServerLog.push(buf);
	} 
	/*Mutex �������� �ʰ� �ٲ��ݴϴ�. ���ؽ��� signaled ���·� �ٲߴϴ�*/
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
			/*���� ������ �޽����� �����ɴϴ�.*/
			strSystemMessage = qServerLog.front();

			/*���� ������ �޽����� Queue���� ����ϴ�.*/
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
