#pragma once
class PoolObject
{
private:
	bool bIsActive = false;
public:
	PoolObject();
	virtual ~PoolObject();

	virtual void OnActiveObject();
	virtual void OnUnActiveObject();
};

