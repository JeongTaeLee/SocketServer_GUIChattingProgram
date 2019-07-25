#pragma once

class BaseUserData;

template<class T>
class UserDataAdmin
{
protected:
	ObjectPool<T> dataObjectPool;

public:
	inline void InitializeAdmin(int inMaxUserCount)
	{
		dataObjectPool.InitializePool(inMaxUserCount);
	}
	inline void ReleaseAdmin()
	{
		dataObjectPool.ReleasePool();
	}

	inline T * AddUser(T * user)
	{
	}
};
