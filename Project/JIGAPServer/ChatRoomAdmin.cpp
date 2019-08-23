#include "pch.h"
#include "ChatRoomAdmin.h"
#include "ChatRoom.h"

void ChatRoomAdmin::Release()
{
	std::lock_guard gd(roomAdminMutex);

	for (auto Iter : rooms)
	{
		SAFE_DELETE(Iter.second);
	}
	rooms.clear();
}

void ChatRoomAdmin::SerialieRoomList(PacketHandler* inLpHandler)
{
	std::lock_guard gd(roomAdminMutex);

	for (auto Iter : rooms)
	{
		JIGAPPacket::RoomListElement element;
		JIGAPPacket::RoomInfo* info = element.mutable_roominfo();

		info->set_roomname(Iter.second->GetRoomName());
		inLpHandler->SerializePacket(JIGAPPacket::Type::eRoomListElement, element);;

		info = element.release_roominfo();
		SAFE_DELETE(info);
	}
}

ChatRoom* ChatRoomAdmin::CreateLobby(const std::string& inStrRoomName)
{
	if (lpLobby) return lpLobby;

	std::lock_guard gd(roomAdminMutex);
	lpLobby = rooms.insert(std::hash_map<std::string, ChatRoom*>::value_type(inStrRoomName, new ChatRoom(inStrRoomName))).first->second;
	
	return lpLobby;
}

ChatRoom* ChatRoomAdmin::CreateRoom(const std::string& inStrRoomName)
{
	std::lock_guard gd(roomAdminMutex);

	if (auto find = rooms.find(inStrRoomName); find != rooms.end())
		return find->second;

	ChatRoom* createRoom = rooms.insert(std::hash_map<std::string, ChatRoom*>::value_type(inStrRoomName, new ChatRoom(inStrRoomName))).first->second;
	
	return createRoom;
}

ChatRoom* ChatRoomAdmin::FindRoom(const std::string& inStrRoomName)
{
	std::lock_guard gd(roomAdminMutex);

	if (auto find = rooms.find(inStrRoomName); find != rooms.end())
		return find->second;

	return nullptr;
}

void ChatRoomAdmin::DeleteRoom(const std::string& inStrRoomName)
{
	std::lock_guard gd(roomAdminMutex);
	
	if (auto find = rooms.find(inStrRoomName); find != rooms.end())
		rooms.erase(find);
}
