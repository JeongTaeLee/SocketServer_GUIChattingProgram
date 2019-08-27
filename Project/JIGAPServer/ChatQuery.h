#pragma once
#include "BaseQuery.h"
#include "BaseDB.h"

class ChatUserData;

class ChatQuery :
	public BaseQuery
{
public:
	virtual bool InitializeQuery() override;
	virtual void ReleaseQuery() override;
	
	/*������ DB�� ����Ǿ��ִ��� Ȯ���մϴ�.
	TRUE : ������ �����մϴ�
	FALSE : ������ �������� �ʽ��ϴ�*/
	bool CheckUserDataInDB(const std::string & strInId);
	bool FindUserDataInDB(const std::string& strInId, TYPE_ROW& row);
	bool CheckDuplicationUserName(const std::string& strInId);
	bool InsertUserDataInDB(const std::string& strInId, const std::string& strInPassword, const std::string& strInName);
};

