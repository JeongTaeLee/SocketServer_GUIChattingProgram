#include "pch.h"

#include "ChatUserData.h"

ChatUserData::ChatUserData()
{
}

ChatUserData::~ChatUserData()
{
}

void ChatUserData::OnUnActiveObject()
{
	BaseUserData::OnUnActiveObject();
	bLogin		= false;
	lpCurRoom	= nullptr;
}
