#pragma once
#include "COMException.h"

namespace fe
{
	class ErrorLogger
	{
	public:
		static void Log(std::string message);
		static void Log(HRESULT hr, std::string message);
		static void Log(HRESULT hr, std::wstring message);
		static void Log(COMException& exception);
	};
}
