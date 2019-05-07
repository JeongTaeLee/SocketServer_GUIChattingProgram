// JIGAPServer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "JIGAPServer.h"



JIGAPServer::JIGAPServer()
	:lpServData(nullptr), bServerOn(false)
{
}

JIGAPServer::~JIGAPServer()
{
}

HRESULT JIGAPServer::InitializeServer()
{
	lpServData = new HANDLE_DATA;

	/*Winsock을 시작합니다*/
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return E_FAIL;

	/*WSASocket을 생성합니다.*/
	lpServData->hSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (lpServData->hSock == INVALID_SOCKET)
		return E_FAIL;

	PrintSystemLog("소켓이 성공적으로 생성되었습니다.");

	/*Socket을 Bind 합니다*/
	lpServData->SockAddr.sin_family = PF_INET;
	lpServData->SockAddr.sin_addr.s_addr = inet_addr(szIpAddr.c_str());
	lpServData->SockAddr.sin_port = atoi(szPortAddr.c_str());
	if (bind(lpServData->hSock, (SOCKADDR*)& lpServData->SockAddr, sizeof(lpServData->SockAddr)) == SOCKET_ERROR)
		return E_FAIL;

	PrintSystemLog("소켓이 성공적으로 할당되었습니다.");

	/*연결 대기열을 생성합니다*/
	if (listen(lpServData->hSock, 10) == SOCKET_ERROR)
		return E_FAIL;
	
	PrintSystemLog("서버의 연결대기열이 생성되었습니다.");

	hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);

	if (hCompletionHandle == nullptr)
		return E_FAIL;
	
	PrintSystemLog("서버의 CompletionPort가 생성되었습니다.");

	
	/*메모리 동시접근 버그를 잡기위한 WinAPI 뮤텍스 생성입니다.*/
	hSystemLogMutex = CreateMutex(0, FALSE, NULL);
	hThreadMutex = CreateMutex(0, FALSE, NULL);
	return S_OK;
}

void JIGAPServer::ReleaseServer()
{
	/*현재 서버에 작동 여부를 확인하는 bool 변수를 여러 쓰레드에 동시 접근을 막기 위해 Mutex를 사용한 부분입니다.*/
	WaitForSingleObject(hThreadMutex, INFINITE);
	bServerOn = false;
	ReleaseMutex(hThreadMutex);

	Sleep(100);
	if (chattingThread.joinable())
		chattingThread.join();
	Sleep(100);
	if (connectThread.joinable())
		connectThread.join();
	
	/*생성한 WInAPI Mutex를 해제합니다.*/
	CloseHandle(hSystemLogMutex);
	CloseHandle(hThreadMutex);

	for (auto Iter : liHandleData)
	{
		shutdown(Iter->hSock, SD_BOTH);
		closesocket(Iter->hSock);

		delete Iter->lpIOData;
		delete Iter;
	}

	WSACleanup();

	delete lpServData;
	lpServData = nullptr;
}

void JIGAPServer::ConnectThread()
{
	PrintSystemLog("서버의 연결 담당 Thread가 작동되었습니다.");
	

	while (true)
	{
		/*현재 서버에 작동 여부를 확인하는 bool 변수에 동시 접근을 막기 위해 Mutex를 사용한 부분입니다.*/
		WaitForSingleObject(hThreadMutex, INFINITE);
	
		LPIO_DATA  lpIOData = new IO_DATA;
		
		LPHANDLE_DATA lpClntData = new HANDLE_DATA;
		
		int iAddrSize = sizeof(lpClntData->SockAddr);
		lpClntData->hSock = accept(lpServData->hSock, (SOCKADDR*)& lpClntData->SockAddr, &iAddrSize);
		lpClntData->lpIOData = lpIOData;

		if (!bServerOn)
			break;
		
		/*소켓에 연결을 실패 했을 경우*/
		if (lpClntData->hSock == INVALID_SOCKET)
		{
			closesocket(lpClntData->hSock);
			delete lpClntData;
			PrintSystemLog("연결을 시도하던 소켓과의 연결이 실패하였습니다");
			continue;
		}
		
		/*CompletionPort에 해당 소켓을 연결합니다*/
		if (CreateIoCompletionPort((HANDLE)lpClntData->hSock, hCompletionHandle, (DWORD_PTR)lpClntData, 0) == NULL)
		{
			closesocket(lpClntData->hSock);
			delete lpClntData;
			PrintSystemLog("CompletionPort에 연결을 시도하던 소켓을 할당하지 못했습니다.");
			continue;
		}
		
		char message[128];
		sprintf(message, "%d 소켓과 연결되었습니다.", lpClntData->hSock);
		PrintSystemLog(message);
		
		/*Message를 받기 위한 부분입니다.*/
		DWORD Flag = 0;
		DWORD dwRecvBytes = 0;
		WSARecv(lpClntData->hSock, &lpIOData->wsaBuf, 1, &dwRecvBytes, &Flag, (OVERLAPPED*)&lpIOData, NULL);
		
		ReleaseMutex(hThreadMutex);
	}


	PrintSystemLog("서버의 연결 Thread가 해제되었습니다.");
}

