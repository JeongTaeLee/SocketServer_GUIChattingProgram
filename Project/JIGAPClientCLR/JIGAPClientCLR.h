#pragma once

#include "../JIGAPClient/JIGAPClient.h"

using namespace System;

namespace JIGAPClientCLR 
{
	delegate void JIGAPEventCallBack();

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
		void JIGAPWrapOnLoginCallBack(void (*lpInCallBack)());
		void JIGAPWrapOnLoginFailedCallBack(void (*lpInCallBack)());
		void JIGAPWrapOnRoomListCallBack(void (*lpInCallBack)());
		void JIGAPWrapOnCreateRoomCallBack(void (*lpInCallBack)());
		void JIGAPWrapOnCreateRoomFailedCallBack(void (*lpInCallBack)());
		void JIGAPWrapOnJoinedRoomCallBack(void (*lpInCallBack)());
		void JIGAPWrapOnJoinedRoomFaileCallBack(void (*lpInCallBack)());
		void JIGAPWrapOnExitRoomCallBack(void (*lpInCallBack)());
	
	public:
		String^ JIGAPGetMessageLog();
		bool JIGAPCheckMessage();
		
	};
}
