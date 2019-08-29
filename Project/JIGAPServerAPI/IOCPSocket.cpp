#include "pch.h"
#include "IOCPSocket.h"
#include "IOCPIOData.h"

JIGAPServerAPI::IOCPSocket::IOCPSocket()
{
	ZeroMemory(&wsaData, sizeof(wsaData));
}

JIGAPServerAPI::IOCPSocket::IOCPSocket(SOCKET inSock)
	:BaseSocket((unsigned __int64)(hSocket = inSock))
{
	ZeroMemory(&wsaData, sizeof(wsaData));
}

bool JIGAPServerAPI::IOCPSocket::InitializeSocket()
{
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if (result != 0)
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Initialize] Failed WSAStartup");
	
	hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (hSocket == INVALID_SOCKET)
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Initialize] Failed WSASocket");

	BaseSocket::SetIdNumber((unsigned __int64)hSocket);

	return true;
}

void JIGAPServerAPI::IOCPSocket::CloseSocket()
{
	closesocket(hSocket);
}

bool JIGAPServerAPI::IOCPSocket::Bind(unsigned short inPort)
{
	sockaddr_in sockAddr;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons(inPort);

	int bindResult = bind(hSocket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr));

	if (bindResult >= 0)
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Bind] Failed Bind");

	return true;
}

bool JIGAPServerAPI::IOCPSocket::Listen(int inQueueCount)
{
	int listenResult = listen(hSocket, inQueueCount);

	if (listenResult >= 0)
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Listen] Failed Listen");

	return true;
}

bool JIGAPServerAPI::IOCPSocket::Connect(const std::wstring& inIp, unsigned short inPort)
{
	std::string conversionIp;
	conversionIp.assign(inIp.begin(), inIp.end());

	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(conversionIp.c_str());
	sockAddr.sin_port = inPort;

	int connectResult = connect(hSocket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr));

	if (connectResult >= 0)
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Connect] Failed Connect");
	
	return true;
}

JIGAPServerAPI::BaseSocket* JIGAPServerAPI::IOCPSocket::Accept()
{
	sockaddr_in sockAddr;

	int addrSize = sizeof(sockAddr);

	SOCKET socket = accept(hSocket, reinterpret_cast<sockaddr*>(&sockAddr), &addrSize);

	if (socket == INVALID_SOCKET)
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Accept] Failed Accept");

	return new IOCPSocket(socket);
}

void JIGAPServerAPI::IOCPSocket::Recv()
{
	if (bRecving == false)
	{
		DWORD dwFlag = 0;
		bRecving = true;
		iocpIoData->wsaBuf.len = MAXBUFFERLENGTH;

		if (WSARecv(hSocket, &iocpIoData->wsaBuf, 1, nullptr, &dwFlag, iocpIoData, nullptr) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Recv] Failed Recv");
		}

		++iSocketReferenceCount;
	}
	iocpIoData->eIoMode = IOMODE::E_IOMODE_RECV;
}

void JIGAPServerAPI::IOCPSocket::Send(const char *inSendBuffer, int inBufferSize)
{
	//memcpy(lpSendData->szBuffer, szInStream, iInSendSize);
	memcpy(iocpIoData->szBuffer, inSendBuffer, inBufferSize);

	DWORD dwRecvByte = 0;
	DWORD dwFlag = 0;

	if (WSASend(hSocket, &iocpIoData->wsaBuf, 1, &dwRecvByte, dwFlag, iocpIoData, nullptr) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Send] Failed Send");
	}

	iocpIoData->eIoMode = IOMODE::E_IOMODE_SEND;
	++iSocketReferenceCount;
}

