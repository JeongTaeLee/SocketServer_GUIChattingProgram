#pragma once

#define BASEALLOCSIZE 10000+


struct PacketHeader
{
	google::protobuf::int32 size;
	JIGAPPacket::PacketType packetType;
};

class PacketHandler
{
private:
	google::protobuf::int8* lpBuffer;
public:
	PacketHandler();
	~PacketHandler();

	template<class Packet>
	void AddPacket(JIGAPPacket::PacketType type, const Packet & packet);

};

template<class Packet>
inline void PacketHandler::AddPacket(JIGAPPacket::PacketType type, const Packet& packet)
{
	switch (type)
	{
	case JIGAPPacket::LoginRequestType:
		break;
	case JIGAPPacket::LoginAnswerType:
		break;
	case JIGAPPacket::JoinedRoomRequestType:
		break;
	case JIGAPPacket::JoinedRoomAnswerType:
		break;
	case JIGAPPacket::CreateRoomType:
		break;
	case JIGAPPacket::RoomListRequestType:
		break;
	case JIGAPPacket::RoomListAnswerType:
		break;
	case JIGAPPacket::RoomElementType:
		break;
	case JIGAPPacket::ExitRoomRequestType:
		break;
	case JIGAPPacket::ExitRoomAnswerType:
		break;
	case JIGAPPacket::ChattingType:
		break;
	}

	PacketHeader header;
	header.packetType = type;
	header.size = packet.ByteSize();
}
