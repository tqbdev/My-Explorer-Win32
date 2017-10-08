#pragma once
#include "Convert.h"
#include "ListPointer.h"

namespace MyExplorer
{
	class Disk
	{
	private:
		// Thông tin cơ bản
		std::wstring		diskPath_; // Ví dụ: C:/
		std::wstring		diskName_; // Ví dụ: Windows
		std::wstring		fullDiskName_; // Ví dụ: Windows (C:)
		long				idIcon_; // ID của icon sau khi load từ resource

		// Thông tin về dung lượng
		std::wstring		freeSize_;
		std::wstring		totalSize_;
	public:
		Disk();
		~Disk();

		void Create(std::wstring diskPath, std::wstring diskName, long idIcon);

		// Getter và setter các thuộc tính
		LPWSTR				GetDiskPath() const;
		LPWSTR				GetDiskName() const;
		LPWSTR				GetFullDiskName() const;
		long				GetIdIcon() const;

		void				SetDiskPath(std::wstring &diskPath);
		void				SetDiskName(std::wstring &diskName);
		void				SetAutoFullDiskName();
		void				SetIdIcon(long ID);

		LPWSTR				GetType(); // Dùng để set cho ListView
		//
		// Get các thông tin về dung lượng ổ đĩa
		long long			GetSize() const;
		long long			GetFreeSize() const;
		LPWSTR				GetSizeStr();
		LPWSTR				GetFreeSizeStr();
		//
	};
}