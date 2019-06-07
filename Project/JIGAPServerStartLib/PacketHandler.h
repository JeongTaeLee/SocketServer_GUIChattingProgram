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

	// 현재 버퍼에 할당된 총 길이입니다.
	unsigned int iIStreamAllocSize;
	unsigned int iOStreamAllocSize;

	// 현재 버터에 쓰여진 길이입니다.
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
	// 로그인 패킷.
	void WriteLoginRequest(const std::string & strInNickName);
	void WriteLoginAnswer(bool bInSuccess);

	//방 생성 패킷.
	void WriteCreateRoomPacket(const std::string& strInRoomName);

	// 방 참가 패킷.
	void WriteJoinedRoomRequest(const std::string& strInRoomName);
	void WriteJoinedRoomAnswer(const std::string& strInRoomName, bool bInSuccess);

	// 방 이탈 패킷.
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

