#pragma once
#include "PoolObject.h"

class TCPSocket;

class BaseUserData : public PoolObject
{
protected:
	std::string strUserId = "None";
	TCPSocket* lpTCPSocket = nullptr;
public:
	BaseUserData() {}
	virtual ~BaseUserData() {}

	virtual void OnActiveObject() override;
	virtual void OnUnActiveObject() override;

	void SetUserID(const std::string& strInUserId) { strUserId = strInUserId; }
	void SetTCPSock(TCPSocket* lpInTCPSocket) { lpTCPSocket = lpInTCPSocket; }
};

