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

	// 받은 패킷의 사이즈입니다.
	unsigned int iSizeOfRecvPacket;
public:
	PacketHandler();
	~PacketHandler();

private:
	void ClearRecvStream();
public:
	//------------------------------------------------------//
	// 소켓에서 받아온 패킷을 핸들러에 저장합니다.
	//------------------------------------------------------//
	void SetPacketInRecvStream(char* lpInBuffer, int size);

	//------------------------------------------------------//
	// lpInBuffer에서 처음 4 바이트를 읽어 메시지의 총 
	// 사이즈를 알아옵니다.
	//------------------------------------------------------//
	int ParsingPacketSize(const char* lpInBuffer);

	//------------------------------------------------------//
	// 설정한 iSendStream에서 패킷 사이즈만큼 패킷을 읽어옵니다.
	//------------------------------------------------------//
	template <class Packet>
	bool ParsingPacket(Packet& inPacket, unsigned int inSize)
	{
		inPacket.ParseFromArray(&lpRecvStream[iRecvStreamPosition], inSize);
		iRecvStreamPosition += inSize;
		return true;
	}
	
	//------------------------------------------------------//
	// 설정한 iSendStream에서 다음 패킷의 헤더를 읽어옵니다.
	//------------------------------------------------------//
	void ParsingPacketHeader(PacketHeader& inHeader);

public:
	//------------------------------------------------------//
	// 패킷헤더와 패킷을 iSendStream에 직렬화 합니다.
	//------------------------------------------------------//
	template <class Packet>
	bool SerializePacket(JIGAPPacket::PacketType eInType, Packet& inPacket)
	{
		PacketHeader header;
		header.packetType = eInType;
		header.size = inPacket.ByteSize();

		int headerSize = sizeof(header);

		memcpy(&lpSendStream[iSendStreamPosition], (char*)& header, headerSize);
		iSendStreamPosition += headerSize;

		inPacket.SerializeToArray(&lpSendStream[iSendStreamPosition], iSendStreamSize - iSendStreamPosition);
		iSendStreamPosition += inPacket.ByteSize();

		SerializeTotalPacketSize(iSendStreamPosition);

		return true;
	}
	//------------------------------------------------------//
	// 답변할 패킷에 담을 정보가 없다면 헤더만 직렬화해서 발신합니다.
	//------------------------------------------------------//
	bool SerializeHader(JIGAPPacket::PacketType eeInType);
	
	//------------------------------------------------------//
	// 패킷헤더와 패킷을 iSendStream에 직렬화 합니다.
	//------------------------------------------------------//
	void ClearSendStream();

	//------------------------------------------------------//
	// 현재 패킷의 총 사이즈를 Stream 앞 4바이트에 직렬화 합니다.
	//------------------------------------------------------//
	void SerializeTotalPacketSize(unsigned int iInSize);	

public:
	const char* GetSendStream() const { return lpSendStream; }	
	int GetSizeOfRecvPacket() const { return iSizeOfRecvPacket; }
	int GetSendStreamPosition() const { return iSendStreamPosition; }
};

