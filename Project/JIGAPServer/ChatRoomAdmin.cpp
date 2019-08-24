#include "pch.h"
#include "ChatRoomAdmin.h"
#include "ChatRoom.h"
#include "ObjectPool.h"

void ChatRoomAdmin::Initialize(int inIRoomname)
{
	roomPool.InitializePool(inIRoomname);
}

void ChatRoomAdmin::Release()
{
	std::lock_guard gd(roomAdminMutex);

	hmRooms.clear();
	roomPool.ReleasePool();
}

ChatRoom* ChatRoomAdmin::CreateLobby(const std::string& inStrRoomName)
{
	if (lpLobby) return lpLobby;

	std::lock_guard gd(roomAdminMutex);

	ChatRoom* returnRoom = roomPool.GetItem();
	returnRoom->SetRoomName(inStrRoomName);
	returnRoom->SetBaseRoom();
	
	hmRooms.insert(std::hash_map<std::string, ChatRoom*>::value_type(inStrRoomName, returnRoom));

	lpLobby = returnRoom;
	
	return lpLobby;
}

ChatRoom* ChatRoomAdmin::CreateRoom(const std::string& inStrRoomName, bool InBIsBaseRoom)
{
	std::lock_guard gd(roomAdminMutex);

	if (auto find = hmRooms.find(inStrRoomName); find != hmRooms.end())
		return find->second;

	ChatRoom* returnRoom = roomPool.GetItem();
	
	returnRoom->SetRoomName(inStrRoomName);

	if (InBIsBaseRoom) returnRoom->SetBaseRoom();

	hmRooms.insert(std::hash_map<std::string, ChatRoom*>::value_type(inStrRoomName, returnRoom));
	
	return returnRoom;
}

ChatRoom* ChatRoomAdmin::FindRoom(const std::string& inStrRoomName)
{
	if (auto find = hmRooms.find(inStrRoomName); find != hmRooms.end())
		return find->second;

	return nullptr;
}

void ChatRoomAdmin::DeleteRoom(const std::string& inStrRoomName)
{
	std::lock_guard gd(roomAdminMutex);
	
	if (auto find = hmRooms.find(inStrRoomName); find != hmRooms.end())
	{
		find->second->SetActive(false);
		hmRooms.erase(find);
	}
}

void ChatRoomAdmin::SerialieRoomList(PacketHandler* inLpHandler)
{
	std::lock_guard gd(roomAdminMutex);

	JIGAPPacket::RoomListAnswer answer;
	answer.set_roomcount(hmRooms.size());

	inLpHandler->SerializePacket(JIGAPPacket::Type::eRoomListAnswer, answer);

	for (auto Iter : hmRooms)
	{
		JIGAPPacket::RoomListElement element;
		JIGAPPacket::RoomInfo* info = element.mutable_roominfo();

		info->set_roomname(Iter.second->GetRoomName());
		inLpHandler->SerializePacket(JIGAPPacket::Type::eRoomListElement, element);
	}
}
