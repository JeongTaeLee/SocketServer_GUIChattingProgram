#pragma once

class Room;
class TCPIOData;
class TCPSocket;
class SerializeObject;
class PacketHandler;
class UserTCPSocket;

class JIGAPServer
{
private:
	TCPSocket* lpServSock;
	PacketHandler* lpPacketHandler;

	HANDLE hSystemLogMutex;	
	HANDLE hClientDataMutex;
	HANDLE hRoomsMutex;

	std::string szIpAddr;
	std::string szPortAddr;

	std::thread connectThread;
	std::thread ioThread;

	std::map < SOCKET, UserTCPSocket* > mClientData;
	std::map < std::string, Room*> mRooms;
	
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
	void JIGAPIOThread();

	DWORD  CheckIOCompletionSocket(UserTCPSocket* & inSocket, TCPIOData* & inIOData);

	void RemoveClientInServer(const SOCKET& hSock);
	void RemoveClientInRoom(UserTCPSocket* lpSock);
public:
	void JIGAPPrintSystemLog(const char * fmt, ...);
	
	std::string JIGAPGetSystemMsg();
	bool JIGAPCheckSystemMsg() { return !qSystemMsg.empty(); };
public:
	
};

