#pragma once

#include "../JIGAPClient/JIGAPClient.h"
public delegate void PROGRESS_CSHARP(void);
using namespace System;


namespace JIGAPClientCLR 
{
	public ref class JIGAPClientWrap
	{
	protected:
		JIGAPClient* lpJigapClient;
	public:
		JIGAPClientWrap();
		~JIGAPClientWrap();

		bool JIGAPWrapClientStart(String^ szIpAddr, String^ szPortAddr);
		void JIGAPWrapClientEnd();

	public:
		bool JIGAPWrapRequestLogin(String^ szNickName);
		bool JIGAPWrapRequestRoomList();
		bool JIGAPWrapRequsetCreateRoom(String ^ szRoomName);
		bool JIGAPWrapRequestJoeindRoom(String ^ szRoomName);
		bool JIGAPWrapRequestExitRoom();

	public:
		void JIGAPWrapOnLoginCallBack			(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnLoginFailedCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnRoomListCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnCreateRoomCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnCreateRoomFailedCallBack(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnJoinedRoomCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnJoinedRoomFaileCallBack	(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnExitRoomCallBack		(PROGRESS_CSHARP ^fucn);
	

		void JIGAPGetRoomList(cli::array<String^>^% arr);
		void JIGAPWrapGetRoomList(System::Collections::Generic::List<String^> liStr);
	public:
		String^ JIGAPGetMessageLog();
		bool JIGAPCheckMessage();
		
	};
}
