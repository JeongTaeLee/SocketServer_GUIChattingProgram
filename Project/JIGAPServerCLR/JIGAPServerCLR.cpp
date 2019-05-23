#include "pch.h"
#include "JIGAPServerCLR.h"

#include <msclr/marshal_cppstd.h>
#include <msclr/marshal.h>


JIGAPServerCLR::JIGAPServerWrap::JIGAPServerWrap()
	:lpJIGAPServer(new JIGAPServer)
{
}

JIGAPServerCLR::JIGAPServerWrap::~JIGAPServerWrap()
{
	delete lpJIGAPServer;
}

bool JIGAPServerCLR::JIGAPServerWrap::JIGAPWrapServerOpen(System::String^ szIpAddr, System::String^ szPortAddr)
{
	/*System::String^ ������ std::string ������ ���ڿ��� �ٲܶ� ���� ����Դϴ�*/
	std::string _szIpAddr = msclr::interop::marshal_as < std::string > (szIpAddr);
	std::string _szPortAddr = msclr::interop::marshal_as< std::string >(szPortAddr);

	/*System::String^ ������ const char * ������ ���ڿ��� �ٲܶ� ���� ����Դϴ�.*/
	//msclr::interop::marshal_context context;
	//const char * ch = context.marshal_as<const char*>(szPortAddr);

	return lpJIGAPServer->JIGAPServerOpen(_szIpAddr, _szPortAddr);
}

void JIGAPServerCLR::JIGAPServerWrap::JIGAPWrapServerClose()
{
	lpJIGAPServer->JIGAPServerClose();
}

String^ JIGAPServerCLR::JIGAPServerWrap::JIGAPGetSystemMsg()
{
	String^ strSystemMsg = msclr::interop::marshal_as<String^>(lpJIGAPServer->JIGAPGetSystemMsg());
	return strSystemMsg;
}

bool JIGAPServerCLR::JIGAPServerWrap::JIGAPCheckSystemMsg()
{
	return lpJIGAPServer->JIGAPCheckSystemMsg();
}
