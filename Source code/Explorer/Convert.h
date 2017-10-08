#pragma once
#include <string>

namespace MyExplorer
{
	class Convert
	{
	public:
		static std::wstring ConvertVol(long long size);
		static LPWSTR		ConvertDate(const FILETIME &ftLastWrite);
	};
}