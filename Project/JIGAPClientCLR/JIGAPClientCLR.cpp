#include "pch.h"
#include "JIGAPClientCLR.h"

#include <msclr/marshal_cppstd.h>
#include <msclr/marshal.h>

JIGAPClientCLR::JIGAPClientWrap::JIGAPClientWrap()
	:lpJigapClient(new JIGAPClient)
{
}

JIGAPClientCLR::JIGAPClientWrap::~JIGAPClientWrap()
{
	delete lpJigapClient;
	lpJigapClient = nullptr;
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapClientStart(String^ szIpAddr, String^ szPortAddr)
{
	std::string _szIpAddr = msclr::interop::marshal_as < std::string >(szIpAddr);
	std::string _szPortAddr = msclr::interop::marshal_as< std::string >(szPortAddr);

	return lpJigapClient->JIGAPClientStart(_szIpAddr, _szPortAddr);
}

void JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapClientEnd()
{
	lpJigapClient->JIGAPClientEnd();
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPWrapSend(String^ szInMessage)
{
	return lpJigapClient->JIGAPSend(msclr::interop::marshal_as<std::string>(szInMessage));
}

String^ JIGAPClientCLR::JIGAPClientWrap::JIGAPGetMessageLog()
{
	String^ str = msclr::interop::marshal_as<String^>(lpJigapClient->JIGAPGetMessageLog());
	return str;
}

bool JIGAPClientCLR::JIGAPClientWrap::JIGAPCheckMessage()
{
	return lpJigapClient->JIGAPCheckMessageLog();
}
