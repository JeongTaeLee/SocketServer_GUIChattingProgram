#pragma once

class ChatUserData;

class ChatRoom
{
private:
	std::string strRoomName;

	std::set<ChatUserData*> sChatUserData;

public:
	ChatRoom(const std::string& inStrRoomName);
	virtual ~ChatRoom();

	bool AddUser(ChatUserData* inLpChatUserData);
	void DeleteUser(ChatUserData* inLpChatUserData);

	void SendMessageInUsers(PacketHandler * inHandler);


	const std::string& GetRoomName() { return strRoomName; }
};

