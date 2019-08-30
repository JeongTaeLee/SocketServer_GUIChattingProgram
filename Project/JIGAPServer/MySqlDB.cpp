#include "pch.h"
#include "MySqlDB.h"

#include "LogManager.h"

bool MySqlDB::ConnectToDB()
{
	if (mysql_init(&connection) == NULL)
		throw CustomException(__LINE__, __FILEW__, "MySql를 초기화 하지 못했습니다.");

	mySql = mysql_real_connect(&connection, strHost.c_str(), strUser.c_str(),
		strPassword.c_str(), strDB.c_str(), iPort, NULL, 0);
	if (mySql == nullptr)
		throw CustomException(__LINE__, __FILEW__, "My sql connect fail ");

	if (mysql_select_db(&connection, strDB.c_str()))
		throw CustomException(__LINE__, __FILEW__, "MySql faild use DB!");

	return true;
}

void MySqlDB::DisconnectToDB()
{
	mysql_close(mySql);
}

QUERYRESULT MySqlDB::ReadTables(const std::string& query, TYPE_ROWS& inResult)
{
	MYSQL_RES* sqlRes = nullptr;
	
	try
	{
		if (mysql_real_query(mySql, query.c_str(), query.size()))
			throw CustomException(__LINE__, __FILEW__, "Query Read Error");

		sqlRes = mysql_store_result(&connection);

		if (sqlRes) // 리턴된 결과가 있다.
		{
			MYSQL_FIELD* sqlField = mysql_fetch_field(sqlRes);
			MYSQL_ROW sqlRow = mysql_fetch_row(sqlRes);

			int fieldCount = mysql_num_fields(sqlRes);
			int rowCount = mysql_num_rows(sqlRes);
			

			if (sqlRow == nullptr)
			{
				mysql_free_result(sqlRes);
				return QUERYRESULT::EMPTY;
			}

			inResult.resize(rowCount);

			for (int i = 0; i < rowCount; ++i)
			{
				for (int j = 0; j < fieldCount; ++j)
				{
					if (sqlRow[j] != nullptr)
						inResult[i].insert(TYPE_ROW::value_type(sqlField[j].name, sqlRow[j]));
					else
						inResult[i].insert(TYPE_ROW::value_type(sqlField[j].name, "NULL"));
				}

				sqlRow = mysql_fetch_row(sqlRes);
			}
		}
		else // 리턴된 Row가 업다.
		{
			if (mysql_field_count(&connection) == 0) // Row가 리턴되지 않은 쿼리인지.
				int numRows = mysql_affected_rows(&connection);	// Delete 등에서 몇개의 결과가 영향을 받았는지.
			else // 무언가 Error가 있을 시.
				throw CustomException(__LINE__, __FILEW__, "Query Read Error");
		}
	}
	catch (CustomException& ex)
	{
		LOGMANAGER.LogError(ex.WhatPath().c_str(), ex.WhatLine(), ex.what());

		if (sqlRes != nullptr)
			mysql_free_result(sqlRes);
		
		return QUERYRESULT::FAILE;
	}

	if (sqlRes != nullptr)
		mysql_free_result(sqlRes);

	return QUERYRESULT::SUCCESS;
}

QUERYRESULT MySqlDB::ReadRow(const std::string& query, TYPE_ROW& inResult)
{
	MYSQL_RES* sqlRes = nullptr;

	try
	{
		if (mysql_real_query(mySql, query.c_str(), query.size()))
			throw CustomException(__LINE__, __FILEW__, "Query Read Error");

		sqlRes = mysql_store_result(&connection);

		if (sqlRes)
		{
			MYSQL_FIELD* sqlField = mysql_fetch_field(sqlRes);
			MYSQL_ROW sqlRow = mysql_fetch_row(sqlRes);

			int fieldCount = mysql_num_fields(sqlRes);
			int rowCount = mysql_num_rows(sqlRes);

			if (sqlRow == nullptr)
			{
				mysql_free_result(sqlRes);
				return QUERYRESULT::EMPTY;
			}

			for (int j = 0; j < fieldCount; ++j)
			{
				if (sqlRow[j] != nullptr)
					inResult.insert(TYPE_ROW::value_type(sqlField[j].name, sqlRow[j]));
				else
					inResult.insert(TYPE_ROW::value_type(sqlField[j].name, "NULL"));
			}
		}
		else 
		{
			if (mysql_field_count(&connection) == 0)
				int numRows = mysql_affected_rows(&connection);
			else
				throw CustomException(__LINE__, __FILEW__, "Query Read Error");
		}
	}
	catch(CustomException & ex)
	{
		LOGMANAGER.LogError(ex.WhatPath().c_str(), ex.WhatLine(), ex.what());

		if (sqlRes != nullptr)
			mysql_free_result(sqlRes);
		return QUERYRESULT::FAILE;
	}

	if (sqlRes != nullptr)
		mysql_free_result(sqlRes);

	return QUERYRESULT::SUCCESS;
}

QUERYRESULT MySqlDB::WriteQuery(const std::string& query)
{
	MYSQL_RES* sqlRes = nullptr;

	try
	{
		if (mysql_real_query(mySql, query.c_str(), query.size()))
			throw CustomException(__LINE__, __FILEW__, "Query Write Error!");

		sqlRes = mysql_store_result(&connection);

		if (sqlRes)
		{
			MYSQL_ROW sqlRow = mysql_fetch_row(sqlRes);

			if (sqlRow == nullptr)
			{
				mysql_free_result(sqlRes);
				return QUERYRESULT::EMPTY;
			}
		}
		else
		{
			if (mysql_field_count(&connection) == 0) // Row가 리턴되지 않은 쿼리인지.
				int numRows = mysql_affected_rows(&connection);	// Delete 등에서 몇개의 Row가 영향을 받았는지.
			else // 무언가 Error가 있을 시.
				throw CustomException(__LINE__, __FILEW__, "Query Write Error!");
		}
	}
	catch (CustomException& ex)
	{
		LOGMANAGER.LogError(ex.WhatPath().c_str(), ex.WhatLine(), ex.what());

		if (sqlRes != nullptr)
			mysql_free_result(sqlRes);

		return QUERYRESULT::FAILE;
	}

	if (sqlRes != nullptr)
		mysql_free_result(sqlRes);

	return QUERYRESULT::SUCCESS;
}