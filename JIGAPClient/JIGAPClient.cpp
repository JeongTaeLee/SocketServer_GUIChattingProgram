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
	int iErrorCode = 0;
	lpSocket = new TCPSocket;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		JIGAPPrintMessageLog("WSAStartup Error! Code : %d", iErrorCode);
		return E_FAIL;
	}
	
	if ((iErrorCode = lpSocket->SYNCSocket()))
	{
		JIGAPPrintMessageLog("socket Error! Code : %d, Faild Create Socket", iErrorCode);
		return E_FAIL;
	}
	if ((iErrorCode = lpSocket->Connect(szIpAddr.c_str(), szPortAddr.c_str())))
	{
		JIGAPPrintMessageLog("connect Error! Code : %d, Faild to Connect to Server", iErrorCode);
		return E_FAIL;
	}

	return S_OK;
}

void JIGAPClient::JIGAPReleaseClient()
{
	lpSocket->Closesocket();
	WSACleanup();
}

bool JIGAPClient::JIGAPClientStart(const std::string& InIpAddr, const std::string& InPortAddr)
{
	szIpAddr = InIpAddr;
	szPortAddr = InPortAddr;

	if (FAILED(JIGAPInitializeClient()))
	{
		JIGAPPrintMessageLog("Failed to connect to server");
		return false;
	}

	/*ũ��Ƽ�� ���ǿ� ���� �������� �ʱ� ���� Mutex�� �����մϴ�.*/
	hMessageMutex = CreateMutex(NULL, false, NULL);

	/*������ ����ϴ� �����带 �����մϴ�.*/
	recvThread = std::thread([&]() { JIGAPRecvThread(); });
	/*�߽��� ����ϴ� �����带 �����մϴ�.*/
	sendThread = std::thread([&]() { JIGAPSendThread(); });

	return true;
}

void JIGAPClient::JIGAPClientEnd()
{
	JIGAPReleaseClient();

	if (recvThread.joinable())
		recvThread.join();
	if (sendThread.joinable())
		sendThread.join();

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
			JIGAPPrintMessageLog("Disconnected from server");
			break;
		}
		else if (iRecvLen == -1)
		{
			JIGAPPrintMessageLog("recv Error! Code : %d, Socket : %d", WSAGetLastError(), lpSocket->GetSocket());
			break;
		}
	}
}

void JIGAPClient::JIGAPSendThread()
{
}

void JIGAPClient::JIGAPPrintMessageLog(const char* fmt, ...)
{
	/*Mutex�� �����մϴ�. �ش��ڵ��� non-signaled ���°� �˴ϴ� �ݸ� �̹� �ڵ��� Non-signaled �����̸� ���ŷ�մϴ�. */
	WaitForSingleObject(hMessageMutex, INFINITE);
	char buf[512] = { 0 };
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	qMessage.push(buf);

	/*Mutex �������� �ʰ� �ٲ��ݴϴ�. ���ؽ��� signaled ���·� �ٲߴϴ�*/
	ReleaseMutex(hMessageMutex);
}

