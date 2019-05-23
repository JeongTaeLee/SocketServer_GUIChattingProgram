#include "pch.h"
#include "SocketAddress.h"


sockaddr* SocketAddress::GetAsSockAddr()
{
	/*sockaddr_in * 형식으로 주소값을 변환합니다. 서로 크기가 같기 때문에 가능합니다.*/
	return reinterpret_cast<sockaddr*>(&mSockAddrIn);
}

SocketAddress::SocketAddress()
{
}

SocketAddress::~SocketAddress()
{
}

void SocketAddress::SetAddress(const char* InIpAddr, const char* InPortAddr)
{
	mSockAddrIn.sin_family = AF_INET;
	mSockAddrIn.sin_addr.s_addr = inet_addr(InIpAddr);
	mSockAddrIn.sin_port = htons(atoi(InPortAddr));
}

void SocketAddress::SetAddress(const SocketAddress& InSockAddrIn)
{
	memcpy(&mSockAddrIn, &InSockAddrIn, sizeof(mSockAddrIn));
}
