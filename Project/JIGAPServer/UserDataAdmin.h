#pragma once

class BaseUserData;

template<class T>
class UserDataAdmin
{
protected:
	ObjectPool<T> dataObjectPool;

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

	/*��ȯ ���� ��ü�� �ʱ�ȭ ������մϴ�.*/
	inline T * AddUser(TCPSocket * lpInTCPSocket)
	{
		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
			return nullptr;

		T* addObject = dataObjectPool.GetItem();
		addObject->SetTCPSock(lpInTCPSocket);

		if (!addObject)
			throw std::exception("������ �ִ� User���� �ʰ��߽��ϴ�");

		return mUsers.insert(std::hash_map<SOCKET, T*>::value_type(lpInTCPSocket->GetSocket(), addObject)).first->second;
	}
	inline void DeleteUser(TCPSocket* lpInTCPSocket)
	{
		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
		{
			dataObjectPool.ReturnItem((*Iter).second);
			mUsers.erase(Iter);
			return;
		}
	}

	inline T* FindUser(TCPSocket* lpInTCPSocket)
	{
		if (auto Iter = mUsers.find(lpInTCPSocket->GetSocket()); Iter != mUsers.end())
			return (*Iter).second;
		
		return nullptr;
	}
};
