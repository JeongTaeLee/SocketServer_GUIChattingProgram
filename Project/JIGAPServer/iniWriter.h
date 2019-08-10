#pragma once
class iniWriter
{
private:
	std::string fileName;

public:
	void Open(const std::string& inFileName);

	bool WriteValue(const std::string& section, const std::string key, const std::string value);
};

