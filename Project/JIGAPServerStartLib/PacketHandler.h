#pragma once

struct PacketHeader
{

};

class PacketHandler
{
private:
	JIGAPPacket::Packet* lpPacket;


public:
	PacketHandler();
	~PacketHandler();

	void SetPacketParameter(JIGAPPacket::Packet_PacketType eInPacketType, const std::string& strInSender, const std::string& message);
};

