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

	// ���� ���ۿ� �Ҵ�� �� �����Դϴ�.
	unsigned int iIStreamAllocSize;
	unsigned int iOStreamAllocSize;

	// ���� ���Ϳ� ������ �����Դϴ�.
	unsigned int iIStreamWriteSize;
	unsigned int iOStreamWriteSize;


public:
	PacketHandler();
	~PacketHandler();


private:
	bool Write(const PacketHeader & header, google::protobuf::Message * lpInPacket, unsigned int iInSizes);

public:
	// ��û �ܿ� �ٸ� ������ ���ʿ� ���� �����͸� ���� ����մϴ�.
	void WriteBaseRequest(JIGAPPacket::PacketType eInPacketType);
	// �亯 �ܿ� �ܿ� �ٸ� ������ ���ʿ� ���� �����͸� ���� ����մϴ�.
	void WriteBaseAnswer(JIGAPPacket::PacketType eInPacketType);

	// �α���
	void WriteLoginRequest(const std::string & strInNickName);
	void WriteLoginAnswer(bool bInSuccess);

	// �� ����.
	void WriteJoinedRoomRequest(const std::string& strInRoomName);
	void WriteJoinedRoomAnswer(bool bInSuccess, const std::string& strInRoomName);

	//RoomList
	void WriteRoomListAnswer(int iInRoomCount);
	void WriteRoomElement(const std::string& strInRoomName);

	void WriteTotalPacketSize(unsigned int size);
	void ClearWirteBuffer();
};

