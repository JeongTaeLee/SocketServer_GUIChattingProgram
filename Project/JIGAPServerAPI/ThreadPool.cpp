#include "pch.h"
#include "ThreadPool.h"

JIGAPServerAPI::ThreadPool::ThreadPool(int inPoolSize)
{
	vThreads.reserve(inPoolSize);

	for (int i = 0; i < inPoolSize; ++i)
	{
		vThreads.emplace_back([this]() {
			while (true)
			{

				threadsMutex.lock();

				if (qFunction.empty() && bDestroy)
				{
					threadsMutex.unlock(); 
					return;
				}
				if (qFunction.empty() || bPause)
				{
					threadsMutex.unlock(); 
					continue;
				}

				auto behaviors = std::move(qFunction.front());
				qFunction.pop();

				threadsMutex.unlock();

				behaviors();
			}
			});
	}
}

JIGAPServerAPI::ThreadPool::~ThreadPool()
{
	bDestroy = true;

	for (int i = 0; i < vThreads.size(); ++i)
	{
		if (vThreads[i].joinable())
			vThreads[i].join();
	}
	vThreads.clear();
}

void JIGAPServerAPI::ThreadPool::AddFunc(std::function<void()> inFunc)
{
	qFunction.emplace(inFunc);
}
