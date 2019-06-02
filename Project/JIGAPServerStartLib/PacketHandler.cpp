#include "pch.h"
#include "PacketHandler.h"


PacketHandler::PacketHandler()
	:lpPacket(new JIGAPPacket::Packet)
{
}


PacketHandler::~PacketHandler()
{
	delete lpPacket;
	lpPacket = nullptr;
}

void PacketHandler::SetPacketParameter(JIGAPPacket::Packet_PacketType eInPacketType, const std::string& strInSender, const std::string& message)
{
	lpPacket->set_packettype(eInPacketType);
	lpPacket->set_sender(strInSender);
	lpPacket->set_msg(message);
}
