#pragma once
#include "BaseSocket.h"
#include "IOCPIOData.h"

namespace JIGAPServerAPI
{
	class IOCPSocket :
		public BaseSocket
	{
	protected:
		WSADATA wsaData;
		SOCKET hSocket = -1;

		IOCPIOData * iocpIoData = new IOCPIOData();

		int iSocketReferenceCount = 0;
		bool bRecving = false;
	public:
		IOCPSocket();
		IOCPSocket(SOCKET inSock);
		
		virtual bool InitializeSocket() override;
		virtual void CloseSocket() override;

		virtual bool Bind(unsigned short inPort) override;
		virtual bool Listen(int inQueueCount) override;

		virtual bool Connect(const std::wstring& inIp, unsigned short inPort) override;
		virtual BaseSocket* Accept() override;

		virtual void Recv() override;
		virtual void Send(const char *inSendBuffer, int inBufferSize) override;
	};
}
