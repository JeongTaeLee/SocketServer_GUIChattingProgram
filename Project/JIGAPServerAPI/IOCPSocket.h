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
		/* ���� ���� �� ����ϴ� �������Դϴ٤�.*/
		IOCPSocket();

		/* Accept �Լ����� Ŭ���̾�Ʈ�� ���鶧 ����ϴ� �������Դϴ�.*/
		IOCPSocket(SOCKET inSock);
		
		virtual ~IOCPSocket();


		/* ������ �����մϴ�.
		 * return 0		: ����.
		 * return �� ��	: ����.*/
		virtual int InitializeSocket() override;
		
		/* ������ �ݽ��ϴ�.*/
		virtual void CloseSocket() override;

		/* ������ �Ҵ� �մϴ�.
		 * return 0		: ����
		 * return �� ��	: ���� */
		virtual int Bind(unsigned short inPort) override;

		/* ���� ��⿭�� �����մϴ�..
		 * return 0		: ����
		 * return �� ��	: ���� */
		virtual int Listen(int inQueueCount) override;

		/* ������ �����մϴ�.
		 * return 0		: ����
		 * return �� ��	: ���� */
		virtual int Connect(const std::wstring& inIp, unsigned short inPort) override;
	
		/* Ŭ���̾�Ʈ ���� ��û�� �޾ƿɴϴ�.
		 * return 0		: ����
		 * return �� ��	: ���� */
		virtual BaseSocket* Accept() override;

		/* ���� ���μ����� �����մϴ�
		 * return 0		: ����
		 * return �� ��	: ���� */
		virtual int Recv() override;
		
		/* �߽� ���μ����� �����մϴ�
		 * return 0		: ����		 
		 * return �� ��	: ���� */
		virtual int Send(const char *inSendBuffer, int inBufferSize) override;
	};
}
