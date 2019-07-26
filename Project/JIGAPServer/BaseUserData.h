#pragma once
#include "PoolObject.h"

class TCPSocket;

class BaseUserData : public PoolObject
{
protected:
	std::string strUserId = "None";
	TCPSocket* lpTCPSocket = nullptr;

	bool bLogin = false;
public:
	BaseUserData() {}
	virtual ~BaseUserData() {}

	virtual void OnActiveObject() override;
	virtual void OnUnActiveObject() override;

	const std::string& GetUserID() { return strUserId; }
	TCPSocket* GetTCPSocket() { return lpTCPSocket; }
	bool GetLogin() { return bLogin; }

	void SetLogin(bool bInLogin) { bLogin = bInLogin;}
	void SetUserID(const std::string& strInUserId) { strUserId = strInUserId; }
	void SetTCPSock(TCPSocket* lpInTCPSocket) { lpTCPSocket = lpInTCPSocket; }
};

