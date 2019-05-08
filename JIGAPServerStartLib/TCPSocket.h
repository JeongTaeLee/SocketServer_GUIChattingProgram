#pragma once

class SocketAddress;

class TCPSocket
{
private:
	SOCKET hSock;
	SocketAddress sockAddr;

public:	
	TCPSocket();
	TCPSocket(SOCKET InSocket);
	TCPSocket(SOCKET hInSock, const SocketAddress& InAddr);
	~TCPSocket();

	int Bind(const char * szInIpAddr, const char * szInPortAddr);
	int Listen(int inBackLog);
	TCPSocket * Accept();
	int Connect();

};

