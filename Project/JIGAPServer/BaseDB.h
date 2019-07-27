#pragma once
class BaseDB
{

public:
	BaseDB()
	{}
	~BaseDB()
	{}

	virtual bool ConnectToDB() PURE;
	virtual void DisconnectToDB() PURE;
};

