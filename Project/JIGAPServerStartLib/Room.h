#pragma once

class TCPSocket;
class Lobby;

class Room
{
protected:
	std::list<LPTCPSOCK> liUserList;
public:
	Room();
	virtual ~Room();

	void SendRoomMessage(const char * inMessage);

	void AddUser(LPTCPSOCK lpSocket);
	void DeleteUser(LPTCPSOCK lpSocket);

};

using LPROOM = Room *;