#include "pch.h"
#include "SocketAddress.h"


sockaddr* SocketAddress::GetAsSockAddr()
{
	/*sockaddr_in * �������� �ּҰ��� ��ȯ�մϴ�. ���� ũ�Ⱑ ���� ������ �����մϴ�.*/
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
