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
			JIGAPPrintMessageLog("로그인을 성공했습니다.");

			if (lpOnLoginCallBack)
				lpOnLoginCallBack();

			eClientState = JIGAPSTATE::E_NOTROOM;
			
			bLogin = true;
		}
		else
		{
			if (lpOnLoginFailedCallBack)
				lpOnLoginFailedCallBack();

			JIGAPPrintMessageLog("로그인을 실패했습니다");
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
			JIGAPPrintMessageLog("방생성에 성공했습니다.");
			JIGAPPrintMessageLog("생성한 방에 참가합니다.");
			
			if (lpOnCreateRoomCallBack)
				lpOnCreateRoomCallBack();
			
			eClientState = JIGAPSTATE::E_ONROOM;
		}
		else
		{
			if (lpOnCreateRoomFailedCallBack)
				lpOnCreateRoomFailedCallBack();

			JIGAPPrintMessageLog("방생성에 성공했습니다.");
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
			JIGAPPrintMessageLog("방참가에 성공했습니다.");

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

			JIGAPPrintMessageLog("방참가에 실패했습니다.");
		}
	}
}

void JIGAPClient::JIGAPOnAnswerExtiRoom()
{
	if (bLogin && eClientState == JIGAPSTATE::E_ONROOM)
	{
		eClientState = JIGAPSTATE::E_NOTROOM;
		JIGAPPrintMessageLog("방에서 나왔습니다.");

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

