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

	/*반환 받은 객체를 초기화 해줘야합니다.*/
	inline T * AddUser(TCPSocket * lpInTCPSocket)
	{
		if (auto Iter = hmUsers.find(lpInTCPSocket->GetSocket()); Iter != hmUsers.end())
			throw CustomException(__LINE__, __FILEW__, "The Client tried to access abnormally");

		T* addObject = dataPool.GetItem();	
		addObject->SetTCPSock(lpInTCPSocket);

		if (!addObject)
			throw CustomException(__LINE__, __FILEW__, "Over UserData Count in server");

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
