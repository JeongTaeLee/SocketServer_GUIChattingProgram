#pragma once

class JIGAPServer;

class ServerLogic
{
	JIGAPServer* lpServer;


		//LoginRequest = 0,
		//
		//JoinRoomRequest = 2,
		//
		//CreateRoomRequest = 4,
		//
		//RoomListRequest = 6,
		//
		//
		//ExitRoomRequest = 9,
		//
		//ChattingRequest = 11,
		//
		//PlayerEnterRoom = 13,
		//PlayerExitRoom = 14,
public:
	void PacketProcess(JIGAPPacket::LoginRequest& packet);
	
};

