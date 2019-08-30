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
		/* 서버 생성 시 사용하는 생성자입니다ㅏ.*/
		IOCPSocket();

		/* Accept 함수에서 클라이언트를 만들때 사용하는 생성자입니다.*/
		IOCPSocket(SOCKET inSock);
		
		virtual ~IOCPSocket();


		/* 소켓을 생성합니다.
		 * return 0		: 성공.
		 * return 그 외	: 실패.*/
		virtual int InitializeSocket() override;
		
		/* 소켓을 닫습니다.*/
		virtual void CloseSocket() override;

		/* 소켓을 할당 합니다.
		 * return 0		: 성공
		 * return 그 외	: 실패 */
		virtual int Bind(unsigned short inPort) override;

		/* 서버 대기열을 생성합니다..
		 * return 0		: 성공
		 * return 그 외	: 실패 */
		virtual int Listen(int inQueueCount) override;

		/* 서버와 연결합니다.
		 * return 0		: 성공
		 * return 그 외	: 실패 */
		virtual int Connect(const std::wstring& inIp, unsigned short inPort) override;
	
		/* 클라이언트 연결 요청을 받아옵니다.
		 * return 0		: 성공
		 * return 그 외	: 실패 */
		virtual BaseSocket* Accept() override;

		/* 수신 프로세스를 진행합니다
		 * return 0		: 성공
		 * return 그 외	: 실패 */
		virtual int Recv() override;
		
		/* 발신 프로세스를 진행합니다
		 * return 0		: 성공		 
		 * return 그 외	: 실패 */
		virtual int Send(const char *inSendBuffer, int inBufferSize) override;
	};
}
