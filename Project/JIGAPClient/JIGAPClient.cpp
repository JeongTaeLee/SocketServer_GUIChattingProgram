#include "pch.h"
#include "JIGAPClient.h"

JIGAPClient::JIGAPClient()
	:eClientState(JIGAPSTATE::E_REQUESTROOM), 
	bLogin(false),
	lpSocket(nullptr), 
	lpSerializeObject(new SerializeObject),	
	lpPacketHandler( new PacketHandler),
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

	
//Test�� �ڵ��Դϴ�.
	/*lpPacketHandler->ClearSendStream();
	JIGAPPacket::StringPacket packet;
	packet.set_str("TestPacket");
	lpPacketHandler->SerializePacket(JIGAPPacket::LoginRequestType, packet);
	lpSocket->SYNCSend(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());*/
//
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
		char message[MAXBUFFERSIZE] = {0};
		
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

		unsigned int packetSize = lpPacketHandler->ParsingPacketSize(message);
		if (packetSize > iRecvLen)
			continue;
		
		lpPacketHandler->ClearSendStream();
		lpPacketHandler->SetPacketInRecvStream(message, packetSize);

		PacketHeader header; 
		lpPacketHandler->ParsingPacketHeader(header);

		switch (header.packetType)
		{
		case JIGAPPacket::LoginAnswerType:
			JIGAPOnAnswerLogin(header.size);
			break;
		case JIGAPPacket::RoomListAnswerType:
			JIGAPOnAnswerRoomList(header.size);
			break;
		case JIGAPPacket::CreateRoomAnswerType:
			JIGAPOnAnswerCreateRoom(header.size);
			break;
		case JIGAPPacket::JoinedRoomAnswerType:
			JIGAPOnAnswerJoinedRoom(header.size);
			break;
		case JIGAPPacket::ExitRoomAnswerType:
			JIGAPOnAnswerExtiRoom(header.size);
			break;
		case JIGAPPacket::ChattingSpreadType:
			JIGAPOnAnswerChatting(header.size);
			break;
		default:
			break;
		}
		lpPacketHandler->ClearSendStream();

	}
}



bool JIGAPClient::JIGAPRequsetLogin(const std::string & strInNickName)
{
	if (!bLogin)
	{
		lpPacketHandler->ClearSendStream();

		JIGAPPacket::StringPacket strPacket;
		strPacket.set_str(strInNickName);

		lpPacketHandler->SerializePacket(JIGAPPacket::LoginRequestType, strPacket);
		return this->JIGAPSendSerializeBuffer(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
	}
	return false;
}

bool JIGAPClient::JIGAPRequestRoomList()
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		lpPacketHandler->ClearSendStream();

		lpPacketHandler->SerializeHader(JIGAPPacket::RoomListRequestType);
		return this->JIGAPSendSerializeBuffer(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
	}

	return false;
}

bool JIGAPClient::JIGAPRequestCreateRoom(const std::string& strInRoomName)
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		lpPacketHandler->ClearSendStream();

		JIGAPPacket::StringPacket strPacket;
		strPacket.set_str(strInRoomName);

		lpPacketHandler->SerializePacket(JIGAPPacket::CreateRoomRequestType, strPacket);
		return this->JIGAPSendSerializeBuffer(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
	}

	return false;
}

bool JIGAPClient::JIGAPRequestJoinedRoom(const std::string& strInRoomName)
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		lpPacketHandler->ClearSendStream();

		JIGAPPacket::StringPacket strPacket;
		strPacket.set_str(strInRoomName);

		lpPacketHandler->SerializePacket(JIGAPPacket::JoinedRoomRequestType, strPacket);
		return this->JIGAPSendSerializeBuffer(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
	}

	return false;
}

