#include "pch.h"
#include "CustomException.h"

CustomException::CustomException(int inLine, const std::wstring& inFilePath, const std::string& inErrorMessage)
	:std::exception(inErrorMessage.c_str())
{
	iLine = inLine;
	strFilePath = inFilePath;
}

CustomException::~CustomException()
{
}
