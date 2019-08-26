#pragma once

class TCPSocket;

class SocketAddress
{
public:
	sockaddr_in sockAddrIn;

	sockaddr* GetAsSockAddr();

public:
	virtual ~SocketAddress();

	void SetAddress(const char* InPortAddr);
	void SetAddress(const SocketAddress& InSockAddrIn);

	int GetSize() const { return sizeof(sockAddrIn); }
	
	
};

