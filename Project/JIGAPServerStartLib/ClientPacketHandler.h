#pragma once
#include "PacketHandler.h"
class ClientPacketHandler :
	public PacketHandler
{
protected:
	google::prt
public:
	ClientPacketHandler();
	virtual ~ClientPacketHandler();
};

