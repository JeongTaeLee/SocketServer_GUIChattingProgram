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
	/*Server�� Intialize�մϴ�.*/
	HRESULT InitializeServer();
	/*Server�� Release �մϴ�*/
	void ReleaseServer();

	void ConnectThread();
	void ChattingThread();
public:
	/*JIGAPServer::Intialize�Լ��� ȣ���ϰ� �ʱ� �۾����� �����մϴ�.*/
	bool JIGAPServerOpen(std::string szIpAddr, std::string szPortAddr);
	/*JIGAPServer::ReleaseServer �Լ��� ȣ���ϰ� ���� �۾����� �����մϴ�.*/
	void JIGAPServerClose();

	void PrintSystemLog(const std::string & key);
public:
	/*System Message Queue ���� �޽����� �Ѱ� ���ɴϴ�.*/
	std::string JIGAPGetSystemMsg();

	/*System Message Queue �� �޽����� �ִ��� Ȯ�� �մϴ� (TRUE : Message ����, FALSE : Message ����)*/
	bool JIGAPCheckSystemMsg() { return !qSystemMsg.empty(); };
};

