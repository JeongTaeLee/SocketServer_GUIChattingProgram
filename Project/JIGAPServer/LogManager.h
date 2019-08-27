#pragma once
#include "singleton.h"

typedef void (*LogFunc)(int* strLog);

class LogManager :
	public singleton<LogManager>
{
private:
	std::queue<std::string> qLogs;

	LogFunc lpLogFunc = nullptr;
public:
	void Initialize(void* inLogFunc);

	void Log(const char* fmt, ...);
	void LogError(const wchar_t* fileName, int line, const char* fmt, ...);
};

#define LOGMANAGER LogManager::GetInst()