void JIGAPServer::ChattingThread()
{
	PrintSystemLog("서버의 채팅 담당 Thread가 작동되었습니다.");

	while (true)
	{
		/*현재 서버에 작동 여부를 확인하는 bool 변수에 동시 접근을 막기 위해 Mutex를 사용한 부분입니다.*/
		WaitForSingleObject(hThreadMutex, INFINITE);

		if (!bServerOn)
			break;

		LPHANDLE_DATA lpHandle = nullptr;
		LPIO_DATA lpIO = nullptr;

		DWORD dwByte = 0;
		DWORD dwFlag = 0;

		/*메시지 입출력이 완료된 소켓을 얻어옵니다. 없을 경우 대기합니다.*/
		GetQueuedCompletionStatus(hCompletionHandle,
			&dwByte,
			(LPDWORD)& lpHandle, // 입출력이 완려된 소켓의 데이터입니다.
			(LPOVERLAPPED*)& lpIO, 
			INFINITE);

		if (dwByte == 0)
		{
			shutdown(lpHandle->hSock, SD_BOTH);
			closesocket(lpHandle->hSock);

			delete lpHandle;
			delete lpIO;
			continue;
		}
		
		/*소켓에서 보낸 데이터를 모두 받았다.*/
		if (lpIO->eOvlmode == E_OVLMODE_RECV)
		{
			/*모든 유저에게 메시지를 보냅니다 서버에 먼저 수신된 메시지를 전송합니다.*/
			for (auto Iter : liHandleData)
			{
				DWORD dwSendbyte = 0;

				memcpy(Iter->lpIOData->szBuffer, lpHandle->lpIOData->szBuffer, sizeof(MAXBUFFERSIZE));
				Iter->lpIOData->eOvlmode = E_OVLMODE_SEND;

				WSASend(Iter->hSock,
					&Iter->lpIOData->wsaBuf,
					1,
					&dwSendbyte,
					dwFlag,
					(OVERLAPPED*)Iter->lpIOData,
					nullptr);
			}
		}
		else
		{
			DWORD dwRecvByte = 0;

			lpHandle->lpIOData->eOvlmode = E_OVLMODE_RECV;
			WSARecv(lpHandle->hSock,
				&lpHandle->lpIOData->wsaBuf,
				1,
				&dwRecvByte,
				&dwFlag,
				(OVERLAPPED*)lpHandle->lpIOData,
				nullptr);
		}


		ReleaseMutex(hThreadMutex);
	}

	PrintSystemLog("서버의 채팅 Thread가 해제되었습니다.");
}


bool JIGAPServer::JIGAPServerOpen(std::string _szIpAddr, std::string _szPortAddr)
{
	szIpAddr = _szIpAddr;
	szPortAddr = _szPortAddr;

	if (FAILED(JIGAPServer::InitializeServer()))
	{
		PrintSystemLog("서버를 열 수 없습니다!");
		return false;
	}

	/*현재 서버에 작동 여부를 확인하는 bool 변수에 동시 접근을 막기 위해 Mutex를 사용한 부분입니다.*/
	WaitForSingleObject(hThreadMutex, INFINITE);
	bServerOn = true;
	ReleaseMutex(hThreadMutex);

	connectThread = std::thread([&]() { ConnectThread(); });
	Sleep(100);
	chattingThread = std::thread([&]() { ChattingThread(); });
	Sleep(100);

	PrintSystemLog("서버를 열었습니다!");
	return true;
}

void JIGAPServer::JIGAPServerClose()
{
	bServerOn = false;
	ReleaseServer();

	PrintSystemLog("서버가 닫혔습니다.");
}

void JIGAPServer::PrintSystemLog(const std::string& key)
{
	/*Mutex를 소유합니다. 해당핸들은 non-signaled 상태가 됩니다 반면 이미 핸들이 Non-signaled 상태이면 블록킹합니다. */
	WaitForSingleObject(hSystemLogMutex, INFINITE);
	qSystemMsg.push(key);
	/*Mutex 소유하지 않게 바꿔줍니다. 뮤텍스를 signaled 상태로 바꿉니다*/
	ReleaseMutex(hSystemLogMutex);
}

std::string JIGAPServer::JIGAPGetSystemMsg()
{
	/*가장 오래된 메시지를 가져옵니다.*/
	WaitForSingleObject(hSystemLogMutex, INFINITE);
	std::string strSystemMessage = qSystemMsg.front();

	/*가장 오래된 메시지를 Queue에서 지웁니다.*/
	qSystemMsg.pop();
	ReleaseMutex(hSystemLogMutex);
	return strSystemMessage;
}
