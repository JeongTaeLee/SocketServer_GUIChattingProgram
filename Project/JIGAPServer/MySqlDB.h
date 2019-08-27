#pragma once
#include "BaseDB.h"
class MySqlDB :
	public BaseDB
{
private:
	MYSQL* mySql = nullptr;
	MYSQL connection;


public:
	virtual bool ConnectToDB() override;
	virtual void DisconnectToDB() override;

	///Sql에 쿼리를 입력합니다.
	/// 인자 : 쿼리
	virtual QUERYRESULT WriteQuery(const std::string& inQuery) override;
	
	/// 데이터베이스에 한 열을 읽어 옵니다.
	/// 인자 : 쿼리, 읽어온 테이블을 저장할 컨테이너
	virtual QUERYRESULT ReadRow(const std::string& inQuery, TYPE_ROW & inResult) override;
	
	/// 테이블을 읽어 옵니다.
	/// 인자 : 쿼리, 읽어온 테이블을 저장할 컨테이너
	virtual QUERYRESULT ReadTables(const std::string& inQuery, TYPE_ROWS& inResult) override;
};

