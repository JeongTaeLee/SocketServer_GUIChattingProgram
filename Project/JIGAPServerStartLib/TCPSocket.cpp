#include "pch.h"
#include "TCPSocket.h"

#include "SocketAddress.h"
#include "TCPIOData.h" 

TCPSocket::TCPSocket()
	:hSock(NULL), lpIOData(new TCPIOData)
{
}

TCPSocket::TCPSocket(SOCKET InSocket)
	:hSock(InSocket), lpIOData(new TCPIOData)
{
}

TCPSocket::TCPSocket(SOCKET hInSock, const SocketAddress& InAddr)
	:hSock(hInSock), lpIOData(new TCPIOData)
{
	sockAddr.SetAddress(InAddr);
}

TCPSocket::~TCPSocket()
{
	delete lpIOData;
}

int TCPSocket::IOCPSocket()
{
	hSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (hSock != INVALID_SOCKET)
		return 0;

	return WSAGetLastError();
}

int TCPSocket::SYNCSocket()
{
	hSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (hSock != INVALID_SOCKET)
		return 0;

	return WSAGetLastError();
}

int TCPSocket::Bind(const char* szInIpAddr, const char* szInPortAddr)
{
	sockAddr.SetAddress(szInIpAddr, szInPortAddr);

	int iBindResult = bind(hSock, sockAddr.GetAsSockAddr(), sockAddr.GetSize());
	if (iBindResult >= 0)
		return NO_ERROR;

	return WSAGetLastError();
}

int TCPSocket::Listen(int inBackLog)
{
	int err = listen(hSock, inBackLog);
	if (err >= 0)
		return NO_ERROR;
	return WSAGetLastError();
}

int TCPSocket::Connect(const char* szInIpAddr, const char* szInPortAddr)
{
	sockAddr.SetAddress(szInIpAddr, szInPortAddr);

	int iConnectResult = connect(hSock, sockAddr.GetAsSockAddr(), sockAddr.GetSize());
	if (iConnectResult >= 0)
		return NO_ERROR;
	return WSAGetLastError();
}

void TCPSocket::Closesocket()
{
	closesocket(hSock);
}

int TCPSocket::IOCPRecv()
{
	DWORD dwFlag = 0;
	lpIOData->wsaBuf.len = MAXBUFFERSIZE;

	if (WSARecv(hSock, &lpIOData->wsaBuf, 1, nullptr, &dwFlag, lpIOData, nullptr) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			return WSAGetLastError();
	}

	lpIOData->eIOMode = IOMODE::E_IOMODE_RECV;

	return 0;
}

int TCPSocket::IOCPSend(const char* szInStream, int iInSendSize)
{
	lpIOData->wsaBuf.len = iInSendSize;
	memcpy(lpIOData->szBuffer, szInStream, iInSendSize);

	DWORD dwRecvByte = 0;
	DWORD dwFlag = 0;	

	
	if (WSASend(hSock, &lpIOData->wsaBuf, 1, &dwRecvByte, dwFlag, (OVERLAPPED*)lpIOData, nullptr) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			return WSAGetLastError();
	}

	lpIOData->eIOMode = IOMODE::E_IOMODE_SEND;
	return 0;
}

int TCPSocket::SYNCRecv(char* szInBuf, int iInBufSize)
{	
	return recv(hSock, szInBuf, iInBufSize, 0);
}

int TCPSocket::SYNCSend(const char* szInBuf, int iInBufSize)
{
	return send(hSock, szInBuf, iInBufSize, 0);
}

const IOMODE& TCPSocket::GetIOMode()
{
	return lpIOData->eIOMode;
}

char* TCPSocket::GetBufferData()
{
	return lpIOData->szBuffer;
}
