#pragma once
#include "PoolObject.h"

template <typename T>
class ObjectPool
{
private:
	std::vector<PoolObject*> pool;

	int poolSize = 0;
public:
	void InitializePool(int size)
	{
		poolSize = size;

		pool.reserve(size);

		for (int i = 0; i < size; i++)
		{
			pool.push_back(new T());
		}
	}

	void ReleasePool()
	{
		for (int i = 0; i < poolSize; i++)
		{
			SAFE_DELETE(pool[i]);
		}

		pool.clear();
	}

	T* GetItem()
	{
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