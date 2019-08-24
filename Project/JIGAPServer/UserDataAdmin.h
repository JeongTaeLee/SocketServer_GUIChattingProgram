#pragma once

class BaseUserData;

template<class T>

class UserDataAdmin
{
protected:
	ObjectPool<T> dataPool;

	std::hash_map < SOCKET, T* > hmUsers;

	mutable std::mutex userDataMutex;
public:
	inline void InitializeAdmin(int inMaxUserCount)
	{
		dataPool.InitializePool(inMaxUserCount);
	}
	inline void ReleaseAdmin()
	{
		for (auto Iter : hmUsers)
			Iter.second->CloseTCPSocket();
		hmUsers.clear();
		
		dataPool.ReleasePool();
	}

	/*��ȯ ���� ��ü�� �ʱ�ȭ ������մϴ�.*/
	inline T * AddUser(TCPSocket * lpInTCPSocket)
	{
		if (auto Iter = hmUsers.find(lpInTCPSocket->GetSocket()); Iter != hmUsers.end())
			return nullptr;

		T* addObject = dataPool.GetItem();
		
		addObject->SetTCPSock(lpInTCPSocket);

		if (!addObject)
			throw std::exception("������ �ִ� User���� �ʰ��߽��ϴ�");

		userDataMutex.lock();
		T* t = hmUsers.insert(std::hash_map<SOCKET, T*>::value_type(lpInTCPSocket->GetSocket(), addObject)).first->second;
		userDataMutex.unlock();
		return t;
	}
	inline void DeleteUser(TCPSocket* lpInTCPSocket)
	{
		if (auto Iter = hmUsers.find(lpInTCPSocket->GetSocket()); Iter != hmUsers.end())
		{
			Iter->second->CloseTCPSocket();
			Iter->second->SetActive(false);

			userDataMutex.lock();
			hmUsers.erase(Iter);
			userDataMutex.unlock();
		}
	}

	inline T* FindUser(TCPSocket* lpInTCPSocket)
	{
		if (auto Iter = hmUsers.find(lpInTCPSocket->GetSocket()); Iter != hmUsers.end())
			return (*Iter).second;

		return nullptr;
	}

	inline T* FindUserInId(const std::string & stdId)
	{
		std::lock_guard guard(userDataMutex);
		for (auto Iter : hmUsers)
		{
			if (Iter.second->GetUserID() == stdId)
				return Iter.second;
		}

		return nullptr;
	}
};
