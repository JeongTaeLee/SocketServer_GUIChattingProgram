#include "pch.h"
#include "JIGAPServerCLR.h"

#include <msclr/marshal_cppstd.h>
#include <msclr/marshal.h>


JIGAPServerCLR::JIGAPServerWrap::JIGAPServerWrap()
{
}

JIGAPServerCLR::JIGAPServerWrap::~JIGAPServerWrap()
{
}

void JIGAPServerCLR::JIGAPServerWrap::Initialize()
{
	lpJIGAPServer = new JIGAPServer();
}

void JIGAPServerCLR::JIGAPServerWrap::Release()
{
	delete lpJIGAPServer;
	lpJIGAPServer = nullptr;
}

bool JIGAPServerCLR::JIGAPServerWrap::OpenServer(String^ inStrIp, String^ inStrPort)
{
	std::string strIp = msclr::interop::marshal_as<std::string>(inStrIp);
	std::string strPort = msclr::interop::marshal_as<std::string>(inStrPort);

	return lpJIGAPServer->StartServer(strIp, strPort);
}

void JIGAPServerCLR::JIGAPServerWrap::CloseServer()
{
	lpJIGAPServer->CloseServer();
}

void JIGAPServerCLR::JIGAPServerWrap::RegisterLogFunc(LogFunc^ lpInLogFunc)
{
	IntPtr lpFunc = Marshal::GetFunctionPointerForDelegate(lpInLogFunc);
	lpJIGAPServer->RegisterLogFunc(lpFunc.ToPointer());
}

