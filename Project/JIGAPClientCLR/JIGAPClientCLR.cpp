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

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapRequsetCreateRoom(String^ szRoomName)
{
	return lpJigapClient->JIGAPRequestCreateRoom(msclr::interop::marshal_as<std::string>(szRoomName));
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapRequestJoeindRoom(String^ szRoomName)
{
	return lpJigapClient->JIGAPRequestJoinedRoom(msclr::interop::marshal_as<std::string>(szRoomName));
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapRequestExitRoom()
{
	return lpJigapClient->JIGAPRequestExtiRoom();
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapRequestChattingMessage(String^ szString)
{
	return lpJigapClient->JIGAPRequestChatting(msclr::interop::marshal_as <std::string>(szString));
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSetLoginCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);
	
	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnLoginCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSetLoginFailedCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnLoginFailedCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSetRoomListCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnRoomListCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSetCreateRoomCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnCreateRoomCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSetCreateRoomFailedCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnCreateRoomFailedCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSetJoinedRoomCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnJoinedRoomCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSetJoinedRoomFaileCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnJoinedRoomFailedCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSetExitRoomCallBack(PROGRESS_CSHARP^ fucn)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fucn);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fucn);
	PROGRESS callBack = static_cast<PROGRESS>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnExitRoomCallBack(callBack);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSetChattingCallBack(PROGRESS_CSHARP_CHATTING^ func)
{
	System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(func);

	IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(func);
	PROGRESS_CHATTING callBack = static_cast<PROGRESS_CHATTING>(ip.ToPointer());
	lpJigapClient->JIGAPSetOnChattingCallBack(callBack);
}


void JIGAPClientCLR::JIGAPClientWrap::JIGAPGetRoomList(cli::array<String^>^% arr)
{
	std::vector<std::string> & liStr = lpJigapClient->JIGAPGetRoomList();

	arr = gcnew cli::array<String^>(liStr.size());
	
	for (int i = 0; i < 40; ++i)
	{
		if (i < liStr.size())
			arr[i] = msclr::interop::marshal_as<String^>(liStr[i]);
		else
			break;
	}
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
