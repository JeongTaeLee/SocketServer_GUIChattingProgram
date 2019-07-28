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

QUERYRESULT MySqlDB::ReadTables(const std::string& query, TYPE_ROWS& inResult)
{
	if (mysql_real_query(mySql, query.c_str(), query.size()))
	{
		// 실패
		throw std::exception("Sql 구문 오류");
		return QUERYRESULT::FAILE;
	}

	MYSQL_RES* result = mysql_store_result(&connection);
	if (result) // 리턴된 결과가 있다.
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
	else // 리턴된 Row가 업다.
	{
		if (mysql_field_count(&connection) == 0) // Row가 리턴되지 않은 쿼리인지.
			int numRows = mysql_affected_rows(&connection);	// Delete 등에서 몇개의 결과가 영향을 받았는지.
		else // 무언가 Error가 있을 시.
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
		// 실패
		throw std::exception("Sql 구문 오류");
		return QUERYRESULT::FAILE;
	}

	MYSQL_RES* result = mysql_store_result(&connection);
	if (result) // 리턴된 로우가 있다.
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
	else // 리턴된 결과가 없다.
	{
		if (mysql_field_count(&connection) == 0) // Row가 리턴되지 않은 쿼리인지.
			int numRows = mysql_affected_rows(&connection);	// Delete 등에서 몇개의 Row가 영향을 받았는지.
		else // 무언가 Error가 있을 시.
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
		// 실패
		throw std::exception("Sql 구문 오류");
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
		if (mysql_field_count(&connection) == 0) // Row가 리턴되지 않은 쿼리인지.
			int numRows = mysql_affected_rows(&connection);	// Delete 등에서 몇개의 Row가 영향을 받았는지.
		else // 무언가 Error가 있을 시.
		{
			DEBUG_LOG("MySql Error! : " << mysql_error(&connection));
			mysql_free_result(result);
			return QUERYRESULT::FAILE;
		}
	}

	mysql_free_result(result);
	return QUERYRESULT::SUCCESS;
}