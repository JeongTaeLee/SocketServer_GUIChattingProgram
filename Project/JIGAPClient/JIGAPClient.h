#pragma once
class JIGAPClient
{
private:
	LPTCPSOCK lpSocket;

	HANDLE hMessageMutex;

	std::thread recvThread;

	std::string strIpAddr;
	std::string strPortAddr;

	std::queue<std::string> qMessage;
public:
	JIGAPClient();
	virtual ~JIGAPClient();

private:
	HRESULT JIGAPInitializeClient();
	void JIGAPReleaseClient();
public:
	/*ä�� Ŭ���̾�Ʈ�� �����մϴ� initializeClient() �� Thread ���� ���� ó���մϴ�.*/
	bool JIGAPClientStart(const std::string & InIpAddr, const std::string & InPortAddr);
	
	/*ä�� Ŭ���̾�Ʈ�� �����մϴ� ReleaseClient() �� ���� �ڿ� ���� ���� ó���մϴ�.*/
	void JIGAPClientEnd();
public:
	void JIGAPRecvThread();
	bool JIGAPSend(std::string szInMessage);

public:
	void JIGAPPrintMessageLog(const char * fmt, ...);

public:
	bool JIGAPCheckMessageLog();
	std::string  JIGAPGetMessageLog();
};

