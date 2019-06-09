#pragma once

class TCPSocket;

class Room
{
protected:
	std::list<TCPSocket*> liUserList;
	std::string strRoomName;
public:

	Room();
	virtual ~Room();


	void AddUser(TCPSocket* lpSocket);
	void DeleteUser(TCPSocket* lpSocket);
	void SendToAllUser(const char* lpBuffer, size_t tSize);


	int GetUserCount() { return liUserList.size(); }

	void SetRoomName(const std::string& name) { strRoomName = name; }
	const std::string& GetRoomName() { return strRoomName; }
	const std::list<TCPSocket*> GetUserList() { return liUserList; }
};

using LPROOM = Room *;