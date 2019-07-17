#pragma once

#include "TCPIOData.h"
#include "SocketAddress.h"

enum IOMODE;
class TCPIOData;
class Room;

class TCPSocket
{
protected:	 
	SOCKET hSock;
	SocketAddress sockAddr;

	TCPIOData * lpIOData;

public:	
	TCPSocket();
	TCPSocket(SOCKET InSocket);
	TCPSocket(SOCKET hInSock, const SocketAddress& InAddr);
	virtual ~TCPSocket();

	/*IOCP TCP Socket�� �����մϴ�
	���� : 0
	����  : WSAGetLastError()*/
	int IOCPSocket();

	/*���� TCP Socket�� �����մϴ�
	���� : 0
	����  : WSAGetLastError()*/
	int SYNCSocket();
	
	/*IOCP TCP Socket�� ���ε�(�Ҵ�)�մϴ�.
	���� : 0
	����  : WSAGetLastError()*/
	int Bind(const char * szInIpAddr, const char * szInPortAddr);

	/*IOCP TCP Socket�� ��⿭�� �����մϴ�.
	���� : 0
	����  : WSAGetLastError()*/
	int Listen(int inBackLog);\

	/*
	* IOCP TCP Socket�� �ٸ� ���ϰ� �����մϴ�..
	* ���� : TCPSocket�� ������
	* ����  : NULL
	* �� �Լ��� ���ø��� ����� ����? TCPSocket ��ӹ޴�
	* ��� �ڽ��Լ��� Ÿ������ ��ȯ�ϱ� �����Դϴ�.
	*/
	template <class T>
	T* Accept()
	{
		SocketAddress addr;
		int size = addr.GetSize();

		SOCKET sock = accept(hSock, addr.GetAsSockAddr(), &size);
		if (sock != INVALID_SOCKET)
			return new T(sock, addr);
		return nullptr;
	}

	/*IOCP TCP Socket�� ������ �����մϴ�.
	���� : 0
	����  : WSAGetLastError()*/
	int Connect(const char* szInIpAddr, const char* szInPortAddr);

	void Closesocket();
public:
	/*IOCP ��ķ� �����͸� �����մϴ�.
	���� : 0
	���� : WSAGetLastError()
	*/
	int IOCPRecv();

	/*IOCP ��ķ� �����͸� �߽��մϴ�.
	���� : 0.
	���� : WSAGetLastError()
	*/
	int IOCPSend(const char * szInStream, int iInSendSize);
	
	/*���� ��ķ� �����͸� �����մϴ�.
	���� : ���ŵ� ������ ũ��
	���� : -1
	*/
	int SYNCRecv(char * szInBuf, int iInBufSize = MAXBUFFERSIZE);

	/*���� ��ķ� �����͸� �߽��մϴ�.
	���� : �߽ŵ� ������ ũ��
	���� : -1
	*/
	int SYNCSend(const char* szInBuf, int iInBufSize = MAXBUFFERSIZE);

public:
	const SOCKET& GetSocket() { return hSock; }

	const IOMODE& GetIOMode();
	char* GetBufferData();
};

using LPTCPSOCK = TCPSocket*;

