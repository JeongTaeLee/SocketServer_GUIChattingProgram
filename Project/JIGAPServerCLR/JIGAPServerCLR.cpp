#include "pch.h"
#include "JIGAPServerCLR.h"

#include <msclr/marshal_cppstd.h>
#include <msclr/marshal.h>


JIGAPServerCLR::JIGAPServerWrap::JIGAPServerWrap()
{
	lpJIGAPServer = new JIGAPServer();
}

JIGAPServerCLR::JIGAPServerWrap::~JIGAPServerWrap()
{
	delete lpJIGAPServer;
}

void JIGAPServerCLR::JIGAPServerWrap::OpenServer(String^ inStrIp, String^ inStrPort)
{
	std::string strIp = msclr::interop::marshal_as<std::string>(inStrIp);
	std::string strPort = msclr::interop::marshal_as<std::string>(inStrPort);

	lpJIGAPServer->StartServer(strIp, strPort);
}

void JIGAPServerCLR::JIGAPServerWrap::CloseServer()
{
	lpJIGAPServer->CloseServer();
}

