#include "pch.h"
#include "JIGAPClient.h"


JIGAPClient::JIGAPClient()
{
}


JIGAPClient::~JIGAPClient()
{
}

HRESULT JIGAPClient::JIGAPInitializeClient()
{
	std::string value;

	int iErrorCode = 0;
	lpSocket = new TCPSocket;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		JIGAPPrintMessageLog("WSAStartup Error! Code : %d", iErrorCode);
		return E_FAIL;
	}
	JIGAPPrintMessageLog("WSAStartUp 호출을 성공했습니다!", iErrorCode);
	
	if ((iErrorCode = lpSocket->SYNCSocket()))
	{
		JIGAPPrintMessageLog("socket Error! Code : %d, Faild Create Socket", iErrorCode);
		return E_FAIL;
	}
	JIGAPPrintMessageLog("socket 호출을 성공했습니다!", iErrorCode);

	if ((iErrorCode = lpSocket->Connect(strIpAddr.c_str(), strPortAddr.c_str())))
	{
		JIGAPPrintMessageLog("connect Error! Code : %d, Faild to Connect to Server", iErrorCode);
		return E_FAIL;
	}
	JIGAPPrintMessageLog("connect 호출을 성공했습니다.", iErrorCode);

	return S_OK;
}

void JIGAPClient::JIGAPReleaseClient()
{
	lpSocket->Closesocket();
	WSACleanup();
}

bool JIGAPClient::JIGAPClientStart(const std::string& InIpAddr, const std::string& InPortAddr)
{
	strIpAddr = InIpAddr;
	strPortAddr = InPortAddr;

	if (FAILED(JIGAPInitializeClient()))
	{
		JIGAPPrintMessageLog("서버에 연결하지 못했습니다.");
		return false;
	}

	/*크리티컬 섹션에 동시 접근하지 않기 위해 Mutex를 생성합니다.*/
	hMessageMutex = CreateMutex(NULL, false, NULL);

	/*수신을 담당하는 쓰레드를 생성합니다.*/
	recvThread = std::thread([&]() { JIGAPRecvThread(); });



	return true;
}

void JIGAPClient::JIGAPClientEnd()
{
	JIGAPReleaseClient();

	if (recvThread.joinable())
		recvThread.join();

	CloseHandle(hMessageMutex);

	delete lpSocket;
}

void JIGAPClient::JIGAPRecvThread()
{
	while (true)
	{
		char message[MAXBUFFERSIZE];
		
		int iRecvLen = lpSocket->SYNCRecv(message);

		if (iRecvLen == 0)
		{
			JIGAPPrintMessageLog("서버와 연결이 끊어졌습니다.");
			break;
		}
		else if (iRecvLen == -1)
		{
			JIGAPPrintMessageLog("recv Error! Code : %d, Socket : %d", WSAGetLastError(), lpSocket->GetSocket());
			break;
		}

		JIGAPPrintMessageLog(message);
	}
}

bool JIGAPClient::JIGAPSend(std::string szInMessage)
{
	int iRecvLen = lpSocket->SYNCSend(szInMessage.c_str());
	
	if (iRecvLen == 0)
	{
		JIGAPPrintMessageLog("send Error! Code : %d, Socket : %d", WSAGetLastError(), lpSocket->GetSocket());
		return false;
	}

	return true;
}

void JIGAPClient::JIGAPPrintMessageLog(const char* fmt, ...)
{
	/*Mutex를 소유합니다. 해당핸들은 non-signaled 상태가 됩니다 반면 이미 핸들이 Non-signaled 상태이면 블록킹합니다. */
	WaitForSingleObject(hMessageMutex, INFINITE);
	char buf[512] = { 0 };
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	qMessage.push(buf);

	/*Mutex 소유하지 않게 바꿔줍니다. 뮤텍스를 signaled 상태로 바꿉니다*/
	ReleaseMutex(hMessageMutex);
}

bool JIGAPClient::JIGAPCheckMessageLog()
{
	return 	!qMessage.empty();
}

std::string JIGAPClient::JIGAPGetMessageLog()
{
	std::string str = qMessage.front();
	qMessage.pop();
	return str;
}

