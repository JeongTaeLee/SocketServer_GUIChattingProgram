#pragma once
#include "BaseUserData.h"

class ChatRoom;

class ChatUserData :
	public BaseUserData
{
private:
	ChatRoom* lpCurRoom = nullptr;

	std::string strName;
public:
	ChatUserData();
	virtual ~ChatUserData();

	virtual void OnUnActiveObject() override;
	
	ChatRoom* GetCurrentRoom() { return lpCurRoom; }
	void SetCurrentRoom(ChatRoom* inLpCurRoom) { lpCurRoom = inLpCurRoom; };

	const std::string& GetName() { return strName; }
	void SetName(const std::string& strInName) { strName = strInName; }
};

