#include "pch.h"
#include "TCPSocket.h"

TCPSocket::TCPSocket()
	:hSock(NULL), lpRecvData(new TCPIOData)//, lpSendData(new TCPIOData)
{
}

TCPSocket::TCPSocket(SOCKET InSocket)
	:hSock(InSocket), lpRecvData(new TCPIOData)//, lpSendData(new TCPIOData)
{
}

TCPSocket::TCPSocket(SOCKET hInSock, const SocketAddress& InAddr)
	:hSock(hInSock), lpRecvData(new TCPIOData)//, lpSendData(new TCPIOData)
{
	sockAddr.SetAddress(InAddr);
}

TCPSocket::~TCPSocket()
{
	delete lpRecvData;
	lpRecvData = nullptr;
	
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

int TCPSocket::Bind(const char* szInPortAddr)
{
	sockAddr.SetAddress(szInPortAddr);

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
	sockAddr.SetAddress(szInPortAddr);

	int iConnectResult = connect(hSock, sockAddr.GetAsSockAddr(), sockAddr.GetSize());
	if (iConnectResult >= 0)
		return NO_ERROR;
	return WSAGetLastError();
}

void TCPSocket::Closesocket()
{
	closesocket(hSock);
}

void TCPSocket::Shutdownsocket(int i)
{
	shutdown(hSock, i);
}

int TCPSocket::IOCPRecv()
{
	if (Recving == false)
	{
		DWORD dwFlag = 0;
		Recving = true;
		lpRecvData->wsaBuf.len = MAXBUFFERSIZE;
		
		if (WSARecv(hSock, &lpRecvData->wsaBuf, 1, nullptr, &dwFlag, lpRecvData, nullptr) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				return WSAGetLastError();
		}

		++iIOReference;
	}
	lpRecvData->eIOMode = IOMODE::E_IOMODE_RECV;
	return 0;
}

int TCPSocket::IOCPSend(const char* szInStream, int iInSendSize)
{
	//memcpy(lpSendData->szBuffer, szInStream, iInSendSize);
	memcpy(lpRecvData->szBuffer, szInStream, iInSendSize);

	DWORD dwRecvByte = 0;
	DWORD dwFlag = 0;

	if (WSASend(hSock, &lpRecvData->wsaBuf, 1, &dwRecvByte, dwFlag, lpRecvData, nullptr) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			return WSAGetLastError();
	}

	lpRecvData->eIOMode = IOMODE::E_IOMODE_SEND;
	++iIOReference;

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
	return lpRecvData->eIOMode;
}

char* TCPSocket::GetBufferData()
{
	return lpRecvData->szBuffer;
}
