#include "pch.h"
#include "MySqlDB.h"

bool MySqlDB::ConnectToDB()
{
	if (mysql_init(&connection) == NULL)
	{
		DEBUG_LOG("MySql�� �ʱ�ȭ ���� ���߽��ϴ�.");
		throw std::exception("MySql�� �ʱ�ȭ ���� ���߽��ϴ�.");
		return false;
	}

	mySql = mysql_real_connect(&connection, strHost.c_str(), strUser.c_str(),
		strPassword.c_str(), strDB.c_str(), iPort, NULL, 0);
	if (mySql == nullptr)
	{
		DEBUG_LOG("MySql�� ���� ���� ���߽��ϴ�.");
		throw std::exception("MySql�� ���� ���� ���߽��ϴ�.");
		return false;
	}

	if (mysql_select_db(&connection, strDB.c_str()))
	{
		DEBUG_LOG("MySql DB�� �������� ���߽��ϴ�");
		throw std::exception("MySql DB�� �������� ���߽��ϴ�");
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
		DEBUG_LOG("Query �Է¿� �����߽��ϴ�.");
		return false;
	}
}
