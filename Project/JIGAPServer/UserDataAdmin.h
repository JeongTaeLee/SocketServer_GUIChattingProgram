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
			throw "UserDataAdmin - T 형식이 BaseUserData의 자식이 아닙니다.";
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
