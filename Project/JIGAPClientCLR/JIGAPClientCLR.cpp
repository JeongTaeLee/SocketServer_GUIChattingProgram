#include "pch.h"
#include "JIGAPClientCLR.h"

#include <msclr/marshal_cppstd.h>
#include <msclr/marshal.h>

JIGAPClientCLR::JIGAPClientWrap::JIGAPClientWrap()
	:lpJigapClient(new JIGAPClient)
{
}

JIGAPClientCLR::JIGAPClientWrap::~JIGAPClientWrap()
{
	delete lpJigapClient;
	lpJigapClient = nullptr;
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapClientStart(String^ szIpAddr, String^ szPortAddr)
{
	std::string ipAddr = msclr::interop::marshal_as < std::string >(szIpAddr);
	std::string portAddr = msclr::interop::marshal_as< std::string >(szPortAddr);

	return lpJigapClient->JIGAPClientStart(ipAddr, portAddr);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapClientEnd()
{
	
	lpJigapClient->JIGAPClientEnd();
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapRequestLogin(String^ szNickName)
{
	std::string nickName = msclr::interop::marshal_as<std::string>(szNickName);
	return lpJigapClient->JIGAPRequsetLogin(nickName);
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapRequestRoomList()
{
	return lpJigapClient->JIGAPRequestRoomList();
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnLoginCallBack(void(*lpInCallBack)())
{
	lpJigapClient->JIGAPSetOnLoginCallBack(lpInCallBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnLoginFailedCallBack(void(*lpInCallBack)())
{
	lpJigapClient->JIGAPSetOnLoginFailedCallBack(lpInCallBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnRoomListCallBack(void(*lpInCallBack)())
{
	lpJigapClient->JIGAPSetOnRoomListCallBack(lpInCallBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnCreateRoomCallBack(void(*lpInCallBack)())
{
	lpJigapClient->JIGAPSetOnCreateRoomCallBack(lpInCallBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnCreateRoomFailedCallBack(void(*lpInCallBack)())
{
	lpJigapClient->JIGAPSetOnCreateRoomFailedCallBack(lpInCallBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnJoinedRoomCallBack(void(*lpInCallBack)())
{
	lpJigapClient->JIGAPSetOnJoinedRoomCallBack(lpInCallBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnJoinedRoomFaileCallBack(void(*lpInCallBack)())
{
	lpJigapClient->JIGAPSetOnJoinedRoomFailedCallBack(lpInCallBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnExitRoomCallBack(void(*lpInCallBack)())
{
	lpJigapClient->JIGAPSetOnExitRoomCallBack(lpInCallBack);
}

String^ JIGAPClientCLR::JIGAPClientWrap::JIGAPGetMessageLog()
{
	String^ str = msclr::interop::marshal_as<String^>(lpJigapClient->JIGAPGetMessageLog());
	return str;
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPCheckMessage()
{
	return lpJigapClient->JIGAPCheckMessageLog();
}
