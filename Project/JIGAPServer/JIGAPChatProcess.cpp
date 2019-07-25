#include "pch.h"
#include "JIGAPChatProcess.h"
#include "UserDataAdmin.h"
#include "ChatUserData.h"


void JIGAPChatProcess::OnInitialize()
{
	lpUserAdmin = new UserDataAdmin<ChatUserData>();
	lpUserAdmin->InitializeAdmin(100000);
}

void JIGAPChatProcess::OnRelease()
{
	lpUserAdmin->ReleaseAdmin();
	SAFE_DELETE(lpUserAdmin);
}

void JIGAPChatProcess::OnConnect(TCPSocket* lpInSocket)
{
}

void JIGAPChatProcess::OnDisconnect(TCPSocket* lpInSocket)
{
}

void JIGAPChatProcess::OnProcess(TCPSocket* lpInTCPSocket, PacketHandler* lpHandler)
{
}

