#include "pch.h"
#include "PoolObject.h"

void JIGAPServerAPI::PoolObject::SetActive(bool bInIsActive)
{
	if (bActive == bInIsActive)
		return;
	
	bActive = bInIsActive;

	bActive ? OnActiveObject() : OnUnActiveObject();
}