#include "pch.h"
#include "BaseUserData.h"

void BaseUserData::OnActiveObject()
{
	strUserId = "None";
	lpTCPSocket = nullptr;

}

void BaseUserData::OnUnActiveObject()
{
	strUserId = "None";
	lpTCPSocket = nullptr;
}

void BaseUserData::CloseTCPSocket()
{
	lpTCPSocket->Closesocket();
	SAFE_DELETE(lpTCPSocket);
}

