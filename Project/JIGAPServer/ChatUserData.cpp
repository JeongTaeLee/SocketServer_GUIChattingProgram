#include "pch.h"

#include "ChatUserData.h"

ChatUserData::ChatUserData()
{
}

ChatUserData::~ChatUserData()
{
}

void ChatUserData::OnActiveObject()
{
	BaseUserData::OnActiveObject();

	bLogin = false;
	lpCurRoom = nullptr;
}

void ChatUserData::OnUnActiveObject()
{
	BaseUserData::OnUnActiveObject();

	bLogin = false;
	lpCurRoom = nullptr;
}
