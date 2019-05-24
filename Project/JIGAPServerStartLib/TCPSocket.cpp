#include "pch.h"
#include "TCPSocket.h"
#include "SocketAddress.h"
#include "TCPIOData.h" 

TCPSocket::TCPSocket()
	:jigapState(JIGAPSTATE::E_LOGIN), hSock(NULL), lpIOData(new TCPIOData), lpRoom(nullptr)
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

TCPSocket* TCPSocket::Accept()
{	
	SocketAddress addr;
	int size = addr.GetSize();

	SOCKET sock = accept(hSock, addr.GetAsSockAddr(), &size);
	if (sock != INVALID_SOCKET)
		return new TCPSocket(sock, addr);
	return nullptr;
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

HANDLE TCPSocket::ConnectionCompletionPort(HANDLE hPortHandle)
{
	HANDLE hHandle = CreateIoCompletionPort((HANDLE)hSock, hPortHandle, (ULONG_PTR)this, NULL);
	
	if (hHandle == nullptr)
		return nullptr;
	
	return hHandle;
}

int TCPSocket::IOCPRecv()
{
	DWORD dwFlag = 0;

	if (WSARecv(hSock, &lpIOData->wsaBuf, 1, nullptr, &dwFlag, lpIOData, nullptr) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			return WSAGetLastError();
	}

	lpIOData->eIOMode = IOMODE::E_IOMODE_RECV;

	return 0;
}

int TCPSocket::IOCPSend()
{
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

void TCPSocket::ReadBufferClear()
{
	char temp_char;
	unsigned long temp_long = 0;
	long i;

	if (ioctlsocket(hSock, FIONREAD, &temp_long) != SOCKET_ERROR)
	{
		for (i = 0; i < temp_long; i++)
		{
			recv(hSock, &temp_char, sizeof(char), 0);
		}
	}
}

const IOMODE& TCPSocket::GetIOMode()
{
	return lpIOData->eIOMode;
}

char* TCPSocket::GetBufferData()
{
	return lpIOData->szBuffer;
}

void TCPSocket::WriteBuffer(const char* message)
{
	memcpy(lpIOData->szBuffer, message, sizeof(lpIOData->szBuffer));
}

void TCPSocket::ClearBuffer()
{
	memset(lpIOData->szBuffer, NULL, sizeof(lpIOData->szBuffer));
}

void TCPSocket::SetUserName(const std::string& name)
{
	strUserName = name;
	jigapState = JIGAPSTATE::E_NOTROOM;
}

void TCPSocket::UnJoinedRoom()
{
	lpRoom->DeleteUser(this);
	lpRoom = nullptr;

	jigapState = JIGAPSTATE::E_NOTROOM;
}

void TCPSocket::JoinedRoom(Room* inRoom)
{
	lpRoom = inRoom;
	lpRoom->AddUser(this);
	
	jigapState = JIGAPSTATE::E_ROOM;
}

