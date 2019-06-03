#include "pch.h"
#include "PacketHandler.h"

PacketHandler::PacketHandler()
	:
	bLoginPacket(false), 
	bRoomListPacket(false),
	bCreateRoomPacket(false),
	bJoinedRoomPacket(false),
	bCattingPacket(false)
{
}

PacketHandler::~PacketHandler()
{
}
