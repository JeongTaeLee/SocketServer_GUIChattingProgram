#pragma once

class BaseUserData;

template<class T>
class UserDataAdmin
{
protected:
	ObjectPool<T> dataObjectPool;
	std::mutex userMutex = std::mutex();

	std::hash_map < SOCKET, T* > mUsers;
public:
	inline void InitializeAdmin(int inMaxUserCount)
	{
		dataObjectPool.InitializePool(inMaxUserCount);
	}
	inline void ReleaseAdmin()
	{
		dataObjectPool.ReleasePool();
	}

	/*반환 받은 객체를 초기화 해줘야합니다.*/
	inline T * AddUser(TCPSocket * lpInTCPSocket)
	{
		userMutex.lock();

		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
		{
			userMutex.unlock();
			return nullptr;
		}

		T* addObject = dataObjectPool.GetItem();
		addObject->SetTCPSock(lpInTCPSocket);

		if (!addObject)
			throw std::exception("서버의 최대 User수를 초과했습니다");

		T* t = mUsers.insert(std::hash_map<SOCKET, T*>::value_type(lpInTCPSocket->GetSocket(), addObject)).first->second;

		userMutex.unlock();
		return t;
	}
	inline void DeleteUser(TCPSocket* lpInTCPSocket)
	{
		userMutex.lock();

		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
		{
			dataObjectPool.ReturnItem((*Iter).second);
			mUsers.erase(Iter);
		}

		userMutex.unlock();
	}

	inline T* FindUser(TCPSocket* lpInTCPSocket)
	{
		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
		{
			return (*Iter).second;
		}

		
		
		return nullptr;
	}

	inline T* FindUserInId(const std::string & stdId)
	{
		for (auto Iter : mUsers)
		{
			if (Iter.second->GetUserID() == stdId)
				return Iter.second;
		}

		return nullptr;
	}
};
