#include "pch.h"
#include "DBManager.h"
#include "MySqlDB.h"

bool DBManager::ConnenctToDB()
{
	MySqlDB * db = new MySqlDB();
	db->SetHost("localhost").SetUser("root").SetPassword("@MisterLee8633").SetSqlPort(3306).SetDB("jigapchatserver");
	lpDB = db;
	lpDB->ConnectToDB();
	db->WriteQuery("desc userdatas");
	return true;
}

void DBManager::DisconnectDB()
{
	lpDB->DisconnectToDB();
	SAFE_DELETE(lpDB);
}
