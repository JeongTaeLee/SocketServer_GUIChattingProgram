#pragma once
class iniReader
{
private:
	std::string strfilePath;

	bool bCantOpenFile = false;
public:
	iniReader() {}
	iniReader(const std::string& inIniPath);

	bool Open(const std::string& inIniPath);
	bool Failed() { return bCantOpenFile; }

	void ReadValue(const std::string& section, const std::string& key, std::string& value);

};

