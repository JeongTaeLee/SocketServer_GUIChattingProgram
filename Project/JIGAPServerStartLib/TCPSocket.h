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

	/*IOCP TCP Socket을 다른 소켓과 연결합니다..
	성공 : TCPSocket의 포인터
	실패  : NULL*/
	virtual TCPSocket * Accept();

	/*IOCP TCP Socket을 서버와 연결합니다.
	성공 : 0
	실패  : WSAGetLastError()*/
	int Connect(const char* szInIpAddr, const char* szInPortAddr);

	void Closesocket();
public:
	/*IOCP Socket을 Completion Port에 연결합니다.
	성공 : Handle
	실패 : NULL
	*/
	HANDLE ConnectionCompletionPort(HANDLE hPortHandle);

	/*IOCP 방식로 데이터를 수신합니다.
	성공 : 0
	실패 : WSAGetLastError()
	*/
	int IOCPRecv();

	/*IOCP 방식로 데이터를 발신합니다.
	성공 : 0.
	실패 : WSAGetLastError()
	*/
	int IOCPSend();
	
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

