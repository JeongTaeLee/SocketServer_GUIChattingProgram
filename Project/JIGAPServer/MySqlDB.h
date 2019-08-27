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

	///Sql�� ������ �Է��մϴ�.
	/// ���� : ����
	virtual QUERYRESULT WriteQuery(const std::string& inQuery) override;
	
	/// �����ͺ��̽��� �� ���� �о� �ɴϴ�.
	/// ���� : ����, �о�� ���̺��� ������ �����̳�
	virtual QUERYRESULT ReadRow(const std::string& inQuery, TYPE_ROW & inResult) override;
	
	/// ���̺��� �о� �ɴϴ�.
	/// ���� : ����, �о�� ���̺��� ������ �����̳�
	virtual QUERYRESULT ReadTables(const std::string& inQuery, TYPE_ROWS& inResult) override;
};

