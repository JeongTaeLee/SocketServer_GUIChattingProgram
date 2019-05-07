#include "pch.h"
#include "JIGAPClient.h"


JIGAPClient::JIGAPClient()
{
}


JIGAPClient::~JIGAPClient()
{
}

HRESULT JIGAPClient::InitializeClient()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return E_FAIL;

	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		return E_FAIL;
	
	sockaddr_in tempSockAddrIn;
	memset(&tempSockAddrIn, 0, sizeof(tempSockAddrIn));
	
	tempSockAddrIn.sin_family = PF_INET;
	tempSockAddrIn.sin_addr.s_addr = inet_addr(strIpAddr.c_str());
	tempSockAddrIn.sin_port = atoi(strPortAddr.c_str());
	
	if (connect(sock, (sockaddr*)& tempSockAddrIn, sizeof(tempSockAddrIn) == SOCKET_ERROR))
		return E_FAIL;

	lpHandleData = new HANDLE_DATA;
	lpHandleData->hSock = sock;
	memcpy(&lpHandleData->SockAddr, &tempSockAddrIn, sizeof(lpHandleData->SockAddr));

	return S_OK;
}

HRESULT JIGAPClient::ReleaseClient()
{
	return E_NOTIMPL;
}

bool JIGAPClient::JIGAPClientStart(const std::string& InIpAddr, const std::string& InPortAddr)
{
	strIpAddr = InIpAddr;
	strPortAddr = InPortAddr;

	if (FAILED(InitializeClient()))
	{
		MessageBoxA(nullptr, "서버에 연결 할 수 없습니다!", "Error!", MB_OK);
		return false;
	}

	return true;
}

void JIGAPClient::JIGAPClientEnd()
{
}
