#pragma once
class JIGAPClient
{
private:
	JIGAPSTATE eClientState;
	bool bLogin;

	LPTCPSOCK lpSocket;
	SerializeObject* lpSerializeObject;

	HANDLE hMessageMutex;

	std::thread recvThread;

	std::string strIpAddr;
	std::string strPortAddr;

	std::queue<std::string> qMessage;

	std::list<std::string> liRoomList;

	void (*lpOnLoginCallBack)();
	void (*lpOnRoomListCallBack)();
	void (*lpOnJoinedRoomList)();
	void (*lpOnExitRoomList)();
public:
	JIGAPClient();
	virtual ~JIGAPClient();

private:
	HRESULT JIGAPInitializeClient();
	void JIGAPReleaseClient();
public:
	bool JIGAPClientStart(const std::string & InIpAddr, const std::string & InPortAddr);
	void JIGAPClientEnd();

public:
	void JIGAPRecvThread();

	bool JIGAPRequsetLogin(const std::string & strInNickName);
	bool JIGAPRequestRoomList();
	bool JIGAPRequestCreateRoom(const std::string & strInRoomName);
	bool JIGAPRequestJoinedRoom(const std::string& strInRoomName);
	bool JIGAPRequestExtiRoom();

private:
	void JIGAPOnAnswerLogin();
	void JIGAPOnAnswerRoomList();
	void JIGAPOnAnswerCreateRoom();
	void JIGAPOnAnswerJoinedRoom();
	void JIGAPOnAnswerExtiRoom();


public:
	void JIGAPSetOnLoginCallBack(void (*lpInCallBack)()) { lpOnLoginCallBack = lpInCallBack; }
	void JIGAPSetOnRoomListCallBack(void (*lpInCallBack)()) { lpOnRoomListCallBack = lpInCallBack; }
	void JIGAPSetOnJoinedRoomCallBack(void (*lpInCallBack)()) { lpOnJoinedRoomList = lpInCallBack; }
	void JIGAPSetOnExitRoomCallBack(void (*lpInCallBack)()) { lpOnExitRoomList = lpInCallBack; }

private:
	void JIGAPPrintMessageLog(const char * fmt, ...);

	bool JIGAPSendSerializeBuffer();

public:
	bool JIGAPCheckMessageLog();
	std::string  JIGAPGetMessageLog();
};

