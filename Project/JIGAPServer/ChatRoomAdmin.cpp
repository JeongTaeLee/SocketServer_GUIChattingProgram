#include "pch.h"
#include "ChatRoomAdmin.h"
#include "ChatRoom.h"

void ChatRoomAdmin::Release()
{
	roomsMutex.lock();
	for (auto Iter : rooms)
	{
		SAFE_DELETE(Iter.second);
	}
	rooms.clear();
	roomsMutex.unlock();
}

void ChatRoomAdmin::SerialieRoomList(PacketHandler* inLpHandler)
{
	roomsMutex.lock();

	for (auto Iter : rooms)
	{
		JIGAPPacket::RoomListElement element;
		JIGAPPacket::RoomInfo* info = element.mutable_roominfo();

		info->set_roomname(Iter.second->GetRoomName());
		inLpHandler->SerializePacket(JIGAPPacket::Type::eRoomListElement, element);;

		info = element.release_roominfo();
		SAFE_DELETE(info);
	}

	roomsMutex.unlock();
}

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
