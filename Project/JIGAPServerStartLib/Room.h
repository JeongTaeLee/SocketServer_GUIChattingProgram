#pragma once

class TCPSocket;
class Lobby;

class Room
{
protected:
	std::list<TCPSocket*> liUserList;
	std::string strRoomName;
public:

	Room();
	virtual ~Room();

	void SendRoomMessage(const char * inMessage);

	void AddUser(TCPSocket* lpSocket);
	void DeleteUser(TCPSocket* lpSocket);

	int GetUserCount() { return liUserList.size(); }

	void SetRoomName(const std::string& name) { strRoomName = name; }
	const std::string& GetRoomName() { return strRoomName; }
};

using LPROOM = Room *;