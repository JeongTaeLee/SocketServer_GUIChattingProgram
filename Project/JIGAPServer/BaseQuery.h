#pragma once

class BaseDB;

class BaseQuery
{
protected:
	BaseDB* lpDB = nullptr;

public:
	virtual void InitializeQuery() PURE;
	virtual void ReleaseQuery() PURE;
};

