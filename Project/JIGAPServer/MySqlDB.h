#pragma once
#include "BaseDB.h"
class MySqlDB :
	public BaseDB
{
private:
	MYSQL* mySql = nullptr;
	MYSQL connection;
	MYSQL_RES * mySqlRes = nullptr;
	MYSQL_ROW mySqlRow;

	std::string strHost;
	std::string strUser;
	std::string strPassword;
	std::string strDB;
	std::string strTable;

	int iPort = 0;

public:
	// BaseDB을(를) 통해 상속됨
	virtual bool ConnectToDB() override;
	virtual void DisconnectToDB() override;

	
	bool WriteQuery(const std::string& key);

	MySqlDB& SetHost(const std::string& _host) { strHost = _host;  return *this; }
	MySqlDB& SetUser(const std::string& _user) { strUser = _user;  return *this; }
	MySqlDB& SetPassword(const std::string& _password) { strPassword = _password;  return *this; }
	MySqlDB& SetDB(const std::string& _strDB) { strDB = _strDB ;  return *this; }
	MySqlDB& SetSqlPort(int _port) { iPort = _port; return *this; }
	
};

