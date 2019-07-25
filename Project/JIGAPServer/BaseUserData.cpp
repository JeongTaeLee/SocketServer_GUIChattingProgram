#include "pch.h"
#include "BaseUserData.h"

void BaseUserData::OnActiveObject()
{
}

void BaseUserData::OnUnActiveObject()
{
	strUserId = "None";
	lpTCPSocket = nullptr;

}

