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

QUERYRESULT MySqlDB::ReadTables(const std::string& query, TYPE_ROWS& inResult)
{
	if (mysql_real_query(mySql, query.c_str(), query.size()))
	{
		// ����
		throw std::exception("Sql ���� ����");
		return QUERYRESULT::FAILE;
	}

	MYSQL_RES* result = mysql_store_result(&connection);
	if (result) // ���ϵ� ����� �ִ�.
	{
		MYSQL_FIELD* lpField = mysql_fetch_field(result);
		int fieldCount = mysql_num_fields(result);

		MYSQL_ROW lpRow = mysql_fetch_row(result);
		int rowCount = mysql_num_rows(result);

		if (lpRow == nullptr)
		{
			mysql_free_result(result);
			return QUERYRESULT::EMPTY;
		}

		inResult.resize(rowCount);

		for (int i = 0; i < rowCount; ++i)
		{
			for (int j = 0; j < fieldCount; ++j)
			{
				if (lpRow[j] != nullptr)
					inResult[i].insert(TYPE_ROW::value_type(lpField[j].name, lpRow[j]));
				else
					inResult[i].insert(TYPE_ROW::value_type(lpField[j].name, "NULL"));
			}

			lpRow = mysql_fetch_row(result);
		}
	}
	else // ���ϵ� Row�� ����.
	{
		if (mysql_field_count(&connection) == 0) // Row�� ���ϵ��� ���� ��������.
			int numRows = mysql_affected_rows(&connection);	// Delete ��� ��� ����� ������ �޾Ҵ���.
		else // ���� Error�� ���� ��.
		{
			DEBUG_LOG("MySql Error! : " << mysql_error(&connection));
			mysql_free_result(result);
			return QUERYRESULT::FAILE;
		}
	}

	mysql_free_result(result);
	return QUERYRESULT::SUCCESS;
}

QUERYRESULT MySqlDB::ReadRow(const std::string& query, TYPE_ROW& inResult)
{
	if (mysql_real_query(mySql, query.c_str(), query.size()))
	{
		// ����
		throw std::exception("Sql ���� ����");
		return QUERYRESULT::FAILE;
	}

	MYSQL_RES* result = mysql_store_result(&connection);
	if (result) // ���ϵ� �ο찡 �ִ�.
	{
		MYSQL_FIELD* lpField = mysql_fetch_field(result);
		int fieldCount = mysql_num_fields(result);

		MYSQL_ROW lpRow = mysql_fetch_row(result);
		int rowCount = mysql_num_rows(result);

		if (lpRow == nullptr)
		{
			mysql_free_result(result);
			return QUERYRESULT::EMPTY;
		}

		if (lpRow != nullptr)
		{
			for (int j = 0; j < fieldCount; ++j)
			{
				if (lpRow[j] != nullptr)
					inResult.insert(TYPE_ROW::value_type(lpField[j].name, lpRow[j]));
				else
					inResult.insert(TYPE_ROW::value_type(lpField[j].name, "NULL"));
			}
		}

	}
	else // ���ϵ� ����� ����.
	{
		if (mysql_field_count(&connection) == 0) // Row�� ���ϵ��� ���� ��������.
			int numRows = mysql_affected_rows(&connection);	// Delete ��� ��� Row�� ������ �޾Ҵ���.
		else // ���� Error�� ���� ��.
		{
			DEBUG_LOG("MySql Error! : " << mysql_error(&connection));

			mysql_free_result(result);
			return QUERYRESULT::FAILE;
		}
	}

	mysql_free_result(result);
	return QUERYRESULT::SUCCESS;
}

QUERYRESULT MySqlDB::WriteQuery(const std::string& query)
{
	if (mysql_real_query(mySql, query.c_str(), query.size()))
	{
		// ����
		throw std::exception("Sql ���� ����");
		return QUERYRESULT::FAILE;
	}

	MYSQL_RES* result = mysql_store_result(&connection);
	if (result)
	{
		MYSQL_ROW lpRow = mysql_fetch_row(result);
		
		if (lpRow == nullptr)
		{
			mysql_free_result(result);
			return QUERYRESULT::EMPTY;
		}
	}
	else
	{
		if (mysql_field_count(&connection) == 0) // Row�� ���ϵ��� ���� ��������.
			int numRows = mysql_affected_rows(&connection);	// Delete ��� ��� Row�� ������ �޾Ҵ���.
		else // ���� Error�� ���� ��.
		{
			DEBUG_LOG("MySql Error! : " << mysql_error(&connection));
			mysql_free_result(result);
			return QUERYRESULT::FAILE;
		}
	}

	mysql_free_result(result);
	return QUERYRESULT::SUCCESS;
}