#pragma once
typedef void(__stdcall* PROGRESS)(void);
typedef void(__stdcall* PROGRESS_CHATTING)(const char* sender, const char* message, int senderSize, int messageSize);
typedef void(__stdcall* PROGRESS_JOINEDROOM)(const char* szInRoomName, int iInLen);

class TCPSocket;
class SerializeObject;
class PacketHandler;

class JIGAPClient
{
private:
	JIGAPSTATE eClientState;
	bool bLogin;

	TCPSocket * lpSocket;
	SerializeObject* lpSerializeObject;
	PacketHandler* lpPacketHandler;

	HANDLE hMessageMutex;

	std::thread recvThread;

	std::string strIpAddr;
	std::string strPortAddr;
	std::string strMyName;

	std::queue<std::string> qMessage;

	std::vector<std::string> liRoomList;

	std::function<void()> lpOnLoginCallBack;
	std::function<void()> lpOnLoginFailedCallBack;
	std::function<void()> lpOnRoomListCallBack;
	std::function<void()> lpOnCreateRoomCallBack;
	std::function<void()> lpOnCreateRoomFailedCallBack;
	std::function<void(const char* szInRoomName, int iInLen)> lpOnJoinedRoomCallBack;
	std::function<void()> lpOnJoinedRoomFailedCallBack;
	std::function<void()> lpOnExitRoomCallBack;
	std::function<void(const char* sender, const char* message, int senderSize, int messageSize)> lpOnChattingCallBack;
public:
	JIGAPClient();
	virtual ~JIGAPClient();

private:
	HRESULT JIGAPInitializeClient();
	void JIGAPReleaseClient();
public:
	bool JIGAPClientStart(const std::string& InIpAddr, const std::string& InPortAddr);
	void JIGAPClientEnd();

public:
	void JIGAPRecvThread();

	bool JIGAPRequsetLogin(const std::string& strInNickName);
	bool JIGAPRequestRoomList();
	bool JIGAPRequestCreateRoom(const std::string& strInRoomName);
	bool JIGAPRequestJoinedRoom(const std::string& strInRoomName);
	bool JIGAPRequestExtiRoom();
	bool JIGAPRequestChatting(const std::string & strInMessage);
private:
	void JIGAPOnAnswerLogin(unsigned int iInSize);
	void JIGAPOnAnswerRoomList(unsigned int iInSize);
	void JIGAPOnAnswerCreateRoom(unsigned int iInSize);
	void JIGAPOnAnswerJoinedRoom(unsigned int iInSize);
	void JIGAPOnAnswerExtiRoom(unsigned int iInSize);
	void JIGAPOnAnswerChatting(unsigned int iInSize);
public:
	void JIGAPSetOnLoginCallBack(PROGRESS lpInCallBack) { lpOnLoginCallBack = lpInCallBack; }
	void JIGAPSetOnLoginFailedCallBack(PROGRESS lpInCallBack) { lpOnLoginFailedCallBack = lpInCallBack; }
	void JIGAPSetOnRoomListCallBack(PROGRESS lpInCallBack) { lpOnRoomListCallBack = lpInCallBack; }
	void JIGAPSetOnCreateRoomCallBack(PROGRESS lpInCallBack) { lpOnCreateRoomCallBack = lpInCallBack; }
	void JIGAPSetOnCreateRoomFailedCallBack(PROGRESS lpInCallBack) { lpOnCreateRoomFailedCallBack = lpInCallBack; }
	void JIGAPSetOnJoinedRoomCallBack(PROGRESS_JOINEDROOM lpInCallBack) { lpOnJoinedRoomCallBack = lpInCallBack; }
	void JIGAPSetOnJoinedRoomFailedCallBack(PROGRESS lpInCallBack) { lpOnJoinedRoomFailedCallBack = lpInCallBack; }
	void JIGAPSetOnExitRoomCallBack(PROGRESS lpInCallBack) { lpOnExitRoomCallBack = lpInCallBack; }
	void JIGAPSetOnChattingCallBack(PROGRESS_CHATTING lpInCallBack) { lpOnChattingCallBack = lpInCallBack; }

private:
	void JIGAPPrintMessageLog(const char* fmt, ...);
	bool JIGAPSendSerializeBuffer(const char* szInMessage, unsigned int iInSize);

public:
	bool JIGAPCheckMessageLog();
	std::string  JIGAPGetMessageLog();
	std::vector<std::string>& JIGAPGetRoomList() { return liRoomList; }
};