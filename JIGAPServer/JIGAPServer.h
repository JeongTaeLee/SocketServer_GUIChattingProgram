#pragma once

class JIGAPServer
{
private:
	LPHANDLE_DATA lpServData;
	
	std::string szIpAddr;
	std::string szPortAddr;

	std::thread connectThread;
	std::thread chattingThread;

	bool bServerOn;
public:
	JIGAPServer();
	virtual ~JIGAPServer();

private:
	/*Server를 Intialize합니다.*/
	HRESULT InitializeServer();
	/*Server를 Release 합니다*/
	void ReleaseServer();

	void ConnectThread();
	void ChattingThread(HANDLE hCompletion);

public:
	/*JIGAPServer::Intialize함수를 호출하고 초기 작업들을 수행합니다.*/
	bool JIGAPServerOpen(std::string szIpAddr, std::string szPortAddr);
	/*JIGAPServer::ReleaseServer 함수를 호출하고 정리 작업들을 수행합니다.*/
	void JIGAPServerClose();
};

