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

	void SetActive(bool bInIsActive);
	bool GetActive() { return bIsActive; }
};

