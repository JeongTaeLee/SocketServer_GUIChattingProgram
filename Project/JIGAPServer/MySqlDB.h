#pragma once
#include "BaseDB.h"
class MySqlDB :
	public BaseDB
{
private:
	MYSQL* mySql = nullptr;
	MYSQL connection;


public:
	// BaseDB을(를) 통해 상속됨
	virtual bool ConnectToDB() override;
	virtual void DisconnectToDB() override;

	virtual QUERYRESULT WriteQuery(const std::string& key);
	virtual QUERYRESULT ReadRow(const std::string& key, TYPE_ROW & inResult) override;
	virtual QUERYRESULT ReadTables(const std::string& key, TYPE_ROWS& inResult) override;

};

