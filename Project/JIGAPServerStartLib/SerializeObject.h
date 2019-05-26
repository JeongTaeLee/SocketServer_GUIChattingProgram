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
	void SerializeDataSendBuffer(const T& object) 
	{
		int dataSize = sizeof(object);
		sendStreamSize += dataSize;

		memcpy(sendStreamHead, (char*)&object, dataSize);
		sendStreamHead = &sendStreamBuffer[sendStreamSize];
	}
	template<class T>
	void SerializeDataSendBuffer(const T& object, int size)
	{
		int dataSize = size;
		sendStreamSize += dataSize;

		memcpy(sendStreamHead, (char*)& object, dataSize);
		sendStreamHead = &sendStreamBuffer[sendStreamSize];
	}

	template<class T>
	void DeserializeRecvBuffer(const T& object)
	{
		int dataSize = sizeof(object);
		recvStreamSize += dataSize;

		memcpy((char*)& object, recvStreamHead, dataSize);
		recvStreamHead = &recvStreamBuffer[recvStreamSize];
	}

	template<class T>
	void DeserializeRecvBuffer(const T& object, int size)
	{
		int dataSize = size;
		recvStreamSize += dataSize;

		memcpy((char*)& object, recvStreamHead, dataSize);
		recvStreamHead = &recvStreamBuffer[recvStreamSize];
		recvStreamBuffer[size - 1] = NULL;
	}

	void SetRecvStreamBuffer(char* lpBuffer);
	const char* GetSendStreamBuffer() { return sendStreamBuffer; }

	void ClearSendStreamBuffer();
	void ClearRecvStreamBuffer();

	int GetSendStreamSize() { return sendStreamSize; }
	int GetRecvStreamSize() { return recvStreamSize; }

};

