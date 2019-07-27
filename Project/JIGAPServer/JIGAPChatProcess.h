#pragma once
#include "JIGAPBaserProcess.h"
class TCPSocket;
class ChatUserData;
class BaseDB;

template<typename _Ty> class UserDataAdmin;


class JIGAPChatProcess :
	public JIGAPBaserProcess
{
private:
	UserDataAdmin<ChatUserData>* lpUserAdmin = nullptr;
	BaseDB* lpDB = nullptr;
	
public:
	JIGAPChatProcess() {}
	virtual ~JIGAPChatProcess() {}

	// JIGAPServerBase을(를) 통해 상속됨
	virtual void OnInitialize()							override;
	virtual void OnRelease()							override;
	virtual void OnConnect(TCPSocket* lpInSocket)		override;
	virtual void OnDisconnect(TCPSocket* lpInSocket)	override;
	virtual void OnProcess(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler)	override;

	void OnSingUpRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header);
	void OnLoginRequest(TCPSocket * lpInTCPSocket, PacketHandler * lpHandler, PacketHeader& header);

};

