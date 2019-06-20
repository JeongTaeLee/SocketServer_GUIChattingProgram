#pragma once
typedef void(__stdcall* PROGRESS)(void);
typedef void(__stdcall* PROGRESS_CHATTING)(const char* sender, const char* message, int senderSize, int messageSize);
typedef void(__stdcall* PROGRESS_JOINEDROOM)(const char* szInRoomName, int iInLen);

class TCPSocket;
class PacketHandler;

class JIGAPClient
{
private:
	bool bLogin;

	TCPSocket * lpSocket;
	PacketHandler* lpPacketHandler;

	std::string strIpAddr;
	std::string strPortAddr;

	std::vector<std::string> liRoomList;
public:
	JIGAPClient();
	virtual ~JIGAPClient();

private:
	HRESULT JIGAPInitializeClient();
	void JIGAPReleaseClient();
public:
	bool JIGAPClientStart(const std::string& InIpAddr, const std::string& InPortAddr);
	void JIGAPClientEnd();

	void JIGAPRecvThread();



};