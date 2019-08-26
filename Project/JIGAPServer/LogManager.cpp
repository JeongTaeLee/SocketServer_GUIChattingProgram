#include "pch.h"
#include "LogManager.h"

void LogManager::Initialize(void * inLogFunc)
{
	lpLogFunc = (LogFunc)inLogFunc;
}

void LogManager::Log(const wchar_t* fileName, int line, const char* fmt, ...)
{
	char buf[1024] = { 0 };
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	wchar_t tBuf[2048];
	MultiByteToWideChar(CP_ACP, 0, buf, -1, tBuf, 2048);
	
	if (lpLogFunc)
		lpLogFunc((int*)tBuf);
}

void LogManager::LogError(const wchar_t * fileName, int line, const char* fmt, ...)
{
	char buf[1024] = { 0 };
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	wchar_t tBuf[512];
	MultiByteToWideChar(CP_ACP, 0, buf, -1, tBuf, 512);

	wchar_t  logBuf[2048];
	swprintf(logBuf, L"%s(%d): Error!: %s", fileName, line, tBuf);

	if (lpLogFunc)
		lpLogFunc((int*)logBuf);
}
