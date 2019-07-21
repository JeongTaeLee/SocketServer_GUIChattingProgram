#pragma once
#include "JIGAPBaserProcess.h"

class TCPSocket;


class JIGAPChatProcess :
	public JIGAPBaserProcess
{
public:
	// JIGAPServerBase��(��) ���� ��ӵ�
	virtual void OnInitialize()							override;
	virtual void OnRelease()							override;
	virtual void OnConnect(TCPSocket* lpInSocket)		override;
	virtual void OnDisconnect(TCPSocket* lpInSocket)	override;
	virtual void OnProcess(TCPSocket* lpInTCPSocket, PacketHandler * lpHandler)	override;
};

