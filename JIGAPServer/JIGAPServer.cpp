// JIGAPServer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "JIGAPServer.h"



JIGAPServer::JIGAPServer()
	:lpServData(nullptr), bServerOn(false)
{
}

JIGAPServer::~JIGAPServer()
{
}

HRESULT JIGAPServer::InitializeServer()
{
	lpServData = new HANDLE_DATA;

	/*Winsock을 시작합니다*/
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return E_FAIL;

	/*WSASocket을 생성합니다.*/
	lpServData->hSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (lpServData->hSock == INVALID_SOCKET)
		return E_FAIL;

	PrintSystemLog("소켓이 성공적으로 생성되었습니다.");

	/*Socket을 Bind 합니다*/
	lpServData->SockAddr.sin_family = PF_INET;
	lpServData->SockAddr.sin_addr.s_addr = inet_addr(szIpAddr.c_str());
	lpServData->SockAddr.sin_port = atoi(szPortAddr.c_str());
	if (bind(lpServData->hSock, (SOCKADDR*)& lpServData->SockAddr, sizeof(lpServData->SockAddr)) == SOCKET_ERROR)
		return E_FAIL;

	PrintSystemLog("소켓이 성공적으로 할당되었습니다.");

	/*연결 대기열을 생성합니다*/
	if (listen(lpServData->hSock, 10) == SOCKET_ERROR)
		return E_FAIL;
	
	PrintSystemLog("서버의 연결대기열이 생성되었습니다.");

	hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);

	if (hCompletionHandle == nullptr)
		return E_FAIL;
	
	PrintSystemLog("서버의 CompletionPort가 생성되었습니다.");
	return S_OK;
}

void JIGAPServer::ReleaseServer()
{
	closesocket(lpServData->hSock);
	WSACleanup();

	bServerOn;

	Sleep(100);
	if (chattingThread.joinable())
		chattingThread.join();
	Sleep(100);
	if (connectThread.joinable())
		connectThread.join();

	delete lpServData;
	lpServData = nullptr;
}

void JIGAPServer::ConnectThread()
{
	PrintSystemLog("서버의 연결 담당 Thread가 작동되었습니다.");
	while (bServerOn)
	{
	/*	LPHANDLE_DATA lpClntData = new HANDLE_DATA;
		int iAddrSize = sizeof(lpClntData->SockAddr);
		lpClntData->hSock = accept(lpServData->hSock, (SOCKADDR*)& lpClntData->SockAddr, &iAddrSize);

		if (lpClntData->hSock == INVALID_SOCKET)
		{
			closesocket(lpClntData->hSock);
			delete lpClntData;
			qSystemMsg.push("연결을 시도하던 소켓과의 연결이 실패하였습니다");
			continue;
		}

		if (CreateIoCompletionPort((HANDLE)lpClntData->hSock, hCompletionHandle, (DWORD_PTR)lpClntData, 0) == NULL)
		{
			closesocket(lpClntData->hSock);
			delete lpClntData;
			qSystemMsg.push("CompletionPort에 연결을 시도하던 소켓을 할당하지 못했습니다.");
			continue;
		}
		
		char message[128];
		sprintf(message, "%d 소켓과 연결되었습니다.", lpClntData->hSock);
		qSystemMsg.push(message);

		LPIO_DATA  lpIOData = new IO_DATA;
		DWORD Flag = 0;
		DWORD dwRecvBytes = 0;

		WSARecv(lpClntData->hSock, &lpIOData->wsaBuf, 1, &dwRecvBytes, &Flag, &lpIOData->overlapped, NULL);*/
	}

	PrintSystemLog("서버의 연결 Thread가 해제되었습니다.");
}

void JIGAPServer::ChattingThread()
{
	PrintSystemLog("서버의 채팅 담당 Thread가 작동되었습니다.");
	while (bServerOn)
	{
		//LPHANDLE_DATA lpHandle = nullptr;
		//LPIO_DATA lpIO = nullptr;

		//DWORD dwByte = 0;
		//DWORD dwFlag = 0;

		//GetQueuedCompletionStatus(hCompletionHandle,
		//	&dwByte,
		//	(LPDWORD)& lpHandle,
		//	(LPOVERLAPPED*)& lpIO,
		//	INFINITE);

		//if (dwByte == 0)
		//{
		//	closesocket(lpHandle->hSock);
		//	delete lpHandle;
		//	delete lpIO;
		//	continue;
		//}

		//lpIO->wsaBuf.len = dwByte;
		//WSASend(lpHandle->hSock, &(lpIO->wsaBuf), 1, &dwByte, dwFlag, NULL, NULL);

	}

	PrintSystemLog("서버의 채팅 Thread가 해제되었습니다.");
}


bool JIGAPServer::JIGAPServerOpen(std::string _szIpAddr, std::string _szPortAddr)
{
	szIpAddr = _szIpAddr;
	szPortAddr = _szPortAddr;

	if (FAILED(JIGAPServer::InitializeServer()))
	{
		PrintSystemLog("서버를 열 수 없습니다!");
		return false;
	}

	bServerOn = true;

	connectThread = std::thread([&]() { ConnectThread(); });
	Sleep(100);
	chattingThread = std::thread([&]() { ChattingThread(); });
	Sleep(100);

	PrintSystemLog("서버를 열었습니다!");
	return true;
}

void JIGAPServer::JIGAPServerClose()
{
	bServerOn = false;
	ReleaseServer();

	PrintSystemLog("서버가 닫혔습니다.");
}

void JIGAPServer::PrintSystemLog(const std::string& key)
{
	systemLogMutex.lock();	
	qSystemMsg.push(key);
	systemLogMutex.unlock();
}

std::string JIGAPServer::JIGAPGetSystemMsg()
{
	/*가장 오래된 메시지를 가져옵니다.*/
	systemLogMutex.lock();
	std::string strSystemMessage = qSystemMsg.front();
	/*가장 오래된 메시지를 Queue에서 지웁니다.*/
	qSystemMsg.pop();
	systemLogMutex.unlock();
	return strSystemMessage;
}
