#pragma once
class ExceptionLoger
{
private:
	std::string strExceptionFilePath;

	ExceptionLoger() {}
public:
	ExceptionLoger(std::string inStrExcpeionFilePath)
		:strExceptionFilePath(inStrExcpeionFilePath)
	{}

	void ExceptionLog(const char * inFile, int inLine, const char * inMessage);
};

