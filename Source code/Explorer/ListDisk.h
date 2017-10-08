#pragma once
#include "Disk.h"

namespace MyExplorer
{
	class ListDisk
	{
	private:
		Disk*			list_; // Danh sách chứa thông tin các ổ đĩa
		int				amount_; // Số lượng các ổ đĩa hiện có trong danh sách
	public:
		ListDisk();
		~ListDisk();

		// Lấy thông tin của từng ổ đĩa
		// Thông tin cơ bản
		LPWSTR			GetDiskPath(const int &index) const;
		LPWSTR			GetDiskName(const int &index) const;
		LPWSTR			GetFullDiskName(const int &index) const;
		long			GetIconID(const int &index) const;
		LPWSTR			GetType(const int &index) const;
		//
		// Thông tin về dung lượng
		LPWSTR			GetSizeStr(const int &index) const;
		LPWSTR			GetFreeSizeStr(const int &index) const;
		//
		//
		// Xử lý của ListDisk
		int				GetAmount() const;
		void			GetDisksOnSystem(); // Lấy các ổ địa hiện có trong máy tính
		//
	};
}