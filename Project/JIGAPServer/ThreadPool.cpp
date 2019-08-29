#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool(int inPoolSize)
{

	vThreads.reserve(inPoolSize);
	for (int i = 0; i < inPoolSize; ++i)
	{
		vThreads.emplace_back([this]() {
			while (true)
			{
				std::unique_lock lock(threadsMutex);

				if (qFunction.empty() && bDestroy) return;
				if (qFunction.empty() || bPause) continue;
				
				auto behaviors = std::move(qFunction.front());
				qFunction.pop();

				lock.unlock();

				behaviors();
			}
		});
	}
}

ThreadPool::~ThreadPool()
{
	bDestroy = true;
	
	for (int i = 0; i < vThreads.size(); ++i)
	{
		if (vThreads[i].joinable())
			vThreads[i].join();
	}
	vThreads.clear();
}

void ThreadPool::AddFunc(std::function<void()> inFunc)
{
	qFunction.emplace(inFunc);
}
