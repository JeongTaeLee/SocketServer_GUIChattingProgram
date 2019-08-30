#pragma once

namespace JIGAPServerAPI
{
	enum QueryResult
	{
		eQueryFailed = 0, // 쿼리 구문자체가 실패함.
		eQuerySuccess, // 쿼리 성공
		eQueryEmptySuccess // 쿼리 구문은 성공했지만 결과값이 없음. 
	};

	using Row = std::hash_map<std::string, std::string>;
	using Table = std::vector < std::hash_map < std::string, std::string >>;

	class BaseDataBase
	{
	private:
		std::string strHost;
		std::string strUser;
		std::string strPassword;
		std::string strDB;
		std::string strTable;
	
		int iPort = 0;
	public:
		virtual QueryResult WriteQuery(const std::string & inQuery) = 0;
		virtual QueryResult SelectRow(const std::string & inQuery, const Row & inRow) = 0;
		virtual QueryResult SelectTable(const std::string& inQuery, const Table & inRow) = 0;

		BaseDataBase& SetHost(const std::string& _host) { strHost = _host;  return *this; }
		BaseDataBase& SetUser(const std::string& _user) { strUser = _user;  return *this; }
		BaseDataBase& SetPassword(const std::string& _password) { strPassword = _password;  return *this; }
		BaseDataBase& SetDB(const std::string& _strDB) { strDB = _strDB;  return *this; }
		BaseDataBase& SetTable(const std::string& _strTable) { strTable = _strTable; return *this; }
		BaseDataBase& SetSqlPort(int _port) { iPort = _port; return *this; }
	
		const std::string& GetHost() { return strHost; }
		const std::string& GetUser() { return strUser; }
		const std::string& GetPassword() { return strPassword; }
		const std::string& GetDB() { return strDB; }
		const std::string& GetTable() { return strTable; }
		int GetPort() { return iPort; }
	};
}