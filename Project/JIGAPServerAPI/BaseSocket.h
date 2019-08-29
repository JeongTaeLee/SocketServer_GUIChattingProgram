#pragma once

namespace JIGAPServerAPI
{
	class BaseSocket
	{
	protected:
		unsigned __int64 iIdNumber = 0;

	public:
		BaseSocket();
		BaseSocket(unsigned __int64 inIdNumber);
		virtual ~BaseSocket();

		virtual bool InitializeSocket() = 0;
		virtual void CloseSocket() = 0;

		virtual bool Bind(unsigned short inPort) = 0;
		virtual bool Listen(int inQueueCount) = 0;

		virtual bool Connect(const std::wstring & inIp, unsigned short inPort) = 0;
		virtual BaseSocket * Accept() = 0;

		virtual void Recv() = 0;
		virtual void Send(const char* inSendBuffer, int inBufferSize) = 0;

	public:
		unsigned __int64 GetIdNumber() { return iIdNumber; }
		void SetIdNumber(unsigned __int64 inIdNumber) { iIdNumber = inIdNumber; }
	};
}
