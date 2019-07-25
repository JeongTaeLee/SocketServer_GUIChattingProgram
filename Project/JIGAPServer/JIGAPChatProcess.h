#pragma once
#include "JIGAPBaserProcess.h"
class TCPSocket;
class ChatUserData;

template<typename _Ty> class UserDataAdmin;


class JIGAPChatProcess :
	public JIGAPBaserProcess
{
private:
	UserDataAdmin<ChatUserData> * lpUserAdmin;
public:
	JIGAPChatProcess() {}
	virtual ~JIGAPChatProcess() {}

	// JIGAPServerBase을(를) 통해 상속됨
	virtual void OnInitialize()							override;
	virtual void OnRelease()							override;
	virtual void OnConnect(TCPSocket* lpInSocket)		override;
	virtual void OnDisconnect(TCPSocket* lpInSocket)	override;
	virtual void OnProcess(TCPSocket* lpInTCPSocket, PacketHandler * lpHandler)	override;
};

