#pragma once

#include "../JIGAPClient/JIGAPClient.h"

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
		String^ JIGAPGetMessageLog();
		bool JIGAPCheckMessage();
		
	};
}
