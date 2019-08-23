#pragma once
#include "PoolObject.h"

template <typename T>
class ObjectPool
{
private:
	std::vector<PoolObject*> pool;

	std::mutex poolMutex;

	int poolSize = 0;
public:
	void InitializePool(int size)
	{
		poolSize = size;
		pool.reserve(size);

		std::lock_guard gd(poolMutex);

		for (int i = 0; i < size; i++)
			pool.push_back(new T());
	}

	void ReleasePool()
	{
		std::lock_guard gd(poolMutex);

		for (int i = 0; i < poolSize; i++)
			SAFE_DELETE(pool[i]);

		pool.clear();
	}

	T* GetItem()
	{
		std::lock_guard gd(poolMutex);
	
		for (auto Iter : pool)
		{
			if (!Iter->bIsActive)
			{
				Iter->OnActiveObject();
				Iter->bIsActive = true;
				return static_cast<T*>(Iter);
			}
		}

		return nullptr;
	}

	void ReturnItem(T * inItem)
	{
		if (inItem->bIsActive)
		{
			inItem->OnUnActiveObject();
			inItem->bIsActive = false;
		}
	}

	
};