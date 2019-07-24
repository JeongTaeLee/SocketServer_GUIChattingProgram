#pragma once

class TCPSocket;

class BaseUserData abstract
{
protected:
	std::string strUserId = "None";
	TCPSocket* lpTCPSocket = nullptr;
public:

	void DisconnectUser();

	void SetUserID(const std::string& strInUserId) { strUserId = strInUserId; }
	void SetTCPSock(TCPSocket* lpInTCPSocket) { lpTCPSocket = lpInTCPSocket; }
};

