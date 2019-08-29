#include "pch.h"
#include "PathException.h"

JIGAPServerAPI::PathException::PathException(int inLine, const std::wstring& inFilePath, const std::wstring inMessage)
{
	iLine = inLine;
	strFilePath = inFilePath;

	std::string conversionException;
	conversionException.assign(inMessage.begin(), inMessage.end());

	std::exception(conversionException.c_str());
}

JIGAPServerAPI::PathException::~PathException()
{
}
