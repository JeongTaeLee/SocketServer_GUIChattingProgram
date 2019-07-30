#include "pch.h"
#include "ChatRoomAdmin.h"
#include "ChatRoom.h"

ChatRoom* ChatRoomAdmin::CreateLobby(const std::string& inStrRoomName)
{
	if (lpLobby) return lpLobby;

	roomsMutex.lock();
	lpLobby = rooms.insert(std::hash_map<std::string, ChatRoom*>::value_type(inStrRoomName, new ChatRoom(inStrRoomName))).first->second;
	roomsMutex.unlock();
	return lpLobby;
}

ChatRoom* ChatRoomAdmin::CreateRoom(const std::string& inStrRoomName)
{
	roomsMutex.lock();

	if (auto find = rooms.find(inStrRoomName); find != rooms.end())
	{
		roomsMutex.unlock();
		return find->second;
	}

	ChatRoom* createRoom = rooms.insert(std::hash_map<std::string, ChatRoom*>::value_type(inStrRoomName, new ChatRoom(inStrRoomName))).first->second;
	
	roomsMutex.unlock();
	return createRoom;
}

ChatRoom* ChatRoomAdmin::FindRoom(const std::string& inStrRoomName)
{
	roomsMutex.lock();

	if (auto find = rooms.find(inStrRoomName); find != rooms.end())
	{
		roomsMutex.unlock();
		return find->second;
	}

	roomsMutex.unlock();
	return nullptr;
}

void ChatRoomAdmin::DeleteRoom(const std::string& inStrRoomName)
{
	roomsMutex.lock();
	
	if (auto find = rooms.find(inStrRoomName); find != rooms.end())
		rooms.erase(find);

	roomsMutex.unlock();
}
