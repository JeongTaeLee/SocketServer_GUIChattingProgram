#pragma once

#define MAXBUFFERLENGTH 2048

namespace JIGAPServerAPI
{
	enum IOMODE
	{
		E_IOMODE_RECV,
		E_IOMODE_SEND,
	};

	class IOCPIOData : public OVERLAPPED
	{
	private:
		IOMODE eIoMode = E_IOMODE_RECV;
		WSABUF wsaBuf;
		
		char  szBuffer[MAXBUFFERLENGTH];
	
	public:
		IOCPIOData();

		friend class IOCPSocket;
	};

}

