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

	void RecvProcessing(TCPSocket* lpInTcpSocket, int iInRecvByte);

	void OnRequestLogin(TCPSocket*& lpInClnData, unsigned int iInSize);
	void OnRequestRoomList(TCPSocket*& lpInClntData, unsigned int iInSize);
	void OnRequestCreateRoom(TCPSocket*& lpInClntData, unsigned int iInSize);
	void OnRequestJoinedRoom(TCPSocket*& lpInClntData, unsigned int iInSize);
	void OnRequestExitRoom(TCPSocket*& lpInClntData, unsigned int iInSize);
	void OnRequestChatting(TCPSocket*& lpInClntData, unsigned int iInSize);

	DWORD  CheckIOCompletionSocket(TCPSocket* & inSocket, TCPIOData* & inIOData);

public:
	void JIGAPPrintSystemLog(const char * fmt, ...);
	
	std::string JIGAPGetSystemMsg();
	bool JIGAPCheckSystemMsg() { return !qSystemMsg.empty(); };
public:
	void RemoveClientInServer(const SOCKET & hSock);
	void RemoveClientInRoom(TCPSocket* & lpSock);
	
};

