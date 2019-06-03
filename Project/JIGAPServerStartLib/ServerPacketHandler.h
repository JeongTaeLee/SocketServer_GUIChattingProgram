#pragma once
#include "PacketHandler.h"
class ServerPacketHandler :
	public PacketHandler
{
protected:
	bool bLoginAnswerPacket;
	JIGAPPacket::LoginAnswerPacket loginAnswerPacket;
	
	bool bJoinedRoomAnswerPacket;
	JIGAPPacket::JoinedRoomAnswerPacket joinedRoomAnswerPacket;

	bool bRoomListAnswerPacket;
	JIGAPPacket::RoomListAnswerPacket roomListAnswerPacket;
	std::list<JIGAPPacket::RoomElementPacket> liRoomElementPackets;

	bool bExitRoomAnswer;
public:
	ServerPacketHandler();
	virtual ~ServerPacketHandler();

	void SetLoginAnswerPacket(bool bInLoginSuccess);
	void SetJoinedRoomAnswerPacket(bool bInJoinedRoomSuccess, const std::string & strInRoomName);
	void SetRoomListAnswer(unsigned int iInRoomCount);
	void AddRoomElement(const std::string& strInRoomName);

	void SerializeSend(LPTCPSOCK& lpTcpSocket);

};

