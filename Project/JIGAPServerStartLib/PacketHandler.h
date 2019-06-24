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
	unsigned int size;
	JIGAPPacket::PacketType packetType;
};

class PacketHandler
{
private:
	char * lpRecvStream;
	char * lpSendStream;

	// 현재 버퍼에 할당된 총 길이입니다.
	unsigned int iRecvStreamSize;
	unsigned int iSendStreamSize;

	// 현재 스트림에서 읽은 사이즈입니다.
	unsigned int iRecvStreamPosition;
	// 현재 스트림에서 써진 사이즈입니다.
	unsigned int iSendStreamPosition;

public:
	PacketHandler();
	~PacketHandler();

public:
	void ClearSendPacket();
	void SetRecvPacket(const char* ch, int iSize);
	
	int ParsingPacketSize(const char* buffer);
	void ParsingPacketHeader(JIGAPPacket::MessageHeader & inHeader);
	
	template<class Packet>
	void ParsingPacket(Packet& inPaket, int inSize)
	{
		inPaket.ParseFromArray(&lpRecvStream[iRecvStreamPosition], inSize);
		iRecvStreamPosition += inSize;
	}

public:
	void SerializePacketSize(int iPacketSize);

	template<class Packet>
	void SerializePacket(JIGAPPacket::PacketType inType, Packet& inPaket)
	{
		JIGAPPacket::MessageHeader header;
		header.set_type(inType);
		header.set_size(inPaket.ByteSize());

		inHeader.SerializeToArray(&lpSendStream[iSendStreamPosition], header.ByteSize());
		iSendStreamPosition += header.ByteSize();

		inPaket.SerializeToArray(&lpSendStream[iSendStreamPosition], header.size());
		iSendStreamPosition += header.size();
	}
};

