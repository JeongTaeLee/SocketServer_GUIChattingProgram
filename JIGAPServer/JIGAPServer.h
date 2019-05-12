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
	/*Server�� Intialize�մϴ�.*/
	HRESULT JIGAPInitializeServer();
	void JIGAPReleaseServer();

	void JIGAPConnectThread();
	void JIGAPChattingThread();
public:
	/*�ʱ� �۾����� �����մϴ�.*/
	bool JIGAPServerOpen(std::string szIpAddr, std::string szPortAddr);
	/*���� �۾����� �����մϴ�.*/
	void JIGAPServerClose();
public:
	/*�������ڰ� ������ System Log ��� �Լ� �Դϴ�.*/
	void JIGAPPrintSystemLog(const char * szInFormat, ...);

public:
	/*System Message Queue ���� �޽����� �Ѱ� ���ɴϴ�.*/
	std::string JIGAPGetSystemMsg();
	/*System Message Queue �� �޽����� �ִ��� Ȯ�� �մϴ� (TRUE : Message ����, FALSE : Message ����)*/
	bool JIGAPCheckSystemMsg() { return !qSystemMsg.empty(); };
public:

};

