#pragma once

typedef void (*LogFunc)(int* strLog);

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


class TCPSocket;
class JIGAPBaserProcess;
class PacketHandler;

class JIGAPServer
{
private:
	TCPSocket* lpServerSocket	= nullptr;
	HANDLE hCompletionHandle	= nullptr;
	ServerData serverData		= {};

	std::thread hConnectThread;
	std::thread hRecvThread;

	PacketHandler* lpPacketHandler		= nullptr;
	JIGAPBaserProcess* lpServerProcess	= nullptr;

	bool bServerOn = false;

	//C# ���� CalLBack
private:
	LogFunc lpLogFunc = nullptr;

private:
	bool CreateServerSocket();

	bool ServerInitialize(const std::string& inIpAddress, const std::string& inPortAddress);
	void ServerRelease();
public:
	bool StartServer(const std::string& inIpAddress, const std::string& inPortAddress);
	void CloseServer();

	void OnConnectTask();
	void OnRecvPacketTask();

public:
	void RegisterServerLog(const char* fmt, ...);

public:
	void RegisterLogFunc(void* lpFuncPointer);
};

