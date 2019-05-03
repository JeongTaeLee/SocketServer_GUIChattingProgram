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
	/*Server�� Intialize�մϴ�.*/
	HRESULT InitializeServer();
	/*Server�� Release �մϴ�*/
	void ReleaseServer();

	void ConnectThread();
	void ChattingThread(HANDLE hCompletion);

public:
	/*JIGAPServer::Intialize�Լ��� ȣ���ϰ� �ʱ� �۾����� �����մϴ�.*/
	bool JIGAPServerOpen(std::string szIpAddr, std::string szPortAddr);
	/*JIGAPServer::ReleaseServer �Լ��� ȣ���ϰ� ���� �۾����� �����մϴ�.*/
	void JIGAPServerClose();
};

