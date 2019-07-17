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

	/*IOCP TCP Socket을 생성합니다
	성공 : 0
	실패  : WSAGetLastError()*/
	int IOCPSocket();

	/*동기 TCP Socket을 생성합니다
	성공 : 0
	실패  : WSAGetLastError()*/
	int SYNCSocket();
	
	/*IOCP TCP Socket을 바인드(할당)합니다.
	성공 : 0
	실패  : WSAGetLastError()*/
	int Bind(const char * szInIpAddr, const char * szInPortAddr);

	/*IOCP TCP Socket의 대기열을 생성합니다.
	성공 : 0
	실패  : WSAGetLastError()*/
	int Listen(int inBackLog);\

	/*
	* IOCP TCP Socket을 다른 소켓과 연결합니다..
	* 성공 : TCPSocket의 포인터
	* 실패  : NULL
	* 이 함수에 템플릿을 사용한 이유? TCPSocket 상속받는
	* 모든 자식함수의 타입으로 반환하기 위함입니다.
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

	/*IOCP TCP Socket을 서버와 연결합니다.
	성공 : 0
	실패  : WSAGetLastError()*/
	int Connect(const char* szInIpAddr, const char* szInPortAddr);

	void Closesocket();
public:
	/*IOCP 방식로 데이터를 수신합니다.
	성공 : 0
	실패 : WSAGetLastError()
	*/
	int IOCPRecv();

	/*IOCP 방식로 데이터를 발신합니다.
	성공 : 0.
	실패 : WSAGetLastError()
	*/
	int IOCPSend(const char * szInStream, int iInSendSize);
	
	/*동기 방식로 데이터를 수신합니다.
	성공 : 수신된 데이터 크기
	실패 : -1
	*/
	int SYNCRecv(char * szInBuf, int iInBufSize = MAXBUFFERSIZE);

	/*동기 방식로 데이터를 발신합니다.
	성공 : 발신된 데이터 크기
	실패 : -1
	*/
	int SYNCSend(const char* szInBuf, int iInBufSize = MAXBUFFERSIZE);

public:
	const SOCKET& GetSocket() { return hSock; }

	const IOMODE& GetIOMode();
	char* GetBufferData();
};

using LPTCPSOCK = TCPSocket*;

