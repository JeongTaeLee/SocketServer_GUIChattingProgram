#pragma once
#include "PoolObject.h"

template <typename T>
class ObjectPool
{
private:
	std::vector<PoolObject*> vPool;

	std::mutex poolMutex;

	int iPoolSize = 0;
public:
	void InitializePool(int size)
	{
		iPoolSize = size;
		vPool.reserve(size);

		std::lock_guard gd(poolMutex);

		for (int i = 0; i < size; i++)
		{
			T * t = CreateItem();
			t->SetActive(false);
		}
	}

	void ReleasePool()
	{
		std::lock_guard gd(poolMutex);

		for (int i = 0; i < iPoolSize; i++)
			SAFE_DELETE(vPool[i]);

		vPool.clear();
	}

	T* GetItem()
	{
		std::lock_guard gd(poolMutex);
	
		T* returnItem = nullptr;

		for (auto Iter : vPool)
		{
			if (!Iter->GetActive())
			{
				returnItem = static_cast<T*>(Iter);
				returnItem->SetActive(true);
				break;
			}
		}

		if (returnItem == nullptr)
		{
			returnItem = CreateItem();
			returnItem->SetActive(true);
		}

		return returnItem;
	}

private:
	T* CreateItem()
	{
		T* create = new T();

		vPool.push_back(create);
		
		create->SetActive(true);
		
		return create;
	}

	
};