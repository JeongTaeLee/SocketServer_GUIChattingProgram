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

bool ChatRoom::AddUser(ChatUserData* inLpChatUserData)
{
	if (auto find = sChatUserData.find(inLpChatUserData); find != sChatUserData.end())
		return false;

	inLpChatUserData->SetCurrentRoom(this);
	sChatUserData.insert(inLpChatUserData);

	return true;
}

void ChatRoom::DeleteUser(ChatUserData* inLpChatUserData)
{
	if (auto find = sChatUserData.find(inLpChatUserData); find != sChatUserData.end())
	{
		inLpChatUserData->SetCurrentRoom(nullptr);
		sChatUserData.erase(find);
		
	}
}

void ChatRoom::SendMessageInUsers(PacketHandler* inHandler)
{
	for (auto Iter : sChatUserData)
		Iter->GetTCPSocket()->IOCPSend(inHandler->GetSerializeBufferData(), inHandler->GetSerializeRealSize());
}
