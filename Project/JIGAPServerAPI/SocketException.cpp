#include "pch.h"
#include "SocketException.h"

JIGAPServerAPI::SocketException::SocketException(int inLine, int inErrorCode, const std::wstring& inFilePath, const std::wstring inMessage)
	:PathException(inLine, inFilePath, inMessage), iErrorCode(inErrorCode)
{
}

JIGAPServerAPI::SocketException::~SocketException()
{
}
