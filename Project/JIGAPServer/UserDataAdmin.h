#pragma once

class BaseUserData;

template<class T>

class UserDataAdmin
{
protected:
	ObjectPool<T> dataPool;

	std::hash_map < SOCKET, T* > mUsers;

	mutable std::mutex userDataMutex;
public:
	inline void InitializeAdmin(int inMaxUserCount)
	{
		dataPool.InitializePool(inMaxUserCount);
	}
	inline void ReleaseAdmin()
	{
		dataPool.ReleasePool();
	}

	/*반환 받은 객체를 초기화 해줘야합니다.*/
	inline T * AddUser(TCPSocket * lpInTCPSocket)
	{
		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
			return nullptr;

		T* addObject = dataPool.GetItem();
		
		addObject->SetTCPSock(lpInTCPSocket);

		if (!addObject)
			throw std::exception("서버의 최대 User수를 초과했습니다");

		userDataMutex.lock();
		T* t = mUsers.insert(std::hash_map<SOCKET, T*>::value_type(lpInTCPSocket->GetSocket(), addObject)).first->second;
		userDataMutex.unlock();
		return t;
	}
	inline void DeleteUser(TCPSocket* lpInTCPSocket)
	{
		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
		{
			Iter->second->SetActive(false);

			userDataMutex.lock();
			mUsers.erase(Iter);
			userDataMutex.unlock();
		}
	}

	inline T* FindUser(TCPSocket* lpInTCPSocket)
	{
		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
			return (*Iter).second;

		return nullptr;
	}

	inline T* FindUserInId(const std::string & stdId)
	{
		std::lock_guard guard(userDataMutex);
		for (auto Iter : mUsers)
		{
			if (Iter.second->GetUserID() == stdId)
				return Iter.second;
		}

		return nullptr;
	}
};
