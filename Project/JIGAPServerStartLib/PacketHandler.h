#pragma once

#define BASEALLOCSIZE 2048


struct PacketHeader
{
	google::protobuf::int32 size;
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


private:
	bool Write(const PacketHeader & header, google::protobuf::Message * lpInPacket, unsigned int iInSizes);

public:
	// 요청 외에 다른 정보를 쓸필요 없는 데이터를 쓸때 사용합니다.
	void WriteBaseRequest(JIGAPPacket::PacketType eInPacketType);
	// 답변 외에 외에 다른 정보를 쓸필요 없는 데이터를 쓸때 사용합니다.
	void WriteBaseAnswer(JIGAPPacket::PacketType eInPacketType);

	// 로그인
	void WriteLoginRequest(const std::string & strInNickName);
	void WriteLoginAnswer(bool bInSuccess);

	// 방 참가.
	void WriteJoinedRoomRequest(const std::string& strInRoomName);
	void WriteJoinedRoomAnswer(bool bInSuccess, const std::string& strInRoomName);

	//RoomList
	void WriteRoomListAnswer(int iInRoomCount);
	void WriteRoomElement(const std::string& strInRoomName);

	void WriteTotalPacketSize(unsigned int size);
	void ClearWirteBuffer();
};

