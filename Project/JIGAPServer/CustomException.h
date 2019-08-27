#pragma once
#include <exception>

class CustomException :
	public std::exception
{
private:
	std::wstring strFilePath;
	int iLine;

public:
	CustomException(int inLine, const std::wstring & inFilePath, const std::string & inErrorMessage);
	virtual ~CustomException();

	const std::wstring& WhatPath() { return strFilePath; }
	int WhatLine() { return iLine; }
};

