#pragma once

class JIGAPServer
{
private:
	LPTCPSOCK lpServSock;

	HANDLE hSystemLogMutex;	

	std::string szIpAddr;
	std::string szPortAddr;

	std::thread connectThread;
	std::thread chattingThread;

	std::list  < LPTCPSOCK > liClientData;
	std::queue < std::string > qSystemMsg;

	bool bServerOn;
public:
	HANDLE hCompletionHandle;

public:
	JIGAPServer();
	virtual ~JIGAPServer();

private:
	/*Server를 Intialize합니다.*/
	HRESULT JIGAPInitializeServer();
	void JIGAPReleaseServer();

	void JIGAPConnectThread();
	void JIGAPChattingThread();
public:
	/*초기 작업들을 수행합니다.*/
	bool JIGAPServerOpen(std::string szIpAddr, std::string szPortAddr);
	/*정리 작업들을 수행합니다.*/
	void JIGAPServerClose();
public:
	/*가변인자가 가능한 System Log 출력 함수 입니다.*/
	void JIGAPPrintSystemLog(const char * szInFormat, ...);

public:
	/*System Message Queue 에서 메시지를 한개 빼옵니다.*/
	std::string JIGAPGetSystemMsg();
	/*System Message Queue 에 메시지가 있는지 확인 합니다 (TRUE : Message 있음, FALSE : Message 없음)*/
	bool JIGAPCheckSystemMsg() { return !qSystemMsg.empty(); };
public:

};

