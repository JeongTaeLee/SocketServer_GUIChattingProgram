#pragma once
#include "BaseUserData.h"

class ChatRoom;

class ChatUserData :
	public BaseUserData
{
private:
	ChatRoom* lpCurRoom = nullptr;
public:
	ChatUserData();
	virtual ~ChatUserData();

	virtual void OnUnActiveObject() override;
	
	ChatRoom* GetCurrentRoom(ChatRoom* inLpCurRoom) { return inLpCurRoom; }
	void SetCurrentRoom(ChatRoom* inLpCurRoom) { lpCurRoom = inLpCurRoom; };
};

