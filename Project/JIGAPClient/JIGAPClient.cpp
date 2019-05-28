#include "pch.h"
#include "JIGAPClient.h"

JIGAPClient::JIGAPClient()
	:eClientState(JIGAPSTATE::E_REQUESTROOM), 
	bLogin(false),
	lpSocket(nullptr), 
	lpSerializeObject(new SerializeObject),	
	hMessageMutex(nullptr),

	lpOnLoginCallBack(nullptr),
	lpOnLoginFailedCallBack(nullptr),
	lpOnRoomListCallBack(nullptr),
	lpOnCreateRoomCallBack(nullptr),
	lpOnCreateRoomFailedCallBack(nullptr),
	lpOnJoinedRoomCallBack(nullptr),
	lpOnJoinedRoomFailedCallBack(nullptr),
	lpOnExitRoomCallBack(nullptr),
	lpOnChattingCallBack(nullptr)

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
	JIGAPPrintMessageLog("WSAStartUp ȣ���� �����߽��ϴ�!", iErrorCode);
	
	if ((iErrorCode = lpSocket->SYNCSocket()))
	{
		JIGAPPrintMessageLog("socket Error! Code : %d, Faild Create Socket", iErrorCode);
		return E_FAIL;
	}
	JIGAPPrintMessageLog("socket ȣ���� �����߽��ϴ�!", iErrorCode);

	if ((iErrorCode = lpSocket->Connect(strIpAddr.c_str(), strPortAddr.c_str())))
	{
		JIGAPPrintMessageLog("connect Error! Code : %d, Faild to Connect to Server", iErrorCode);
		return E_FAIL;
	}
	JIGAPPrintMessageLog("connect ȣ���� �����߽��ϴ�.", iErrorCode);

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
		JIGAPPrintMessageLog("������ �������� ���߽��ϴ�.");
		return false;
	}

	/*ũ��Ƽ�� ���ǿ� ���� �������� �ʱ� ���� Mutex�� �����մϴ�.*/
	hMessageMutex = CreateMutex(NULL, false, NULL);

	/*������ ����ϴ� �����带 �����մϴ�.*/
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
			JIGAPPrintMessageLog("������ ������ ���������ϴ�.");
			break;
		}
		else if (iRecvLen == -1)
		{
			JIGAPPrintMessageLog("recv Error! Code : %d, Socket : %d", WSAGetLastError(), lpSocket->GetSocket());
			break;
		}

		lpSerializeObject->ClearSendStreamBuffer();
		lpSerializeObject->ClearRecvStreamBuffer();
		lpSerializeObject->SetRecvStreamBuffer(message);

		uint32_t literal = 0;
		lpSerializeObject->DeserializeRecvBuffer(literal);

		switch (literal)
		{
		case answerLoginLiteral:
			JIGAPOnAnswerLogin();
			break;

		case answerRoomListLiteral:
			JIGAPOnAnswerRoomList();
			break;

		case answerCreateRoomLiteral:
			JIGAPOnAnswerCreateRoom();
			break;

		case answerJoinedRoomLiteral:
			JIGAPOnAnswerJoinedRoom();
			break;

		case anwserChattingLiteral:
			JIGAPOnAnswerChatting();
			break;

		case answerExitRoomLiteral:
			JIGAPOnAnswerExtiRoom();
			break;
		
		default:
			break;
		}
	}
}



bool JIGAPClient::JIGAPRequsetLogin(const std::string & strInNickName)
{
	if (!bLogin)
	{
		char nameBuffer[MAXNAMESIZE] = { 0 };
		memcpy(nameBuffer, strInNickName.c_str(), strInNickName.length());

		lpSerializeObject->SerializeDataSendBuffer(requestLoginLiteral);
		lpSerializeObject->SerializeDataSendBuffer(nameBuffer, sizeof(nameBuffer));
		strMyName = strInNickName;
		return JIGAPSendSerializeBuffer();
	}
	return false;
}

bool JIGAPClient::JIGAPRequestRoomList()
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		lpSerializeObject->SerializeDataSendBuffer(requestRoomListLiteral);
		return JIGAPSendSerializeBuffer();
	}

	return false;
}

bool JIGAPClient::JIGAPRequestCreateRoom(const std::string& strInRoomName)
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		char roomName[MAXROOMNAMESIZE] = {0};
		memcpy(roomName, strInRoomName.c_str(), strInRoomName.length());

		lpSerializeObject->SerializeDataSendBuffer(requestCreateRoomLiteral);
		lpSerializeObject->SerializeDataSendBuffer(roomName, sizeof(roomName));
		return JIGAPSendSerializeBuffer();
	}

	return false;
}

bool JIGAPClient::JIGAPRequestJoinedRoom(const std::string& strInRoomName)
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		char roomName[MAXROOMNAMESIZE] = {0};
		memcpy(roomName, strInRoomName.c_str(), strInRoomName.length());
	
		lpSerializeObject->SerializeDataSendBuffer(requestJoinedRoomLiteral);
		lpSerializeObject->SerializeDataSendBuffer(roomName, sizeof(roomName));
		return JIGAPSendSerializeBuffer();
	}

	return false;
}

