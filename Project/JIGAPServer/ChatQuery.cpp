#include "pch.h"
#include "ChatQuery.h"
#include "MySqlDB.h"
#include "ChatUserData.h"

void ChatQuery::InitializeQuery()
{
	lpDB = new MySqlDB();
	lpDB->SetUser("root").SetHost("localhost").SetSqlPort(3306).SetDB("jigapChatserver").SetPassword("@MisterLee8633");
	lpDB->ConnectToDB();
}

void ChatQuery::ReleaseQuery()
{
	lpDB->DisconnectToDB();
	SAFE_DELETE(lpDB);
}

bool ChatQuery::CheckUserDataToDB(const std::string& strInId)
{
	char ch[256];
	sprintf(ch, "select * from userdatas where id='%s'", strInId.c_str());

	QUERYRESULT result = lpDB->WriteQuery(ch);

	if (result == QUERYRESULT::FAILE)
		return false;

	if (result == QUERYRESULT::EMPTY)
		return false;

	return true;
}

bool ChatQuery::FindUserDataToDB(const std::string& strInId, ChatUserData* lpInUser)
{
	char ch[256];
	sprintf(ch, "select * from userdatas where id='%s'", strInId.c_str());
	
	TYPE_ROW row;
	QUERYRESULT result = lpDB->ReadRow(ch, row);

	if (result == QUERYRESULT::FAILE)
		return false;

	if (result == QUERYRESULT::EMPTY)
		return false;

	lpInUser->SetUserID(strInId);
	lpInUser->SetUserID(row["id"]);

	return true;
}

bool ChatQuery::InsertUserDataToDB(const std::string& strInId, const std::string & strInPassword, const std::string & strInName)
{
	if (CheckUserDataToDB(strInId)) {
		return false;
	}

	char ch[256];
	sprintf(ch, "insert into userdatas values('%s','%s','%s')", strInId.c_str(), strInPassword.c_str(), strInName.c_str());

	if (lpDB->WriteQuery(ch) == QUERYRESULT::FAILE)
		return false;

	return true;
}
