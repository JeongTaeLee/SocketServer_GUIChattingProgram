#pragma once

using TYPE_ROW = std::hash_map<std::string, std::string>;
using TYPE_ROWS = std::vector < std::hash_map < std::string, std::string >>;

class BaseDB
{
protected:

	std::string strHost;
	std::string strUser;
	std::string strPassword;
	std::string strDB;
	std::string strTable;

	int iPort = 0;
public:
	BaseDB()
	{}
	~BaseDB()
	{}

	virtual bool ConnectToDB()		PURE;
	virtual void DisconnectToDB()	PURE;

	virtual bool WriteQuery(const std::string& key) PURE;
	virtual bool ReadRow(const std::string& key, TYPE_ROW& inResult) PURE;
	virtual bool ReadTables(const std::string& key, TYPE_ROWS& inResult) PURE;


	BaseDB& SetHost(const std::string& _host) { strHost = _host;  return *this; }
	BaseDB& SetUser(const std::string& _user) { strUser = _user;  return *this; }
	BaseDB& SetPassword(const std::string& _password) { strPassword = _password;  return *this; }
	BaseDB& SetDB(const std::string& _strDB) { strDB = _strDB;  return *this; }
	BaseDB& SetSqlPort(int _port) { iPort = _port; return *this; }

};

