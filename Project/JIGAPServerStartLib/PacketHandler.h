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
	JIGAPPacket::PacketType packetType;
};

class PacketHandler
{
private:
	char* lpInputStream;
	char* lpOutputStream;

	// ���� ���ۿ� �Ҵ�� �� �����Դϴ�.
	unsigned int iIStreamAllocSize;
	unsigned int iOStreamAllocSize;

	// ���� ���Ϳ� ������ �����Դϴ�.
	unsigned int iIStreamWriteSize;
	unsigned int iOStreamWriteSize;


public:
	PacketHandler();
	~PacketHandler();


public:
	template <class Packet>
	bool WritePacket(const PacketHeader& header, const Packet & inPacket)
	{
		int headerSize = sizeof(header);

		memcpy(&lpOutputStream[iOStreamWriteSize], (char*)& header, headerSize);
		iOStreamWriteSize += headerSize;

		return inPacket->SerializeToArray(&lpOutputStream, iOStreamAllocSize - iOStreamWriteSize);
	}

	template <class Packet>
	bool WritePacket(JIGAPPacket::PacketType eInType, const Packet& inPacket)
	{
		PacketHeader header;
		header.packetType = eInType;
		header.size = inPacket.ByteSize();

		int headerSize = sizeof(header);

		memcpy(&lpOutputStream[iOStreamWriteSize], (char*)& header, headerSize);
		iOStreamWriteSize += headerSize;

		WriteTotalPacketSize(iOStreamWriteSize);

		return inPacket.SerializeToArray(&lpOutputStream, inPacket.ByteSize());
	}

public:
	int ParsingPacketSize(char* lpInBuffer);

public:
	// �α��� ��Ŷ.
	void WriteLoginRequest(const std::string & strInNickName);
	void WriteLoginAnswer(bool bInSuccess);

	//�� ���� ��Ŷ.
	void WriteCreateRoomPacket(const std::string& strInRoomName);

	// �� ���� ��Ŷ.
	void WriteJoinedRoomRequest(const std::string& strInRoomName);
	void WriteJoinedRoomAnswer(const std::string& strInRoomName, bool bInSuccess);

	// �� ��Ż ��Ŷ.
	void WriteExitRoomRequest();
	void WriteExitRoomAnswer();

	//RoomListPacket
	void WriteRoomListRequest();
	void WriteRoomListInformation(unsigned int iInRoomCount);
	void WriteRoomListElement(const std::map < std::string, Room*> & mInRoomList);

	void WriteTotalPacketSize(unsigned int size);
	void ClearWirteBuffer();

public:
};

