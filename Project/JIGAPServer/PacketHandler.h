#pragma once


#ifdef _DEBUG
#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotobuf-lited.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotobuf-lite.lib")
#endif


class Room;

struct PacketHeader
{
	int size;
	JIGAPPacket::Type packetType;
};

class PacketHandler
{
private:
	char * lpRecvStream;
	char * lpSendStream;

	// ���� ���ۿ� �Ҵ�� �� �����Դϴ�.
	unsigned int iRecvStreamSize;
	unsigned int iSendStreamSize;

	// ���� ��Ʈ������ ���� �������Դϴ�.
	unsigned int iRecvStreamPosition;
	// ���� ��Ʈ������ ���� �������Դϴ�.
	unsigned int iSendStreamPosition;

public:
	PacketHandler();
	~PacketHandler();

public:
	void ClearSendPacket();
	void SetRecvPacket(const char* ch, int iSize);
	
	int ParsingPacketSize(const char* buffer);
	void ParsingPacketHeader(PacketHeader& inHeader);
	
	template<class Packet>
	void ParsingPacket(Packet& inPaket, int inSize)
	{
		inPaket.ParseFromArray(&lpRecvStream[iRecvStreamPosition], inSize);
		iRecvStreamPosition += inSize;
	}

public:
	void SerializePacketSize(int iPacketSize);

	template<class Packet>
	void SerializePacket(JIGAPPacket::Type inType, Packet& inPacket)
	{
		memcpy(&lpSendStream[iSendStreamPosition], (void*)& inType, sizeof(int));
		iSendStreamPosition += sizeof(int);
		memcpy(&lpSendStream[iSendStreamPosition], (void*)& inPacket.ByteSize(), sizeof(int));
		iSendStreamPosition += sizeof(int);

		inPacket.SerializeToArray(&lpSendStream[iSendStreamPosition], inPacket.ByteSize());

	}
};

