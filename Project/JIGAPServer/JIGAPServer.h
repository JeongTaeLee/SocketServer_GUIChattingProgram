#pragma once

class Room;
class TCPIOData;
class TCPSocket;
class SerializeObject;
class PacketHandler;

class JIGAPServer
{
private:
	TCPSocket* lpServSock;
	SerializeObject* lpSerializeObject;
	PacketHandler* lpPacketHandler;

	HANDLE hSystemLogMutex;	
	HANDLE hClientDataMutex;
	HANDLE hRoomsMutex;

	std::string szIpAddr;
	std::string szPortAddr;

	std::thread connectThread;
	std::thread chattingThread;

	std::map < std::string, Room*> mRooms;
	std::map < SOCKET, TCPSocket* > mClientData;
	
	std::queue < std::string > qSystemMsg;

	bool bServerOn;
public:
	HANDLE hCompletionHandle;

public:
	JIGAPServer();
	virtual ~JIGAPServer();

private:
	HRESULT JIGAPInitializeServer();
	void JIGAPReleaseServer();

public:
	bool JIGAPServerOpen(std::string szIpAddr, std::string szPortAddr);
	void JIGAPServerClose();

public:
	void JIGAPConnectThread();
	void JIGAPChattingThread();

	void OnLogin(TCPSocket* & lpClntData);
	void OnRequestRoomList(TCPSocket*& lpClntData);
	void OnRequestCreateRoom(TCPSocket* & lpClntData);
	void OnRequestJoinedRoom(TCPSocket* & lpClntData);
	void OnRequestExtiRoom(TCPSocket* & lpClntData);
	void OnRequestChatting(TCPSocket* & lpClntData);

	DWORD  CheckIOCompletionSocket(TCPSocket* & inSocket, TCPIOData* & inIOData);

public:
	void JIGAPPrintSystemLog(const char * fmt, ...);
	
	std::string JIGAPGetSystemMsg();
	bool JIGAPCheckSystemMsg() { return !qSystemMsg.empty(); };
public:
	void RemoveClient(const SOCKET & hSock);
	void RemoveClientToRoom(TCPSocket* & lpSock);
	
};

