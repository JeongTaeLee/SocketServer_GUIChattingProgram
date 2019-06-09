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

	// ���� ���ۿ� �Ҵ�� �� �����Դϴ�.
	unsigned int iRecvStreamSize;
	unsigned int iSendStreamSize;

	// ���� ��Ʈ������ ���� �������Դϴ�.
	unsigned int iRecvStreamPosition;
	// ���� ��Ʈ������ ���� �������Դϴ�.
	unsigned int iSendStreamPosition;

	// ���� ��Ŷ�� �������Դϴ�.
	unsigned int iSizeOfRecvPacket;
public:
	PacketHandler();
	~PacketHandler();

private:
	void ClearRecvStream();
public:
	//------------------------------------------------------//
	// ���Ͽ��� �޾ƿ� ��Ŷ�� �ڵ鷯�� �����մϴ�.
	//------------------------------------------------------//
	void SetPacketInRecvStream(char* lpInBuffer, int size);

	//------------------------------------------------------//
	// lpInBuffer���� ó�� 4 ����Ʈ�� �о� �޽����� �� 
	// ����� �˾ƿɴϴ�.
	//------------------------------------------------------//
	int ParsingPacketSize(const char* lpInBuffer);

	//------------------------------------------------------//
	// ������ iSendStream���� ��Ŷ �����ŭ ��Ŷ�� �о�ɴϴ�.
	//------------------------------------------------------//
	template <class Packet>
	bool ParsingPacket(Packet& inPacket, unsigned int inSize)
	{
		inPacket.ParseFromArray(&lpRecvStream[iRecvStreamPosition], inSize);
		iRecvStreamPosition += inSize;
		return true;
	}
	
	//------------------------------------------------------//
	// ������ iSendStream���� ���� ��Ŷ�� ����� �о�ɴϴ�.
	//------------------------------------------------------//
	void ParsingPacketHeader(PacketHeader& inHeader);

public:
	//------------------------------------------------------//
	// ��Ŷ����� ��Ŷ�� iSendStream�� ����ȭ �մϴ�.
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
	// �亯�� ��Ŷ�� ���� ������ ���ٸ� ����� ����ȭ�ؼ� �߽��մϴ�.
	//------------------------------------------------------//
	bool SerializeHader(JIGAPPacket::PacketType eeInType);
	
	//------------------------------------------------------//
	// ��Ŷ����� ��Ŷ�� iSendStream�� ����ȭ �մϴ�.
	//------------------------------------------------------//
	void ClearSendStream();

	//------------------------------------------------------//
	// ���� ��Ŷ�� �� ����� Stream �� 4����Ʈ�� ����ȭ �մϴ�.
	//------------------------------------------------------//
	void SerializeTotalPacketSize(unsigned int iInSize);	

public:
	const char* GetSendStream() const { return lpSendStream; }	
	int GetSizeOfRecvPacket() const { return iSizeOfRecvPacket; }
	int GetSendStreamPosition() const { return iSendStreamPosition; }
};

