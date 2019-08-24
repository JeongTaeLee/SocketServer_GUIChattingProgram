#pragma once

class ChatRoom;

class ChatRoomAdmin
{
private:
	ChatRoom* lpLobby = nullptr;

	std::hash_map<std::string, ChatRoom*> hmRooms;

	ObjectPool<ChatRoom> roomPool;
	
	mutable std::mutex roomAdminMutex;
public:
	void Initialize(int inIPoolCount);
	void Release();

	ChatRoom* CreateLobby(const std::string & inStrRoomName);
	ChatRoom* CreateRoom(const std::string& inStrRoomName, bool InBIsBaseRoom = false);
	ChatRoom* FindRoom(const std::string& inStrRoomName);
	void DeleteRoom(const std::string& inStrRoomName);

	ChatRoom* GetLobby() { return lpLobby; }
	const std::hash_map<std::string, ChatRoom*> GetRooms() { return hmRooms; }

	void SerialieRoomList(PacketHandler* inLpHandler);
};

