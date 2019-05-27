#pragma once

#include "../JIGAPClient/JIGAPClient.h"
public delegate void PROGRESS_CSHARP(void);
using namespace System;

namespace JIGAPClientCLR 
{
	public delegate void JIGAPEventCallBack();


	public enum class LITERAL
	{
		LiteralLogin,
		LiteralJoinedRoom,
		LiteralExitRoom,
		LiteralChatting,
	};

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
	public:
		void JIGAPWrapOnLoginCallBack			(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnLoginFailedCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnRoomListCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnCreateRoomCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnCreateRoomFailedCallBack(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnJoinedRoomCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnJoinedRoomFaileCallBack	(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapOnExitRoomCallBack		(PROGRESS_CSHARP ^fucn);
	
	public:
		String^ JIGAPGetMessageLog();
		bool JIGAPCheckMessage();
		
	};
}
