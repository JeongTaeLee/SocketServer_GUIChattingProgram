#pragma once

template<class T>
class singleton
{
protected:
	singleton() {}
	virtual ~singleton() {}

public:
	static T& GetInst()
	{
		static T inst;
		return inst;
	}

};
