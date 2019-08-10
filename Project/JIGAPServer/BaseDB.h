#pragma once

using TYPE_ROW = std::hash_map<std::string, std::string>;
using TYPE_ROWS = std::vector < std::hash_map < std::string, std::string >>;

enum QUERYRESULT
{
	FAILE,
	SUCCESS,
	EMPTY,
};

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

	virtual QUERYRESULT WriteQuery(const std::string& key) PURE;
	virtual QUERYRESULT ReadRow(const std::string& key, TYPE_ROW& inResult) PURE;
	virtual QUERYRESULT ReadTables(const std::string& key, TYPE_ROWS& inResult) PURE;

	BaseDB& SetHost(const std::string& _host) { strHost = _host;  return *this; }
	BaseDB& SetUser(const std::string& _user) { strUser = _user;  return *this; }
	BaseDB& SetPassword(const std::string& _password) { strPassword = _password;  return *this; }
	BaseDB& SetDB(const std::string& _strDB) { strDB = _strDB;  return *this; }
	BaseDB& SetTable(const std::string& _strTable) { strTable = _strTable; return *this; }
	BaseDB& SetSqlPort(int _port) { iPort = _port; return *this; }

	const std::string& GetHost()		{ return strHost; }
	const std::string& GetUser()		{ return strUser; }
	const std::string& GetPassword()	{ return strPassword; }
	const std::string& GetDB()			{ return strDB; }
	const std::string& GetTable()		{ return strTable; }
};

