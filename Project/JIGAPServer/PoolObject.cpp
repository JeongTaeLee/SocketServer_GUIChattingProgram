#include "pch.h"
#include "PoolObject.h"

PoolObject::PoolObject()
{
}

PoolObject::~PoolObject()
{
}

void PoolObject::OnActiveObject()
{
}

void PoolObject::OnUnActiveObject()
{
}

void PoolObject::SetActive(bool bInIsActive)
{
	if (bIsActive == bInIsActive)
		return;

	bIsActive = bInIsActive;

	if (bIsActive)
		OnActiveObject();
	else
		OnUnActiveObject();
}
