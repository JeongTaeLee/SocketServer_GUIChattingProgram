#pragma once

class ChatUserData;

class ChatRoom
{
private:
	std::string strRoomName;

	std::set<ChatUserData*> sChatUserData;
	
	std::mutex roomUserMutex;

	bool bBaseRoom = false;
public:
	ChatRoom(const std::string& inStrRoomName);
	virtual ~ChatRoom();

	bool AddUserInRoom(ChatUserData* inLpChatUserData);
	void DeleteUser(ChatUserData* inLpChatUserData);


	void SendMessageInUsers(PacketHandler * inHandler);
	void SetBaseRoom() { bBaseRoom = true; };
	
	int GetRoomUserCount() { return sChatUserData.size(); }
	bool GetBaseRoom() { return bBaseRoom; }
	const std::string& GetRoomName() { return strRoomName; }
};

