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

	/*Socket을 Bind 합니다*/
	lpServData->SockAddr.sin_family = PF_INET;
	lpServData->SockAddr.sin_addr.s_addr = inet_addr(szIpAddr.c_str());
	lpServData->SockAddr.sin_port = atoi(szIpAddr.c_str());
	if (bind(lpServData->hSock, (SOCKADDR*)& lpServData->SockAddr, sizeof(lpServData->SockAddr)) == SOCKET_ERROR)
		return E_FAIL;

	/*연결 대기열을 생성합니다*/
	if (listen(lpServData->hSock, 10) == SOCKET_ERROR)
		return E_FAIL;

	return S_OK;
}

void JIGAPServer::ReleaseServer()
{
	closesocket(lpServData->hSock);
	WSACleanup();

	if (chattingThread.joinable())
		chattingThread.join();

	if (connectThread.joinable())
		connectThread.join();

	delete lpServData;
	lpServData = nullptr;
}

void JIGAPServer::ConnectThread()
{
	while (bServerOn)
	{

	}
}

void JIGAPServer::ChattingThread(HANDLE hCompletion)
{
	while (bServerOn)
	{

	}
}

bool JIGAPServer::JIGAPServerOpen(std::string _szIpAddr, std::string _szPortAddr)
{
	szIpAddr = _szIpAddr;
	szPortAddr = _szPortAddr;

	if (FAILED(JIGAPServer::InitializeServer()))
		return false;

	bServerOn = true;

	connectThread = std::thread([&]() { ConnectThread(); });

	return true;
}

void JIGAPServer::JIGAPServerClose()
{
	bServerOn = false;
	ReleaseServer();
}
