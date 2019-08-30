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

JIGAPServerAPI::IOCPSocket::~IOCPSocket()
{
	delete iocpIoData;
	iocpIoData = nullptr;
}

int JIGAPServerAPI::IOCPSocket::InitializeSocket()
{
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if (result != 0)
	{
#ifdef _DEBUG
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Initialize] Failed WSAStartup");
#else 
		return WSAGetLastError();
#endif

	}
	
	hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (hSocket == INVALID_SOCKET)
	{
#ifdef _DEBUG
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Initialize] Failed WSASocket");
#else
		return WSAGetLastError();
#endif
	}

	BaseSocket::SetIdNumber((unsigned __int64)hSocket);

	return 0;
}

void JIGAPServerAPI::IOCPSocket::CloseSocket()
{
	closesocket(hSocket);
}

int JIGAPServerAPI::IOCPSocket::Bind(unsigned short inPort)
{
	sockaddr_in sockAddr;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons(inPort);

	int bindResult = bind(hSocket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr));

	if (bindResult >= 0)
	{
#ifdef _DEBUG
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Bind] Failed Bind");
#else
		return WSAGetLastError();
#endif
	}

	return 0;
}

int JIGAPServerAPI::IOCPSocket::Listen(int inQueueCount)
{
	int listenResult = listen(hSocket, inQueueCount);

	if (listenResult >= 0)
	{
#ifdef _DEBUG
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Listen] Failed Listen");
#else
		return WSAGetLastError();
#endif
	}

	return 0;
}

int JIGAPServerAPI::IOCPSocket::Connect(const std::wstring& inIp, unsigned short inPort)
{
	std::string conversionIp;
	conversionIp.assign(inIp.begin(), inIp.end());

	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(conversionIp.c_str());
	sockAddr.sin_port = inPort;

	int connectResult = connect(hSocket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr));

	if (connectResult >= 0)
	{
#ifdef _DEBUG
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Connect] Failed Connect");
#else
		return WSAGetLastError();
#endif
	}
	
	return 0;
}

JIGAPServerAPI::BaseSocket* JIGAPServerAPI::IOCPSocket::Accept()
{
	sockaddr_in sockAddr;

	int addrSize = sizeof(sockAddr);

	SOCKET socket(accept(hSocket, reinterpret_cast<sockaddr*>(&sockAddr), &addrSize));

	if (socket == INVALID_SOCKET)
	{
#ifdef _DEBUG
		throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Accept] Failed Accept");
#else
		return nullptr;
#endif
	}

	return new IOCPSocket(socket);
}

int JIGAPServerAPI::IOCPSocket::Recv()
{
	if (bRecving == false)
	{
		DWORD dwFlag = 0;
		bRecving = true;
		iocpIoData->wsaBuf.len = MAXBUFFERLENGTH;

		if (WSARecv(hSocket, &iocpIoData->wsaBuf, 1, nullptr, &dwFlag, iocpIoData, nullptr) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
#ifdef _DEBUG
				throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Recv] Failed Recv");
#else
				return WSAGetLastError();
#endif 
			}
		}

		++iSocketReferenceCount;
	}
	iocpIoData->eIoMode = IOMODE::E_IOMODE_RECV;

	return 0;
}

int JIGAPServerAPI::IOCPSocket::Send(const char *inSendBuffer, int inBufferSize)
{
	memcpy(iocpIoData->szBuffer, inSendBuffer, inBufferSize);

	DWORD dwRecvByte = 0;
	DWORD dwFlag = 0;

	if (WSASend(hSocket, &iocpIoData->wsaBuf, 1, &dwRecvByte, dwFlag, iocpIoData, nullptr) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
#ifdef _DEBUG
			throw SException(WSAGetLastError(), L"[JIGAPServerAPI::IOCPSocket::Send] Failed Send");
#else
			return WSAGetLastError();
#endif
		}
	}

	iocpIoData->eIoMode = IOMODE::E_IOMODE_SEND;
	++iSocketReferenceCount;

	return 0;
}

