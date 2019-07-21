#pragma once
#include "pch.h"

template <class T >
class ObjectPool
{
private:
	std::queue<T*> qUnActivePool;
	std::list<T*> liActivePool;

	int iPoolSize = 0;
public:
	ObjectPool() {}
	~ObjectPool() {}

	void InitializePool(int inIPoolSize)
	{
		iPoolSize = inIPoolSize;

		for (int i = 0; i < iPoolSize; ++i)
			qUnActivePool.push(new T);
	}
	void ReleasePool()
	{
		while (true)
		{
			if (liActivePool.empty())
				break;

			qUnActivePool.push((*liActivePool.begin()));
			liActivePool.erase(liActivePool.begin());
		}

		while (true)
		{
			if (qUnActivePool.empty())
				break;

			SAFE_DELETE(qUnActivePool.front());
			qUnActivePool.pop();
		}
	}

	T* GetPoolObject()
	{
		if (qUnActivePool.empty())
		{
			for (int i = 0; i < 10; ++i)
				qUnActivePool.push(new T);
			
			iPoolSize += 10;
		}

		T* getObject = qUnActivePool.front();
		liActivePool.push_back(getObject);
		qUnActivePool.pop();
		
		return getObject;
	}
	T* ReturnPoolObject(T* inPoolObject)
	{
		for (auto Iter = liActivePool.begin(); Iter != liActivePool.end(); ++Iter)
		{
			if ((*Iter) == inPoolObject)
			{
				T* findObject = (*Iter);
				liActivePool.erase(Iter);
				qUnActivePool.push(findObject);
				break;
			}
		}
	}
};

