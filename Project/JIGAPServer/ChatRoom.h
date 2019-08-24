#pragma once
#include "PoolObject.h"

class ChatUserData;

class ChatRoom : public PoolObject
{
private:
	std::string strRoomName;

	std::set<ChatUserData*> sChatUserData;
	
	std::mutex roomUserMutex;

	bool bBaseRoom = false;
public:
	ChatRoom();
	virtual ~ChatRoom();

	virtual void OnActiveObject() override;
	virtual void OnUnActiveObject() override;

	bool AddUserInRoom(ChatUserData* inLpChatUserData);
	void DeleteUser(ChatUserData* inLpChatUserData);

	void SendMessageInUsers(PacketHandler * inHandler);
	void SetBaseRoom() { bBaseRoom = true; };
	void SetRoomName(const std::string& strInRoomName) { strRoomName = strInRoomName; }
	
	int GetRoomUserCount() { return sChatUserData.size(); }
	bool GetBaseRoom() { return bBaseRoom; }
	const std::string& GetRoomName() { return strRoomName; }
};

