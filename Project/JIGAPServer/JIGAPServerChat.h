#pragma once
#include "JIGAPServerBase.h"
class JIGAPServerChat :
	public JIGAPServerBase
{
	// JIGAPServerBase��(��) ���� ��ӵ�
	virtual void OnInitialize() override;
	virtual void OnRelease() override;
	virtual void OnConnect() override;
	virtual void OnProcess() override;

	
};

