#pragma once
#include "../JIGAPServer/JIGAPServer.h"

using namespace System;

namespace JIGAPServerCLR {

	public ref class JIGAPServerWrap
	{
	protected:
		JIGAPServer* lpJIGAPServer;
	public:
		JIGAPServerWrap();
		virtual ~JIGAPServerWrap();

		bool JIGAPWrapServerOpen(String ^ szIpAddr, String^ szPortAddr);
		void JIGAPWrapServerClose();
	};
}
