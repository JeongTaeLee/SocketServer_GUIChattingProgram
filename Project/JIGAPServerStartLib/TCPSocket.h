#pragma once

#include "SocketAddress.h"

class TCPIOData;
class Room;

class TCPSocket
{
protected:	 
	SOCKET hSock;
	SocketAddress sockAddr;

	LPIODATA lpIOData;
	
	Room * lpRoom;

	std::string strUserName;
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

	/*IOCP TCP Socket�� �ٸ� ���ϰ� �����մϴ�..
	���� : TCPSocket�� ������
	����  : NULL*/
	virtual TCPSocket * Accept();

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
	const SOCKET& GetSocket() { return hSock; }

	const IOMODE& GetIOMode() {	return lpIOData->eIOMode; }
	char* GetBufferData() { return lpIOData->szBuffer; }
	void SetBufferData(const char* copy, int size);

	const std::string& GetMyUserName() { return strUserName; }
	void SetUserName(const std::string& name);

public:
	Room*& GetRoom() { return lpRoom; }
	void SetRoom(Room* room) { lpRoom = room; }
};

using LPTCPSOCK = TCPSocket*;

