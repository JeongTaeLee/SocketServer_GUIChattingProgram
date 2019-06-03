#pragma once

struct PacketHeader
{
	google::protobuf::int32 size;
	JIGAPPacket::PacketType packetType;
};

class PacketHandler
{
private:
public:
	PacketHandler();
	~PacketHandler();
};

