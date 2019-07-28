#pragma once
#include "BaseQuery.h"


class ChatUserData;

class ChatQuery :
	public BaseQuery
{
public:
	virtual void InitializeQuery() override;
	virtual void ReleaseQuery() override;

	
	/*������ DB�� ����Ǿ��ִ��� Ȯ���մϴ�.
	TRUE : ������ �����մϴ�
	FALSE : ������ �������� �ʽ��ϴ�*/
	bool CheckUserDataToDB(const std::string & strInId);
	bool FindUserDataToDB(const std::string& strInId, ChatUserData * lpInUser);
	bool InsertUserDataToDB(const std::string& strInId, const std::string& strInPassword, const std::string& strInName);
};

