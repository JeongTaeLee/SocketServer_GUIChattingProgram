#pragma once

class JIGAPServer
{
private:
	LPTCPSOCK lpServSock;
	SerializeObject* lpSerializeObject;

	HANDLE hSystemLogMutex;	
	HANDLE hClientDataMutex;
	HANDLE hRoomsMutex;

	std::string szIpAddr;
	std::string szPortAddr;

	std::thread connectThread;
	std::thread chattingThread;

	std::map < std::string, LPROOM> mRooms;
	std::map < SOCKET, LPTCPSOCK > mClientData;
	
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

	void OnLogin(LPTCPSOCK & lpClntData);
	void OnRequestRoomList(LPTCPSOCK& lpClntData);
	void OnRequestCreateRoom(LPTCPSOCK & lpClntData);
	void OnRequestJoinedRoom(LPTCPSOCK & lpClntData);
	void OnRequestExtiRoom(LPTCPSOCK & lpClntData);
	void OnRequestChatting(LPTCPSOCK & lpClntData);

	int CheckIOCompletionSocket(LPTCPSOCK & inSocket, LPIODATA & inIOData);

public:
	void JIGAPPrintSystemLog(const char * fmt, ...);
	
	std::string JIGAPGetSystemMsg();
	bool JIGAPCheckSystemMsg() { return !qSystemMsg.empty(); };
public:
	void RemoveClient(const SOCKET & hSock);
	void RemoveClientToRoom(LPTCPSOCK & lpSock);
	
};

