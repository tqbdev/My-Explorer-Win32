#include "stdafx.h"
#include "Convert.h"

enum Type
{
	Bytes,
	KB,
	MB,
	GB,
	TB
};

namespace MyExplorer
{
	std::wstring Convert::ConvertVol(long long size)
	{
		using namespace std;
		unsigned int sizeType = 0;

		while (size >= 1048576)
		{
			size /= 1024;
			sizeType++;
		}

		long long nRight;

		if (size >= 1024)
		{
			//Lấy một chữ số sau thập phân của nSize chứa trong nRight
			nRight = size % 1024;

			while (nRight > 99)
				nRight /= 10;

			size /= 1024;
			sizeType++;
		}
		else
		{
			nRight = 0;
		}

		wstring result = to_wstring(size);
		result += L".";
		result += to_wstring(nRight);

		switch (sizeType)
		{
		case Type::Bytes:
			result += L" bytes";
			break;
		case Type::KB:
			result += L" KB";
			break;
		case Type::MB:
			result += L" MB";
			break;
		case Type::GB:
			result += L" GB";
			break;
		case Type::TB:
			result += L" TB";
			break;
		}

		return result;
	}

	LPWSTR Convert::ConvertDate(const FILETIME &ftLastWrite)
	{
		//Chuyển đổi sang local time
		SYSTEMTIME stUTC, stLocal;
		FileTimeToSystemTime(&ftLastWrite, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

		TCHAR *buffer = new TCHAR[50];
		wsprintf(buffer, L"%02d/%02d/%04d %02d:%02d %s",
			stLocal.wDay, stLocal.wMonth, stLocal.wYear,
			(stLocal.wHour>12) ? (stLocal.wHour / 12) : (stLocal.wHour),
			stLocal.wMinute,
			(stLocal.wHour>12) ? L"Chiều" : L"Sáng");

		return buffer;
	}
}