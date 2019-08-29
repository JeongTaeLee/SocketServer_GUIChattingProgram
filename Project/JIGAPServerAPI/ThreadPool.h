#pragma once

namespace JIGAPServerAPI
{
	class ThreadPool
	{
	private:
		std::vector<std::thread> vThreads;
		std::queue < std::function<void()> > qFunction;

		std::mutex threadsMutex;

		bool bDestroy = false;
		bool bPause = false;

	public:
		ThreadPool(int inPoolSize);
		~ThreadPool();


		template<class ...Args>
		void AddFunc(std::function<void(Args...)> func, Args ... args)
		{
			qFunction.emplace([func, args]() {func(args...); });
		}

		void AddFunc(std::function<void()> inFunc);

		void Stop() { bPause = true; }
		void Resum() { bPause = false; }
	};
}