bool JIGAPClient::JIGAPRequestExtiRoom()
{
	if (bLogin && eClientState == JIGAPSTATE::E_ONROOM)
	{
		lpSerializeObject->SerializeDataSendBuffer(requestExitRoomLiteral);
		return JIGAPSendSerializeBuffer();
	}

	return false;
}

bool JIGAPClient::JIGAPRequestChatting(const std::string& strInMessage)
{
	if (bLogin && eClientState == JIGAPSTATE::E_ONROOM)
	{
		char message[MAXSTRSIZE] = { 0 };
		memcpy(message, strInMessage.c_str(), strInMessage.size());
	
		lpSerializeObject->SerializeDataSendBuffer(requestChattingLiteral);
		lpSerializeObject->SerializeDataSendBuffer(message, sizeof(message));
		return JIGAPSendSerializeBuffer();
	}
	return false;
}

void JIGAPClient::JIGAPOnAnswerLogin()
{
	if (!bLogin)
	{
		bool loginSuccees = false;
		lpSerializeObject->DeserializeRecvBuffer(loginSuccees);

		if (loginSuccees)
		{
			JIGAPPrintMessageLog("�α����� �����߽��ϴ�.");

			if (lpOnLoginCallBack)
				lpOnLoginCallBack();

			eClientState = JIGAPSTATE::E_NOTROOM;
			
			bLogin = true;
		}
		else
		{
			if (lpOnLoginFailedCallBack)
				lpOnLoginFailedCallBack();

			JIGAPPrintMessageLog("�α����� �����߽��ϴ�");
		}
	}
}

void JIGAPClient::JIGAPOnAnswerRoomList()
{
	if (bLogin )
	{
		int iCount = 0;
		lpSerializeObject->DeserializeRecvBuffer(iCount);

		liRoomList.clear();
		for (int i = 0; i < iCount; ++i)
		{
			char roomName[MAXROOMNAMESIZE] = {0};
			lpSerializeObject->DeserializeRecvBuffer(roomName);
			liRoomList.push_back(roomName);
		}

		if (lpOnRoomListCallBack)
			lpOnRoomListCallBack();
	}
}

void JIGAPClient::JIGAPOnAnswerCreateRoom()
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		bool bSuccess = false;
		lpSerializeObject->DeserializeRecvBuffer(bSuccess);

		if (bSuccess)
		{
			JIGAPPrintMessageLog("������� �����߽��ϴ�.");
			JIGAPPrintMessageLog("������ �濡 �����մϴ�.");
			
			if (lpOnCreateRoomCallBack)
				lpOnCreateRoomCallBack();
			
			eClientState = JIGAPSTATE::E_ONROOM;
		}
		else
		{
			if (lpOnCreateRoomFailedCallBack)
				lpOnCreateRoomFailedCallBack();

			JIGAPPrintMessageLog("������� �����߽��ϴ�.");
		}
	}
}

void JIGAPClient::JIGAPOnAnswerJoinedRoom()
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		bool bSuccess = false;
		lpSerializeObject->DeserializeRecvBuffer(bSuccess);

		if (bSuccess)
		{
			JIGAPPrintMessageLog("�������� �����߽��ϴ�.");

			char roomName[MAXROOMNAMESIZE] = { 0 };
			lpSerializeObject->DeserializeRecvBuffer(roomName, sizeof(roomName));

			if (lpOnJoinedRoomCallBack)
				lpOnJoinedRoomCallBack(roomName, strlen(roomName));
			
			eClientState = JIGAPSTATE::E_ONROOM;
		}
		else
		{
			if (lpOnJoinedRoomFailedCallBack)
				lpOnJoinedRoomFailedCallBack();

			JIGAPPrintMessageLog("�������� �����߽��ϴ�.");
		}
	}
}

void JIGAPClient::JIGAPOnAnswerExtiRoom()
{
	if (bLogin && eClientState == JIGAPSTATE::E_ONROOM)
	{
		eClientState = JIGAPSTATE::E_NOTROOM;
		JIGAPPrintMessageLog("�濡�� ���Խ��ϴ�.");

		if (lpOnExitRoomCallBack)
			lpOnExitRoomCallBack();
	}
}

void JIGAPClient::JIGAPOnAnswerChatting()
{
	if (bLogin && eClientState == JIGAPSTATE::E_ONROOM)
	{
		char sender[MAXNAMESIZE] = { 0 };
		lpSerializeObject->DeserializeRecvBuffer(sender, sizeof(sender));

		char message[MAXSTRSIZE] = { 0 };
		lpSerializeObject->DeserializeRecvBuffer(message, sizeof(message));

	
		if (lpOnChattingCallBack)
		{
			lpOnChattingCallBack(sender, message, strlen(sender), strlen(message));
		}
	}
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

bool JIGAPClient::JIGAPSendSerializeBuffer()
{
	int iSendLen = lpSocket->SYNCSend(lpSerializeObject->GetSendStreamBuffer());

	if (iSendLen == 0)
	{
		JIGAPPrintMessageLog("send Error! Code : % d, Socket : % d", WSAGetLastError(), lpSocket->GetSocket());
		return false;
	}
	return true;
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

