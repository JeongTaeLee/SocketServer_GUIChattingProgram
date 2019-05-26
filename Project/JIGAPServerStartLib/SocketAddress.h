#pragma once

class TCPSocket;

class SocketAddress
{
private:
	sockaddr_in sockAddrIn;

	sockaddr* GetAsSockAddr();

public:
	SocketAddress();
	~SocketAddress();

	void SetAddress(const char* InIpAddr, const char* InPortAddr);
	void SetAddress(const SocketAddress& InSockAddrIn);

	int GetSize() const { return sizeof(sockAddrIn); }
	
	friend TCPSocket;
};

