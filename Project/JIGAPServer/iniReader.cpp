#include "pch.h"
#include "iniReader.h"

iniReader::iniReader(const std::string & inIniPath)
{
	strfilePath = inIniPath;
	bCantOpenFile = _access(inIniPath.c_str(), 0);
}

bool iniReader::Open(const std::string& inIniPath)
{
	strfilePath = inIniPath;
	bCantOpenFile = _access(inIniPath.c_str(), 0);
	return bCantOpenFile;
}

void iniReader::ReadValue(const std::string& section, const std::string& key, std::string& value)
{
	char ch[256];
	GetPrivateProfileStringA(section.c_str(), key.c_str(), "null", ch, 256, strfilePath.c_str());

	value = ch;
	
}

