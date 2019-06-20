﻿#pragma once

#include "../JIGAPClient/JIGAPClient.h"


using namespace System;
public delegate void PROGRESS_CSHARP(void);

public delegate void PROGRESS_CSHARP_CHATTING(const byte* sender, const byte* message, int senderSize, int messageSize);
public delegate void PROGRESS_CSHARP_JOINEDROOM(const byte* lpInRoomName, int iLength);

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

		/*
	public:
		bool JIGAPWrapRequestLogin(String^ szNickName);
		bool JIGAPWrapRequestRoomList();
		bool JIGAPWrapRequsetCreateRoom(String ^ szRoomName);
		bool JIGAPWrapRequestJoeindRoom(String ^ szRoomName);
		bool JIGAPWrapRequestExitRoom();
		bool JIGAPWrapRequestChattingMessage(String ^ szString);

	public:
		void JIGAPWrapSetLoginCallBack			(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapSetLoginFailedCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapSetRoomListCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapSetCreateRoomCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapSetCreateRoomFailedCallBack(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapSetJoinedRoomCallBack		(PROGRESS_CSHARP_JOINEDROOM^fucn);
		void JIGAPWrapSetJoinedRoomFaileCallBack	(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapSetExitRoomCallBack		(PROGRESS_CSHARP ^fucn);
		void JIGAPWrapSetChattingCallBack(PROGRESS_CSHARP_CHATTING^ func);
	

		void JIGAPGetRoomList(cli::array<String^>^% arr);
		*/
	};
}
