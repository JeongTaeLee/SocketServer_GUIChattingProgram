#pragma once
#include "JIGAPBaserProcess.h"

class JIGAPServer;
class TCPSocket;
class ChatRoomAdmin;
class ChatUserData;
class ChatQuery;

template<typename _Ty> class UserDataAdmin;


class JIGAPChatProcess :
	public JIGAPBaserProcess
{
private:
	UserDataAdmin<ChatUserData>* lpUserAdmin = nullptr;
	ChatRoomAdmin* lpChatRoomAdmin = nullptr;
	ChatQuery* lpQuery = nullptr;

private:
	JIGAPChatProcess()
		:JIGAPBaserProcess(nullptr) { }

public:
	JIGAPChatProcess(JIGAPServer* inLpJIGAPServer) : JIGAPBaserProcess(inLpJIGAPServer) {}
	virtual ~JIGAPChatProcess() {}

	// JIGAPServerBase을(를) 통해 상속됨
	virtual bool OnInitialize()							override;
	virtual void OnRelease()							override;
	virtual void OnConnect(TCPSocket* lpInSocket)		override;
	virtual void OnDisconnect(TCPSocket* lpInSocket)	override;
	virtual void OnProcess(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler)	override;

	bool OnSingUpRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header);
	bool OnLoginRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header);
	bool OnJoinRoomRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header);
	bool OnRoomListRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header);
	bool OnCreateRoomRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header);
	bool OnChatRequest(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler, PacketHeader& header);

	void ExitRoom(TCPSocket* lpInTCPSocket);

public:
	void PutUserIntoRoom(PacketHandler* inLpHandler, ChatUserData* inLpChatUserData, const std::string& inStrRoomName);

};

