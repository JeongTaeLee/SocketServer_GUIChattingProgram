#pragma once
class SerializeObject
{
private:
	char sendStreamBuffer[MAXBUFFERSIZE];
	char recvStreamBuffer[MAXBUFFERSIZE];

	char* sendStreamHead;	
	char* recvStreamHead;

	int sendStreamSize;
	int recvStreamSize;
public:
	SerializeObject();
	~SerializeObject();

	template<class T>
	void SerializeDataSendBuffer(T& object) 
	{
		int dataSize = sizeof(object);
		sendStreamSize += dataSize;

		memcpy(sendStreamHead, (char*)&object, dataSize);
		sendStreamHead = sendStreamBuffer[sendStreamHead];
	}

	template<class T>
	void DeserializeRecvBuffer(T& object)
	{
		int dataSize = sizeof(object);
		recvStreamSize += dataSize;

		memcpy((char*)& object, recvStreamHead, dataSize);
		recvStreamHead = recvStreamBuffer[recvStreamSize];
	}

	template<class T>
	void DeserializeRecvBuffer(T& object, int size)
	{
		int dataSize = size;
		recvStreamSize += dataSize;

		memcpy((char*)& object, recvStreamHead, dataSize);
		recvStreamHead = recvStreamBuffer[recvStreamSize];
	}

	void SetRecvStreamBuffer(char* lpBuffer);

	void ClearSendStreamBuffer();
	void ClearRecvStreamBuffer();
};

