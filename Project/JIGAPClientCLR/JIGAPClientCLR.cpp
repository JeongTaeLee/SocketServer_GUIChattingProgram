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

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnLoginCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);
	
	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnLoginCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnLoginFailedCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnJoinedRoomFailedCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnRoomListCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnRoomListCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnCreateRoomCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnCreateRoomCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnCreateRoomFailedCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnCreateRoomFailedCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnJoinedRoomCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnJoinedRoomCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnJoinedRoomFaileCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnJoinedRoomFailedCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapOnExitRoomCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnExitRoomCallBack(callBack);
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
