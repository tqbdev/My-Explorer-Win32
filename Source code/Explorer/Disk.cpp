#include "stdafx.h"
#include "Disk.h"
#include "resource.h"

#define DISK_NAME_LEN 30
#define DISK_PATH_LEN 4

// Sử dụng cho hàm SHGetDiskFreeSpaceEx
#include <Shellapi.h>
#pragma comment(lib, "Shell32.lib")

namespace MyExplorer
{
	/*-----------------------------------------------------------------------------------------------------*/
	Disk::Disk()
	{
		this->diskName_.reserve(DISK_NAME_LEN);
		this->diskPath_.reserve(DISK_PATH_LEN);
		this->fullDiskName_.reserve(DISK_NAME_LEN + DISK_NAME_LEN + 1);
		this->idIcon_ = -1;
	}

	void Disk::Create(std::wstring diskPath, std::wstring diskName, long idIcon)
	{
		this->diskPath_ = diskPath;
		this->diskName_ = diskName;
		this->fullDiskName_ = this->diskName_ + L" (" + this->diskPath_.substr(0, 2) + L")";
		this->idIcon_ = idIcon;
	}

	Disk::~Disk()
	{
		this->idIcon_ = -1;
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	LPWSTR Disk::GetDiskPath() const
	{
		return const_cast<LPWSTR>(this->diskPath_.c_str());
	}

	LPWSTR Disk::GetDiskName() const
	{
		return const_cast<LPWSTR>(this->diskName_.c_str());
	}

	LPWSTR Disk::GetFullDiskName() const
	{
		return const_cast<LPWSTR>(this->fullDiskName_.c_str());
	}

	long Disk::GetIdIcon() const
	{
		return idIcon_;
	}

	void Disk::SetDiskPath(std::wstring &diskPath)
	{
		this->diskPath_ = diskPath;
	}

	void Disk::SetDiskName(std::wstring &diskName)
	{
		this->diskName_ = diskName;
	}

	void Disk::SetIdIcon(long ID)
	{
		this->idIcon_ = ID;
	}

	void Disk::SetAutoFullDiskName()
	{
		if (this->diskName_ == L"") this->fullDiskName_ = L"Local Disk";
		else this->fullDiskName_ = this->diskName_;
		this->fullDiskName_ += L" (" + this->diskPath_.substr(0, 2) + L")";
	}

	LPWSTR Disk::GetType()
	{
		switch (this->idIcon_)
		{
		case IDI_DISK:
			return L"Local Disk";
		case IDI_CD:
			return L"CD/DVD Rom";
		case IDI_USB:
			return L"Removable Disk";
		}

		// Nếu không có cái nào trùng thì trả về không biết
		return L"Unknown";
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	long long Disk::GetSize() const
	{
		long long nSize = 0;
		SHGetDiskFreeSpaceEx(const_cast<LPWSTR>(this->diskPath_.c_str()), NULL, (PULARGE_INTEGER)&nSize, NULL);
		return nSize;
	}

	long long Disk::GetFreeSize() const
	{
		long long freeSize;
		GetDiskFreeSpaceEx(const_cast<LPWSTR>(this->diskPath_.c_str()), NULL, NULL, (PULARGE_INTEGER)&freeSize);
		return freeSize;
	}

	LPWSTR Disk::GetSizeStr()
	{
		this->totalSize_ = Convert::ConvertVol(this->GetSize());
		return const_cast<LPWSTR>(this->totalSize_.c_str());
	}

	LPWSTR Disk::GetFreeSizeStr()
	{
		this->freeSize_ = Convert::ConvertVol(this->GetFreeSize());
		return const_cast<LPWSTR>(this->freeSize_.c_str());
	}
	/*-----------------------------------------------------------------------------------------------------*/
}