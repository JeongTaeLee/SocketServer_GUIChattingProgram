#pragma once

class ChatRoom;

class ChatRoomAdmin
{
private:

	std::hash_map<std::string, ChatRoom*> rooms;

	ChatRoom* lpLobby = nullptr;
	
	std::mutex roomAdminMutex;
public:
	void Release();

	void SerialieRoomList(PacketHandler* inLpHandler);

	ChatRoom* CreateLobby(const std::string & inStrRoomName);
	ChatRoom* CreateRoom(const std::string& inStrRoomName);
	ChatRoom* FindRoom(const std::string& inStrRoomName);
	void DeleteRoom(const std::string& inStrRoomName);

	ChatRoom* GetLobby() { return lpLobby; }
	const std::hash_map<std::string, ChatRoom*> GetRooms() { return rooms; }
};

