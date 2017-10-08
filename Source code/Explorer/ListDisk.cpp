#include "stdafx.h"
#include "ListDisk.h"
#include "resource.h"

#define BUFFER_LEN 256

namespace MyExplorer
{
	ListDisk::ListDisk()
	{
		this->list_ = NULL;
		this->amount_ = -1;
	}

	ListDisk::~ListDisk()
	{
		delete[] this->list_;
		this->amount_ = -1;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	LPWSTR ListDisk::GetDiskPath(const int &index) const
	{
		if (index >= 0 && index < this->amount_)
		{
			return this->list_[index].GetDiskPath();
		}

		return NULL;
	}

	LPWSTR ListDisk::GetDiskName(const int &index) const
	{
		if (index >= 0 && index < this->amount_)
		{
			return this->list_[index].GetDiskName();
		}

		return NULL;
	}

	LPWSTR ListDisk::GetFullDiskName(const int &index) const
	{
		if (index >= 0 && index < this->amount_)
		{
			return this->list_[index].GetFullDiskName();
		}

		return NULL;
	}

	long ListDisk::GetIconID(const int &index) const
	{
		if (index >= 0 && index < this->amount_)
		{
			return this->list_[index].GetIdIcon();
		}

		return NULL;
	}

	LPWSTR ListDisk::GetType(const int &index) const
	{
		if (index >= 0 && index < this->amount_)
		{
			return this->list_[index].GetType();
		}

		return NULL;
	}

	LPWSTR ListDisk::GetSizeStr(const int &index) const
	{
		if (index >= 0 && index < this->amount_)
		{
			return this->list_[index].GetSizeStr();
		}

		return NULL;
	}

	LPWSTR ListDisk::GetFreeSizeStr(const int &index) const
	{
		if (index >= 0 && index < this->amount_)
		{
			return this->list_[index].GetFreeSizeStr();
		}

		return NULL;
	}
	/*-----------------------------------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------------------------------*/
	int	ListDisk::GetAmount() const
	{
		return this->amount_;
	}

	void ListDisk::GetDisksOnSystem()
	{
		TCHAR buffer[BUFFER_LEN];
		GetLogicalDriveStrings(BUFFER_LEN, buffer);

		// Get amount of disks
		this->amount_ = 0;
		for (int i = 0; !((buffer[i] == 0) && (buffer[i + 1] == 0)); ++i)
		{
			if (buffer[i] == 0)
			{
				this->amount_++;
			}
		}
		this->amount_++;

		// Cấp phát đúng số lượng cho list_
		this->list_ = new Disk[this->amount_];

		std::wstring temp;
		temp.reserve(50);

		// Lấy diskpart
		int j = 0;
		for (int i = 0; i < this->amount_; i++)
		{
			while (buffer[j] != 0)
			{
				temp += buffer[j++];
			}
			list_[i].SetDiskPath(temp);
			temp.clear();
			j++;
		}

		// Lấy dạng ổ đĩa HDD (SSD), USB, ổ quang
		long driveType;

		for (int i = 0; i < this->amount_; i++)
		{
			driveType = GetDriveType(this->list_[i].GetDiskPath());

			if ((driveType == DRIVE_FIXED) || ((i > 1) && (driveType == DRIVE_REMOVABLE))) // Là HDD (SSD) hay USB
			{
				if (driveType == DRIVE_FIXED)
				{
					this->list_[i].SetIdIcon(IDI_DISK);
				}
				else
				{
					this->list_[i].SetIdIcon(IDI_USB);
				}

				GetVolumeInformation(this->list_[i].GetDiskPath(), buffer, BUFFER_LEN, NULL, NULL, NULL, NULL, 0);
				temp = buffer;
				this->list_[i].SetDiskName(temp);
			}
			else if (driveType == DRIVE_CDROM)
			{
				GetVolumeInformation(this->list_[i].GetDiskPath(), buffer, BUFFER_LEN, NULL, NULL, NULL, NULL, 0);
				if (wcslen(buffer) == 0)
				{
					temp = L"CD/DVD Rom";
					this->list_[i].SetDiskName(temp);
				}
				else
				{
					temp = buffer;
					this->list_[i].SetDiskName(temp);
				}
				this->list_[i].SetIdIcon(IDI_CD);
			}

			this->list_[i].SetAutoFullDiskName();
		}
		/*-----------------------------------------------------------------------------------------------------*/
	}
}