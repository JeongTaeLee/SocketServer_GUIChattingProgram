#include "pch.h"
#include "iniWriter.h"

void iniWriter::Open(const std::string& inFileName)
{
	fileName = inFileName;
}

bool iniWriter::WriteValue(const std::string& section, const std::string key, const std::string value)
{
	bool b = WritePrivateProfileStringA(section.c_str(), key.c_str(), value.c_str(), fileName.c_str());
	return b;
}
