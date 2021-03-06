#include "pch.h"
#include "ChatRoom.h"
#include "ChatUserData.h"

ChatRoom::ChatRoom()
{
}

ChatRoom::~ChatRoom()
{
}

void ChatRoom::OnActiveObject()
{
	strRoomName = "None";
	bBaseRoom = false;
}

void ChatRoom::OnUnActiveObject()
{
	strRoomName = "None";
	bBaseRoom = false;
}

bool ChatRoom::AddUserInRoom(ChatUserData* inLpChatUserData)
{
	if (auto find = sChatUserData.find(inLpChatUserData); find != sChatUserData.end())
		return false;

	inLpChatUserData->SetCurrentRoom(this);

	roomUserMutex.lock();
	sChatUserData.insert(inLpChatUserData);
	roomUserMutex.unlock();

	return true;
}

void ChatRoom::DeleteUser(ChatUserData* inLpChatUserData)
{
	std::lock_guard gd(roomUserMutex);

	if (auto find = sChatUserData.find(inLpChatUserData); find != sChatUserData.end())
	{
		inLpChatUserData->SetCurrentRoom(nullptr);
		sChatUserData.erase(find);
	}
}


void ChatRoom::SendMessageInUsers(PacketHandler* inHandler)
{
	std::lock_guard gd(roomUserMutex);

	for (auto Iter : sChatUserData)
		Iter->GetTCPSocket()->IOCPSend(inHandler->GetSerializeBufferData(), inHandler->GetSerializeRealSize());
}
