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
	HANDLE hUsersMutex;
	HANDLE hRoomsMutex;

	std::string szIpAddr;
	std::string szPortAddr;

	std::thread connectThread;
	std::thread recvThread;

	std::map < SOCKET, UserTCPSocket* > mUsers;
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

	void RecvProcess();
	void WorkProcess();

public:
	void RemoveClientInServer(const SOCKET& hSock);
	void RemoveClientInRoom(UserTCPSocket* lpSock);

public:
	void UsersLock() { WaitForSingleObject(hUsersMutex, INFINITE); }
	void UsersUnlock() { ReleaseMutex(hUsersMutex); }
	
	void RoomsLock() { WaitForSingleObject(hRoomsMutex, INFINITE); }
	void RoomsUnlock() { ReleaseMutex(hRoomsMutex); }

public:
	const std::map < SOCKET, UserTCPSocket* >& GetUsers() { return mUsers; }
	std::map < std::string, Room*>& GetRooms() { return mRooms; }
	PacketHandler* GetPacketHandler() { return lpPacketHandler; }

public:
	void JIGAPPrintSystemLog(const char * fmt, ...);
	
	std::string JIGAPGetSystemMsg();
	bool JIGAPCheckSystemMsg() { return !qSystemMsg.empty(); };
	
};

