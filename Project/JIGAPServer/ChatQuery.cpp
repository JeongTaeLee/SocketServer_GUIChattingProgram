#include "pch.h"
#include "ChatQuery.h"
#include "MySqlDB.h"
#include "ChatUserData.h"
#include "BaseDB.h"

bool ChatQuery::InitializeQuery()
{
	iniReader reader;
	reader.Open("./ServerConfiguration.ini");

	if (reader.Failed())
	{
		char port[256];
		sprintf(port, "%d", 3306);

		iniWriter writer;
		writer.Open("./ServerConfiguration.ini");
		writer.WriteValue("DB", "User", "root");
		writer.WriteValue("DB", "Host", "localhost");
		writer.WriteValue("DB", "Port", port);
		writer.WriteValue("DB", "Database", "jigapchatserver");
		writer.WriteValue("DB", "Password", "@MisterLee8633");
		writer.WriteValue("DB", "Table", "userdatas");
	
		reader.Open("./ServerConfiguration.ini");
	}

	std::string User;
	reader.ReadValue("DB", "User", User);

	std::string Host;
	reader.ReadValue("DB", "Host", Host);

	std::string Port;
	reader.ReadValue("DB", "Port", Port);

	std::string Database;
	reader.ReadValue("DB", "Database", Database);

	std::string Password;
	reader.ReadValue("DB", "Password", Password);

	std::string Table;
	reader.ReadValue("DB", "Table", Table);

	if (User == "null" || Host == "null" || Port == "null" || Database == "null" || Password == "null" || Table == "null")
	{
		throw std::exception("Server Configuration File Error");
		return false;
	}

	lpDB = new MySqlDB();
	lpDB->SetUser(User).SetHost(Host).SetSqlPort(atoi(Port.c_str())).SetDB(Database).SetPassword(Password).SetTable(Table);
	return lpDB->ConnectToDB();
}

void ChatQuery::ReleaseQuery()
{
	lpDB->DisconnectToDB();
	SAFE_DELETE(lpDB);
}

bool ChatQuery::CheckUserDataToDB(const std::string& strInId)
{
	char ch[256];
	sprintf(ch, "select * from %s where id='%s'", lpDB->GetTable().c_str(), strInId.c_str());

	QUERYRESULT result = lpDB->WriteQuery(ch);

	if (result == QUERYRESULT::FAILE)
		return false;

	if (result == QUERYRESULT::EMPTY)
		return false;

	return true;
}

bool ChatQuery::FindUserDataToDB(const std::string& strInId, TYPE_ROW & row)
{
	char ch[256];
	sprintf(ch, "select * from %s where id='%s'", lpDB->GetTable().c_str(), strInId.c_str());
	
	QUERYRESULT result = lpDB->ReadRow(ch, row);

	if (result == QUERYRESULT::FAILE)
		return false;

	if (result == QUERYRESULT::EMPTY)
		return false;

	return true;
}

bool ChatQuery::CheckDuplicationUserName(const std::string& strInId)
{
	char ch[256];
	sprintf(ch, "select * from %s where name='%s'", lpDB->GetTable().c_str(), strInId.c_str());

	QUERYRESULT result = lpDB->WriteQuery(ch);

	if (result == QUERYRESULT::FAILE)
		return false;

	if (result == QUERYRESULT::EMPTY)
		return false;

	return true;
}

bool ChatQuery::InsertUserDataToDB(const std::string& strInId, const std::string & strInPassword, const std::string & strInName)
{
	if (CheckUserDataToDB(strInId)) {
		return false;
	}

	char ch[256];
	sprintf(ch, "insert into %s values('%s','%s','%s')", lpDB->GetTable().c_str(), strInId.c_str(), strInPassword.c_str(), strInName.c_str());

	if (lpDB->WriteQuery(ch) == QUERYRESULT::FAILE)
		return false;

	return true;
}
