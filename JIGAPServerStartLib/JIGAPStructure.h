#pragma once


typedef struct HANDLE_DATA
{
	SOCKET hSock;
	SOCKADDR_IN SockAddr;

	HANDLE_DATA();
	~HANDLE_DATA();

	
} *LPHANDLE_DATA;