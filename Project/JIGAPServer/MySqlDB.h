#pragma once
#include "BaseDB.h"
class MySqlDB :
	public BaseDB
{
private:
	MYSQL* mySql = nullptr;
	MYSQL connection;


public:
	// BaseDB��(��) ���� ��ӵ�
	virtual bool ConnectToDB() override;
	virtual void DisconnectToDB() override;

	virtual bool WriteQuery(const std::string& key);
	virtual bool ReadRow(const std::string& key, TYPE_ROW & inResult) override;
	virtual bool ReadTables(const std::string& key, TYPE_ROWS& inResult) override;

};

