#include "pch.h"
#include "ExceptionLoger.h"

void ExceptionLoger::ExceptionLog(const char* inFile, int inLine, const char* inMessage)
{
	std::ofstream ofstm(strExceptionFilePath);

	if (ofstm.fail())
		return;

	ofstm << "예외가 발생했습니다. FILE : " << inFile << " / LINE : " << inLine << " / MESSAGE : " << inMessage;

	ofstm.close();
}

