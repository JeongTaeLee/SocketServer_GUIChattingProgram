#pragma once
class JIGAPClient
{
private:
	LPHANDLE_DATA lpHandleData;

	std::thread lpRecvThread;
	std::thread lpSendThread;

	std::string strIpAddr;
	std::string strPortAddr;

	std::queue<std::string> qMessageQueue;

public:
	JIGAPClient();
	virtual ~JIGAPClient();

private:
	HRESULT InitializeClient();
	HRESULT ReleaseClient();

public:
	bool JIGAPClientStart(const std::string & InIpAddr, const std::string & InPortAddr);
	void JIGAPClientEnd();

};

