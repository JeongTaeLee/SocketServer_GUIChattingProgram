#pragma once
#include "JIGAPServerBase.h"
class JIGAPServerChat :
	public JIGAPServerBase
{
	// JIGAPServerBase을(를) 통해 상속됨
	virtual void OnInitialize() override;
	virtual void OnRelease() override;
	virtual void OnConnect() override;
	virtual void OnProcess() override;

	
};

