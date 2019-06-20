#include "pch.h"
#include "JIGAPClient.h"

JIGAPClient::JIGAPClient()
	:bLogin(false),
	lpSocket(nullptr), 
	lpPacketHandler( new PacketHandler)
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
		DEBUG_LOG("WSAStartup Error! Code : " << iErrorCode)
		return E_FAIL;
	}
	
	if ((iErrorCode = lpSocket->SYNCSocket()))
	{
		DEBUG_LOG("socket Error! Code : " << iErrorCode << ", Faild Create Socket");
		return E_FAIL;
	}

	if ((iErrorCode = lpSocket->Connect(strIpAddr.c_str(), strPortAddr.c_str())))
	{
		DEBUG_LOG("connect Error! Code : " << iErrorCode << ", Faild to Connect to Server");
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
	strIpAddr = InIpAddr;
	strPortAddr = InPortAddr;

	if (FAILED(JIGAPInitializeClient()))
	{
		DEBUG_LOG("서버에 연결하지 못했습니다.");
		return false;
	}


	return true;
}

void JIGAPClient::JIGAPClientEnd()
{
	JIGAPReleaseClient();

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
			DEBUG_LOG("서버와 연결이 끊어졌습니다.");
			break;
		}
		else if (iRecvLen == -1)
		{
			DEBUG_LOG("recv Error! Code : " << WSAGetLastError() << ", Socket : "<< lpSocket->GetSocket());
			break;
		}

		unsigned int packetSize = lpPacketHandler->ParsingPacketSize(message);
		if (packetSize > iRecvLen)
			continue;
		
		lpPacketHandler->ClearSendStream();
		lpPacketHandler->SetPacketInRecvStream(message, packetSize);

		PacketHeader header; 
		lpPacketHandler->ParsingPacketHeader(header);

		lpPacketHandler->ClearSendStream();

	}
}
