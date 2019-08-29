#pragma once
#include "PathException.h"

namespace JIGAPServerAPI
{
	class SocketException :
		public PathException
	{
	protected:
		int iErrorCode;
	public:
		SocketException(int inLine, int inErrorCode, const std::wstring& inFilePath, const std::wstring inMessage);
		virtual ~SocketException();

		int WhatErrorCode() { return iErrorCode; }
	};

#define SException(errorcode, message) SocketException(__LINE__, errorcode, __FILEW__, message)
}

