#include "pch.h"
#include "SocketAddress.h"


sockaddr* SocketAddress::GetAsSockAddr()
{
	/*sockaddr_in * �������� �ּҰ��� ��ȯ�մϴ�. ���� ũ�Ⱑ ���� ������ �����մϴ�.*/
	return reinterpret_cast<sockaddr*>(&sockAddrIn);
}

SocketAddress::SocketAddress()
{
}

SocketAddress::~SocketAddress()
{
}

void SocketAddress::SetAddress(const char* InIpAddr, const char* InPortAddr)
{
	sockAddrIn.sin_family = AF_INET;
	sockAddrIn.sin_addr.s_addr = inet_addr(InIpAddr);
	sockAddrIn.sin_port = htons(atoi(InPortAddr));
}

void SocketAddress::SetAddress(const SocketAddress& InSockAddrIn)
{
	memcpy(&sockAddrIn, &InSockAddrIn, sizeof(sockAddrIn));
}
