#pragma once

class JIGAPServer
{
private:
	LPHANDLE_DATA lpServData;

	HANDLE hSystemLogMutex;	
	HANDLE hThreadMutex;

	std::string szIpAddr;
	std::string szPortAddr;

	std::thread connectThread;
	std::thread chattingThread;

	std::list  < LPHANDLE_DATA > liHandleData;
	std::queue < std::string > qSystemMsg;

	bool bServerOn;
public:
	HANDLE hCompletionHandle;

public:
	JIGAPServer();
	virtual ~JIGAPServer();

private:
	/*Server를 Intialize합니다.*/
	HRESULT InitializeServer();
	/*Server를 Release 합니다*/
	void ReleaseServer();

	void ConnectThread();
	void ChattingThread();
public:
	/*JIGAPServer::Intialize함수를 호출하고 초기 작업들을 수행합니다.*/
	bool JIGAPServerOpen(std::string szIpAddr, std::string szPortAddr);
	/*JIGAPServer::ReleaseServer 함수를 호출하고 정리 작업들을 수행합니다.*/
	void JIGAPServerClose();

	void PrintSystemLog(const std::string & key);
public:
	/*System Message Queue 에서 메시지를 한개 빼옵니다.*/
	std::string JIGAPGetSystemMsg();

	/*System Message Queue 에 메시지가 있는지 확인 합니다 (TRUE : Message 있음, FALSE : Message 없음)*/
	bool JIGAPCheckSystemMsg() { return !qSystemMsg.empty(); };
};

