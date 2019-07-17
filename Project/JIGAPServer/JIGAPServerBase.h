#pragma once
class JIGAPServerBase
{
protected:

public:
	virtual void OnInitialize()		PURE;
	virtual void OnRelease()		PURE;
	virtual void OnConnect()		PURE;
	virtual void OnProcess()		PURE;
};

