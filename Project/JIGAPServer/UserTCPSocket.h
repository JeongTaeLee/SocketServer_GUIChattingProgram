#pragma once
class Room;

class UserTCPSocket :
	public TCPSocket
{
private:
	std::string strUserName;
		
	Room* lpRoom = nullptr;
public:
	UserTCPSocket();
	UserTCPSocket(SOCKET InSocket);
	UserTCPSocket(SOCKET hInSock, const SocketAddress& InAddr);

public:
	void SetRoom(Room* lpInRoom);
	void SetUserName(const std::string& strInName);

	Room* GetRoom() { return lpRoom; }
	const std::string& GetUserNickName() { return strUserName; }
};

