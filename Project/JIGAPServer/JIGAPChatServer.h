#pragma once
#include "JIGAPBaseServer.h"

class TCPSocket;


class JIGAPChatServer :
	public JIGAPBaseServer
{
public:
	// JIGAPServerBase��(��) ���� ��ӵ�
	virtual void OnInitialize()							override;
	virtual void OnRelease()							override;
	virtual void OnConnect(TCPSocket* lpInSocket)		override;
	virtual void OnDisconnect(TCPSocket* lpInSocket)	override;
	virtual void OnProcess(TCPSocket* lpInTCPSocket, PacketHandler * lpHandler)	override;
};

