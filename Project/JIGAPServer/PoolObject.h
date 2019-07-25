#pragma once
class PoolObject
{
public:
	bool bIsActive = false;
public:
	PoolObject();
	virtual ~PoolObject();

	virtual void OnActiveObject();
	virtual void OnUnActiveObject();
};

