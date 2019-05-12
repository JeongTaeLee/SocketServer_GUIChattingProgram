// JIGAPServer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "JIGAPServer.h"



JIGAPServer::JIGAPServer()
	:lpServSock(nullptr), bServerOn(false)
{
}

JIGAPServer::~JIGAPServer()
{
}

HRESULT JIGAPServer::JIGAPInitializeServer()
{
	int iErrorCode = 0;

	lpServSock = new TCPSocket;

	/*Winsock을 시작합니다*/
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return E_FAIL;

	/*WSASocket을 생성합니다.*/
	if ( (iErrorCode = lpServSock->IOCPSocket()) ) 
	{
		JIGAPPrintSystemLog("socket Error! Code : %d, Faild Create Socket", iErrorCode);
		return E_FAIL;
	}

	JIGAPPrintSystemLog("Socket Created");

	/*Socket을 Bind 합니다*/
	if ( (iErrorCode = lpServSock->Bind(szIpAddr.c_str(), szPortAddr.c_str())) )
	{
		JIGAPPrintSystemLog("bind Error! Code : %d, Faild Bind Socket", iErrorCode);
		return E_FAIL;
	}

	JIGAPPrintSystemLog("Socket Binded");

	/*연결 대기열을 생성합니다*/
	if ( (iErrorCode = lpServSock->Listen(10)) )
	{
		JIGAPPrintSystemLog("listen Error! Code : %d", iErrorCode);
		return E_FAIL;
	}
	
	JIGAPPrintSystemLog("Socket Listen");

	hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
	if (hCompletionHandle == nullptr)
	{
		JIGAPPrintSystemLog("CreateioCompletionPort Error! Code : %d, Failed Create CompletionPort", iErrorCode);
		return E_FAIL;
	}
	
	JIGAPPrintSystemLog("CompletionPort Created!");

	/*메모리 동시접근 버그를 잡기위한 WinAPI 뮤텍스 생성입니다.*/
	return S_OK;
}

void JIGAPServer::JIGAPReleaseServer()
{
	lpServSock->Closesocket();
	CloseHandle(hCompletionHandle);

	for (auto Iter : liClientData)
	{
		Iter->Closesocket();
		delete Iter;
	}
	liClientData.clear();

	delete lpServSock;
	lpServSock = nullptr;

	WSACleanup();
}

void JIGAPServer::JIGAPConnectThread()
{
	int iErrorCode = 0;

	JIGAPPrintSystemLog("Active Connect Thread!");	

	while (true)
	{
		/*연결 대기 합니다.*/
		LPTCPSOCK lpClntData = lpServSock->Accept();

		if (bServerOn == false)
			break;
		
		/*소켓에 연결을 실패 했을 경우*/
		if (lpClntData == nullptr)
		{
			JIGAPPrintSystemLog("Accept Error! Faild Connected");
			continue;
		}
		
		/*연결된 소켓에 CompletionPort 연결 합니다.*/
		if (lpClntData->ConnectionCompletionPort(hCompletionHandle) == NULL)
		{
			JIGAPPrintSystemLog("CompletionPort Connection Error! Code : %d, Socket : %d", WSAGetLastError(), lpClntData->GetSocket());

			lpClntData->Closesocket();
			delete lpClntData;
			continue;
		}

		JIGAPPrintSystemLog("Connected Socket : %d", lpClntData->GetSocket());
		
		/*연결된 소켓을 Client list에 추가합니다.*/
		liClientData.push_back(lpClntData);

		/*연결된 소켓의 메시지를 받을 준비를 합니다.*/
		if ((iErrorCode = lpClntData->IOCPRecv()))
		{
			JIGAPPrintSystemLog("WSARecv Error! Code : %d, Socket : %d", WSAGetLastError(), lpClntData->GetSocket());

			lpClntData->Closesocket();
			delete lpClntData;
		}


	}

	JIGAPPrintSystemLog("Unactive Connect Thread!");
}

void JIGAPServer::JIGAPChattingThread()
{
	JIGAPPrintSystemLog("Active Chatting Thread!");

	while (true)
	{
		LPTCPSOCK lpClntSock = nullptr;
		LPIODATA lpIOData = nullptr;

		DWORD dwByte = 0;

		/*메시지 입출력이 완료된 소켓을 얻어옵니다. 없을 경우 대기합니다.*/
		GetQueuedCompletionStatus(hCompletionHandle,
			&dwByte,
			(LPDWORD)& lpClntSock, // 입출력이 완려된 소켓의 데이터입니다.
			(LPOVERLAPPED*)& lpIOData,
			INFINITE);

		if (dwByte == 0)
		{
			/*서버가 종료되어 0이 들어왔을때.*/
			if (bServerOn == false)
				break;
			else
			{
				/*클라이언트가 종료 요청을 할때 실행되는 구문입니다*/
				lpClntSock->Closesocket();

				JIGAPPrintSystemLog("unconnected Socket : %d", lpClntSock->GetSocket());

				liClientData.remove(lpClntSock);

				delete lpClntSock;
				delete lpIOData;
				continue;
			}
		}

		if (lpClntSock->GetIOMode() == E_IOMODE_RECV)
		{
			/*수신된 메시지를 모든 유저에게 발신합니다.*/
			for (auto Iter : liClientData)
			{
				Iter->WriteBuffer(lpClntSock->GetBufferData());
				Iter->IOCPSend();
			}
		}
		else
		{
			/*모두 전송이 완료되면 해당 소켓을 Recv 모드로 전환합니다.*/
			DWORD dwRecvByte = 0;
			lpClntSock->IOCPRecv();
		}
	}

	JIGAPPrintSystemLog("Unactive Chatting Thread!");
}


bool JIGAPServer::JIGAPServerOpen(std::string _szIpAddr, std::string _szPortAddr)
{
	szIpAddr = _szIpAddr;
	szPortAddr = _szPortAddr;

	bServerOn = true;

	if (FAILED(JIGAPServer::JIGAPInitializeServer()))
	{
		JIGAPPrintSystemLog("Failed Open Server");
		return false;
	}

	hSystemLogMutex = CreateMutex(0, FALSE, NULL);

	/*연결을 담당하는 Thread 입니다.*/
	connectThread = std::thread([&]() { JIGAPConnectThread(); });
	Sleep(10);
	/*Chatting을 담당하는 Thread 입니다.*/
	chattingThread = std::thread([&]() { JIGAPChattingThread(); });
	Sleep(10);

	JIGAPPrintSystemLog("Opened Server!");
	return true;
}

void JIGAPServer::JIGAPServerClose()
{
	/*서버를 종료합니다.*/
	bServerOn = false;
	Sleep(10);

	JIGAPReleaseServer();

	/*Thread를 해제합니다.*/
	if (chattingThread.joinable())
		chattingThread.join();
	if (connectThread.joinable())
		connectThread.join();

	/*생성한 WInAPI Mutex를 해제합니다.*/
	CloseHandle(hSystemLogMutex);

	JIGAPPrintSystemLog("Closed Server");
}

void JIGAPServer::JIGAPPrintSystemLog(const char* szInFormat, ...)
{
	/*Mutex를 소유합니다. 해당핸들은 non-signaled 상태가 됩니다 반면 이미 핸들이 Non-signaled 상태이면 블록킹합니다. */
	WaitForSingleObject(hSystemLogMutex, INFINITE);
	char buf[512] = { 0 };
	va_list ap;

	va_start(ap, szInFormat);
	vsprintf(buf, szInFormat, ap);
	va_end(ap);
	
	qSystemMsg.push(buf);
	
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
