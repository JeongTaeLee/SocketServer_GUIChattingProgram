#pragma once
#include <exception>

namespace JIGAPServerAPI
{
	class PathException :
		public std::exception
	{
	protected:
		int iLine;
		std::wstring strFilePath;

	public:
		PathException(int inLine, const std::wstring& inFilePath, const std::wstring inMessage);
		virtual ~PathException();

		int WhatLine() { return iLine; }
		std::wstring WhatPath() { return strFilePath; }
	};

#define PException(message) PathException(__LINE__, __FILEW__, message)
}

