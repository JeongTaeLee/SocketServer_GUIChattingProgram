#include "pch.h"
#include "MySqlDB.h"

bool MySqlDB::ConnectToDB()
{
	if (mysql_init(&connection) == NULL)
	{
		DEBUG_LOG("MySql를 초기화 하지 못했습니다.");
		throw std::exception("MySql를 초기화 하지 못했습니다.");
		return false;
	}

	mySql = mysql_real_connect(&connection, strHost.c_str(), strUser.c_str(),
		strPassword.c_str(), strDB.c_str(), iPort, NULL, 0);
	if (mySql == nullptr)
	{
		DEBUG_LOG("MySql를 연결 하지 못했습니다.");
		throw std::exception("MySql를 연결 하지 못했습니다.");
		return false;
	}

	if (mysql_select_db(&connection, strDB.c_str()))
	{
		DEBUG_LOG("MySql DB를 선택하지 못했습니다");
		throw std::exception("MySql DB를 선택하지 못했습니다");
		return false;
	}

	return true;
}

void MySqlDB::DisconnectToDB()
{
	mysql_close(mySql);
}

bool MySqlDB::WriteQuery(const std::string& query)
{
	int state = mysql_query(mySql, query.c_str());

	if (state == 0)
	{
		mySqlRes = mysql_store_result(mySql);
		MYSQL_FIELD * fields = mysql_fetch_fields(mySqlRes);
		while ((mySqlRow = mysql_fetch_row(mySqlRes)) != NULL)
		{
			int field = mysql_num_fields(mySqlRes);
			DEBUG_LOG("S");
		}
		mysql_free_result(mySqlRes);
		return true;
	}
	else
	{
		DEBUG_LOG("Query 입력에 실패했습니다.");
		return false;
	}
}
