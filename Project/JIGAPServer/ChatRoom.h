#pragma once

class ChatUserData;

class ChatRoom
{
private:
	std::string strRoomName;

	std::set<ChatUserData*> sChatUserData;

	bool bBaseRoom = false;
public:
	ChatRoom(const std::string& inStrRoomName);
	virtual ~ChatRoom();

	bool AddUser(ChatUserData* inLpChatUserData);
	void DeleteUser(ChatUserData* inLpChatUserData);

	void SendMessageInUsers(PacketHandler * inHandler);
	void SetBaseRoom() { bBaseRoom = true; };
	
	int GetRoomUserCount() { return sChatUserData.size(); }
	const std::string& GetRoomName() { return strRoomName; }
};

