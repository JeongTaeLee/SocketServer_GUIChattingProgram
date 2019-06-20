#pragma once

class UserTCPSocket;

class Room
{
private:
	std::string strRoomName;
	std::list<UserTCPSocket*> liUserList;

public:
	void RegisterUser(UserTCPSocket * lpInUserSocket);
	void UnRegisterUser(UserTCPSocket * lpInUserSocket);

public:
	void SendAlluser(const char* lpInBuffer, size_t iInSize);

	
public:
	void SetRoomName(const std::string&strInRoomName) { strRoomName = strInRoomName; }

	const std::string& GetRoomName() { return strRoomName; }
	size_t GetUserCount() { return liUserList.size(); }
};

