#pragma once

class BaseDB;

class BaseQuery
{
protected:
	BaseDB* lpDB = nullptr;

public:
	virtual bool InitializeQuery() PURE;
	virtual void ReleaseQuery() PURE;
};

