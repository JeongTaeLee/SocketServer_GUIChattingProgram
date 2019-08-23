#include "pch.h"
#include "ChatRoom.h"
#include "ChatUserData.h"

ChatRoom::ChatRoom(const std::string& inStrRoomName)
	:strRoomName(inStrRoomName)
{
}

ChatRoom::~ChatRoom()
{
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
	std::lock_guard(roomUserMutex);

	if (auto find = sChatUserData.find(inLpChatUserData); find != sChatUserData.end())
	{
		inLpChatUserData->SetCurrentRoom(nullptr);
		sChatUserData.erase(find);
	}
}


void ChatRoom::SendMessageInUsers(PacketHandler* inHandler)
{
	std::lock_guard(roomUserMutex);

	for (auto Iter : sChatUserData)
		Iter->GetTCPSocket()->IOCPSend(inHandler->GetSerializeBufferData(), inHandler->GetSerializeRealSize());
}
