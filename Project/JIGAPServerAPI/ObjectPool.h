#pragma once
#include "PoolObject.h"

namespace JIGAPServeAPI
{
	template<class T>
	class ObjectPool
	{
	private:
		std::vector<T*> vObjectPool;

		ObjectPool() { }
	public:
		ObjectPool(int inPoolSize, std::function<T* ()> creator)
		{
			vObjectPool.reserve(inPoolSize);

			for (int i = 0; i < inPoolSize; ++i)
				vObjectPool.push_back(creator());
		}

		ObjectPool(int inPoolSize)
		{
			vObjectPool.reserve(inPoolSize);

			for (int i = 0; i < inPoolSize; ++i)
				vObjectPool.push_back(new T());
		}

		virtual ~ObjectPool() {}

		T* GetItem()
		{
			for (auto Iter : vObjectPool)
			{
				if (!Iter->GetActive())
				{
					Iter->SetActive(true);
					return Iter;
				}
			}

#ifdef _DEBUG
			throw std::out_of_range("You are outside the maximum range for the object pool.");
#else
			return nullptr;
#endif

		}
	};
}