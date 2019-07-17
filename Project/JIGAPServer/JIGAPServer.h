#pragma once

struct ServerData
{
private:
	std::string strIpAddress;
	std::string strPortAddress;

public:
	ServerData() {}
	~ServerData() {}

	void SetServerData(const std::string& inStrIpAddress, const std::string& inStrPortAddress)
	{
		strIpAddress = inStrIpAddress;
		strPortAddress = inStrPortAddress;
	}

	const std::string& GetIpAdress() { return strIpAddress; }
	const std::string& GetPortAddress() { return strPortAddress; }
};

using SerialNumber = SOCKET;

class TCPSocket;

class JIGAPServer
{
private:
	TCPSocket* lpServerSocket	= nullptr;
	HANDLE hCompletionHandle	= nullptr;
	ServerData serverData		= {};

	std::map<SerialNumber, TCPSocket*> mUsers;

	HANDLE hServerLogMutex	= nullptr;
	HANDLE hUsersMapMutex	= nullptr;
	HANDLE hWorkMutex		= nullptr;

	std::thread hConnectThread;
	std::thread hRecvThread;
	std::thread hSendThread;

	std::queue<std::string> qServerLog;


private:
	bool CreateServerSocket();

public:
	bool ServerInitialize(const std::string& inIpAddress, const std::string& inPortAddress);
	void ServerRelease();
	
	void OnConnectTask();
	void OnRecvPacketTask();
	void OnSendPacketTask();

public:
	void RegisterServerLog(const char* fmt, ...);
	std::string PopServerLog();
};

