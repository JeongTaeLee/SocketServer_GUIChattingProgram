#pragma once

#include "SocketAddress.h"

class TCPIOData;

class TCPSocket
{
private:
	SOCKET hSock;
	SocketAddress sockAddr;

	LPIODATA lpIOData;
public:	
	TCPSocket();
	TCPSocket(SOCKET InSocket);
	TCPSocket(SOCKET hInSock, const SocketAddress& InAddr);
	~TCPSocket();

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

	/*IOCP TCP Socket�� �ٸ� ���ϰ� �����մϴ�..
	���� : TCPSocket�� ������
	����  : NULL*/
	TCPSocket * Accept();

	/*IOCP TCP Socket�� ������ �����մϴ�.
	���� : 0
	����  : WSAGetLastError()*/
	int Connect(const char* szInIpAddr, const char* szInPortAddr);

	void Closesocket();

public:
	/*IOCP Socket�� Completion Port�� �����մϴ�.
	���� : Handle
	���� : NULL
	*/
	HANDLE ConnectionCompletionPort(HANDLE hPortHandle);

	/*IOCP ��ķ� �����͸� �����մϴ�.
	���� : 0
	���� : WSAGetLastError()
	*/
	int IOCPRecv();

	/*IOCP ��ķ� �����͸� �߽��մϴ�.
	���� : 0.
	���� : WSAGetLastError()
	*/
	int IOCPSend();
	
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
	const IOMODE& GetIOMode();
	char* GetBufferData();
	void WriteBuffer(const char * message);
	void ClearBuffer();

public:
	const SOCKET& GetSocket() { return hSock; }
	const LPIODATA GetIOData() { return lpIOData; }
public:

};

using LPTCPSOCK = TCPSocket*;

