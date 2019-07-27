#pragma once
#include "singleton.h"

class BaseDB;

class DBManager :
	public singleton<DBManager>
{
private:
	BaseDB* lpDB;

public:
	DBManager() {}
	virtual ~DBManager() {}

	bool ConnenctToDB();
	void DisconnectDB();


};

#define DBMANAGER DBManager::GetInst()