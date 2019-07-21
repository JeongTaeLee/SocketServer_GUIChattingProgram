#pragma once

template<class T>
class singleton
{
private:
	static T* lpInst;

protected:
	singleton() {}
	virtual ~singleton() {}

public:
	static T& GetInst()
	{
		if (lpInst == nullptr)
			lpInst = new T;

		return *lpInst;
	}
	static void ReleaseInst()
	{
		if (lpInst)
			delete lpInst;

		lpInst = nullptr;
	}
};

template<class T>
T* singleton<T>::lpInst = nullptr;