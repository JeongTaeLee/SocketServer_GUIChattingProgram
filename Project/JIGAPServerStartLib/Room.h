#pragma once

class TCPSocket;
class Lobby;

class Room
{
protected:
	std::list<TCPSocket*> liUserList;
public:
	Room();
	virtual ~Room();

	void SendRoomMessage(const char * inMessage);

	void AddUser(TCPSocket* lpSocket);
	void DeleteUser(TCPSocket* lpSocket);

};

using LPROOM = Room *;