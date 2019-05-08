#include "pch.h"
#include "TCPSocket.h"
#include "SocketAddress.h"

TCPSocket::TCPSocket()
{
}

TCPSocket::TCPSocket(SOCKET InSocket)
	:hSock(InSocket)
{
}

TCPSocket::TCPSocket(SOCKET hInSock, const SocketAddress& InAddr)
	:hSock(hInSock)
{
	sockAddr.SetAddress(InAddr);
}

TCPSocket::~TCPSocket()
{
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

TCPSocket* TCPSocket::Accept()
{	
	SocketAddress addr;
	int size = addr.GetSize();


	SOCKET sock = accept(hSock, addr.GetAsSockAddr(), &size);
	if (sock == INVALID_SOCKET)
		return nullptr;

	return new TCPSocket(sock, addr);
}

int TCPSocket::Connect()
{
	int iConnectResult = connect(hSock, sockAddr.GetAsSockAddr(), sockAddr.GetSize());
	if (iConnectResult >= 0)
		return NO_ERROR;

	return WSAGetLastError();
}
