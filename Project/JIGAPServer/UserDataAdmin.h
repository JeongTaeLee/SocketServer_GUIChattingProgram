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

	/*��ȯ ���� ��ü�� �ʱ�ȭ ������մϴ�.*/
	inline T * AddUser(TCPSocket * lpInTCPSocket)
	{
		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
			return nullptr;

		T* addObject = dataPool.GetItem();
		
		addObject->SetTCPSock(lpInTCPSocket);

		if (!addObject)
			throw std::exception("������ �ִ� User���� �ʰ��߽��ϴ�");

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