bool JIGAPClient::JIGAPRequestExtiRoom()
{
	if (bLogin && eClientState == JIGAPSTATE::E_ONROOM)
	{
		lpPacketHandler->ClearSendStream();

		lpPacketHandler->SerializeHader(JIGAPPacket::ExitRoomRequestType);
		return this->JIGAPSendSerializeBuffer(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
	}

	return false;
}

bool JIGAPClient::JIGAPRequestChatting(const std::string& strInMessage)
{
	if (bLogin && eClientState == JIGAPSTATE::E_ONROOM)
	{
		JIGAPPacket::StringPacket strPacket;
		strPacket.set_str(strInMessage);

		lpPacketHandler->SerializePacket(JIGAPPacket::ChattingRequestType, strPacket);
		return this->JIGAPSendSerializeBuffer(lpPacketHandler->GetSendStream(), lpPacketHandler->GetSendStreamPosition());
	}
	return false;
}

void JIGAPClient::JIGAPOnAnswerLogin(unsigned int iInSize)
{
	if (!bLogin)
	{
		JIGAPPacket::BoolPacket answerPacket;
		lpPacketHandler->ParsingPacket(answerPacket, iInSize);

		bool bLoginSuccess = answerPacket.success();

		if (bLoginSuccess)
		{
			if (lpOnLoginCallBack)
				lpOnLoginCallBack();

			eClientState = JIGAPSTATE::E_NOTROOM;
			bLogin = true;
		}
		else
		{
			if (lpOnLoginFailedCallBack)
				lpOnLoginFailedCallBack();
		}
	}
}

void JIGAPClient::JIGAPOnAnswerRoomList(unsigned int iInSize)
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		JIGAPPacket::RoomListAnswerPacket answerPacket;
		lpPacketHandler->ParsingPacket(answerPacket, iInSize);

		liRoomList.clear();
		for (int i = 0; i < answerPacket.roomcount(); ++i)
		{
			PacketHeader header;
			lpPacketHandler->ParsingPacketHeader(header);
			
			if (header.packetType == JIGAPPacket::ElementOfRoomListType)
			{
				JIGAPPacket::StringPacket roomNamePacket;
				lpPacketHandler->ParsingPacket(roomNamePacket, header.size);
				liRoomList.push_back(roomNamePacket.str());
			}
		}

		if (lpOnRoomListCallBack)
			lpOnRoomListCallBack();
	}
}

void JIGAPClient::JIGAPOnAnswerCreateRoom(unsigned int iInSize)
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		JIGAPPacket::BoolPacket answerPacket;
		lpPacketHandler->ParsingPacket(answerPacket, iInSize);

		if (answerPacket.success())
		{
			if (lpOnCreateRoomCallBack)
				lpOnCreateRoomCallBack();

			eClientState = JIGAPSTATE::E_ONROOM;
		}
		else
		{
			if (lpOnCreateRoomFailedCallBack)
				lpOnCreateRoomFailedCallBack();
		}
	}
}

void JIGAPClient::JIGAPOnAnswerJoinedRoom(unsigned int iInSize)
{
	if (bLogin && eClientState == JIGAPSTATE::E_NOTROOM)
	{
		JIGAPPacket::JoinedRoomAnswerPacket answerPacket;
		lpPacketHandler->ParsingPacket(answerPacket, iInSize);

		if (answerPacket.joinedroomsuccess())
		{
			if (lpOnJoinedRoomCallBack)
				lpOnJoinedRoomCallBack(answerPacket.roomname().c_str(), answerPacket.roomname().size());

			eClientState = JIGAPSTATE::E_ONROOM;
		}
		else
		{
			if (lpOnJoinedRoomFailedCallBack)
				lpOnJoinedRoomFailedCallBack();
		}
	}
}


void JIGAPClient::JIGAPOnAnswerExtiRoom(unsigned int iInSize)
{
	if (bLogin && eClientState == JIGAPSTATE::E_ONROOM)
	{
		eClientState = JIGAPSTATE::E_NOTROOM;

		if (lpOnExitRoomCallBack)
			lpOnExitRoomCallBack();
	}
}

void JIGAPClient::JIGAPOnAnswerChatting(unsigned int iInSize)
{
	if (bLogin && eClientState == JIGAPSTATE::E_ONROOM)
	{
		JIGAPPacket::ChattingSpreadPacket spreadPacket;
		lpPacketHandler->ParsingPacket(spreadPacket, iInSize);

		if (lpOnChattingCallBack)
			lpOnChattingCallBack(spreadPacket.sender().c_str(), spreadPacket.msg().c_str(), spreadPacket.sender().size(), spreadPacket.msg().size());
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

bool JIGAPClient::JIGAPSendSerializeBuffer(const char* szInMessage, unsigned int iInSize)
{
	int iSendLen = lpSocket->SYNCSend(szInMessage, iInSize);

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

