#pragma once

class BaseUserData;

template<class T>
class UserDataAdmin
{
protected:
	map<SOCKET, T* > mUserDatas;

public:
	UserDataAdmin()
	{
		if constexpr (std::is_base_of<BaseUserData, T>::value == false)
			throw "UserDataAdmin - T ������ BaseUserData�� �ڽ��� �ƴմϴ�.";
	}
	~UserDataAdmin()
	{

	}

	void DisconnectAllUser();

};

template<class T>
inline void UserDataAdmin<T>::DisconnectAllUser()
{
}
