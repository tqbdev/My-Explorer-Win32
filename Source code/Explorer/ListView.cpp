#include "stdafx.h"
#include "ListView.h"
#include "resource.h"

// Dùng để thực thi tập tin nếu nó chạy được
#include <Shellapi.h>
#pragma comment(lib, "Shell32.lib")

// Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace MyExplorer
{
	/*-----------------------------------------------------------------------------------------------------*/
	ListView::ListView()
	{
		this->hInst_ = NULL;
		this->hParent_ = NULL;
		this->hListView_ = NULL;
		this->ID_ = -1;

		this->lstDrive_ = NULL;

		this->hIml = NULL;
	}

	ListView::~ListView()
	{
		if (this->hIml) delete this->hIml;
		if (this->hListView_) DestroyWindow(this->hListView_);
	}

	void ListView::Create(HWND parentWnd, long ID, HINSTANCE hParentInst,
		int Width, int Height, int x, int y,
		long lExtStyle, long lStyle)
	{
		InitCommonControls();
		this->hInst_ = hParentInst;
		this->hParent_ = parentWnd;
		this->hListView_ = CreateWindowEx(lExtStyle, WC_LISTVIEW, L"List View",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | lStyle,
			x, y, Width, Height, this->hParent_, (HMENU)ID, this->hInst_, NULL);

		this->ID_ = ID;

		InitIconList();
		InitColumn();
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	void ListView::InitColumn()
	{
		LVCOLUMN lvCol;

		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_LEFT;

		lvCol.cx = 180;
		lvCol.pszText = L"Tên";
		ListView_InsertColumn(this->hListView_, 0, &lvCol);

		lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
		lvCol.cx = 100;
		lvCol.pszText = L"Loại";
		ListView_InsertColumn(this->hListView_, 1, &lvCol);

		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.pszText = L"Kích thước";
		lvCol.cx = 130;
		ListView_InsertColumn(this->hListView_, 2, &lvCol);

		lvCol.pszText = L"Mô tả";
		lvCol.cx = 130;
		ListView_InsertColumn(this->hListView_, 3, &lvCol);
	}

	void ListView::InitDiskCol()
	{
		LVCOLUMN lvCol;

		lvCol.mask = LVCF_TEXT | LVCF_FMT;

		lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
		//lvCol.cx = 100;
		lvCol.pszText = L"Loại";
		ListView_SetColumn(this->hListView_, 1, &lvCol);

		lvCol.fmt = LVCFMT_RIGHT | LVCF_WIDTH;
		//lvCol.cx = 80;
		lvCol.pszText = L"Tổng dung lượng";
		ListView_SetColumn(this->hListView_, 2, &lvCol);

		//lvCol.cx = 80;
		lvCol.pszText = L"Dung lượng trống";
		ListView_SetColumn(this->hListView_, 3, &lvCol);
	}

	void ListView::InitFolderCol()
	{
		LVCOLUMN lvCol;

		lvCol.mask = LVCF_WIDTH;
		lvCol.cx = 180;
		ListView_SetColumn(this->hListView_, 0, &lvCol);

		lvCol.mask = LVCF_TEXT | LVCF_FMT;
		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.cx = 130;
		lvCol.pszText = L"Kích thước";
		ListView_SetColumn(this->hListView_, 1, &lvCol);


		lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = 130;
		lvCol.pszText = L"Loại tập tin";
		ListView_SetColumn(this->hListView_, 2, &lvCol);

		lvCol.cx = 180;
		lvCol.pszText = L"Ngày chỉnh sửa";
		ListView_SetColumn(this->hListView_, 3, &lvCol);
	}

	void ListView::InitIconList()
	{
		this->hIml = new HIMAGELIST;

		*hIml = ImageList_Create(16, 16, ILC_COLOR24 | ILC_MASK, 5, 0);

		HICON hIcon;
		hIcon = LoadIcon(this->hInst_, MAKEINTRESOURCE(IDI_THICPC_SMALL));
		ImageList_AddIcon(*hIml, hIcon);

		hIcon = LoadIcon(this->hInst_, MAKEINTRESOURCE(IDI_DISK_SMALL));
		ImageList_AddIcon(*hIml, hIcon);

		hIcon = LoadIcon(this->hInst_, MAKEINTRESOURCE(IDI_FOLDER_SMALL));
		ImageList_AddIcon(*hIml, hIcon);

		hIcon = LoadIcon(this->hInst_, MAKEINTRESOURCE(IDI_USB_SMALL));
		ImageList_AddIcon(*hIml, hIcon);

		hIcon = LoadIcon(this->hInst_, MAKEINTRESOURCE(IDI_CD_SMALL));
		ImageList_AddIcon(*hIml, hIcon);

		hIcon = LoadIcon(this->hInst_, MAKEINTRESOURCE(IDI_UNKNOW_SMALL));
		ImageList_AddIcon(*hIml, hIcon);

		ListView_SetImageList(this->hListView_, *hIml, LVSIL_SMALL);
	}

	LPWSTR ListView::GetTypeInfo_(LPCWSTR path)
	{
		SHFILEINFO sfi = { 0 };
		UINT flag = SHGFI_TYPENAME;

		HRESULT hr = SHGetFileInfo(path, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), flag);
		if (SUCCEEDED(hr)) {
			TCHAR* result = new TCHAR[wcslen(sfi.szTypeName) + 1];
			lstPointer_.AddPointer(result);
			StrCpy(result, sfi.szTypeName);
			return result;
		}

		return NULL;
	}

	int	ListView::GetImageListIconIndex_(LPCWSTR path)
	{
		HICON hIcon;
		SHFILEINFO sfi = { 0 };
		UINT flag = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON;
		HRESULT hr = SHGetFileInfo(path, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), flag);
		if (SUCCEEDED(hr)) {
			hIcon = sfi.hIcon;
		}
		else
		{
			return IDI_UNKNOW;
		}
		return ImageList_AddIcon(ListView_GetImageList(this->hListView_, LVSIL_SMALL), hIcon);
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	LPCWSTR ListView::GetPath(int iItem)
	{
		LVITEM lv;
		lv.mask = LVIF_PARAM;
		lv.iItem = iItem;
		lv.iSubItem = 0;
		ListView_GetItem(this->hListView_, &lv);

		return (LPCWSTR)lv.lParam;
	}

	LPCWSTR ListView::GetCurSelPath()
	{
		return GetPath(ListView_GetSelectionMark(this->hListView_));
	}

	LPCWSTR ListView::GetParentPath(LPCWSTR path)
	{
		if (path == NULL) return NULL;
		if (wcslen(path) == 3) //Cha đang là ThisPC
			return L"This PC";
		else
		{
			TCHAR *parent;
			int nBackSlachPos = (StrRStrI(path, NULL, L"\\") - path);
			parent = new TCHAR[nBackSlachPos + 2];
			lstPointer_.AddPointer(parent);
			StrNCpy(parent, path, nBackSlachPos + 1);
			if (wcslen(parent) == 2)
			{
				StrCat(parent, L"\\");
			}
			return parent;
		}
	}

	LPCWSTR ListView::GetCurParentPath()
	{
		LPCWSTR path = GetPath(0);
		if (path == NULL)
			return NULL;

		if (wcslen(path) == 3) //Cha đang là ThisPC
			return L"This PC";
		else
		{
			TCHAR *parent;
			int nBackSlachPos = (StrRStrI(path, NULL, _T("\\")) - path);
			parent = new TCHAR[nBackSlachPos + 2];
			lstPointer_.AddPointer(parent);
			StrNCpy(parent, path, nBackSlachPos + 1);
			return parent;
		}
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	void ListView::ClearAll()
	{
		ListView_DeleteAllItems(this->hListView_);
	}

	void ListView::LoadRoot()
	{
		LoadRoot(NULL);
	}

	void ListView::LoadRoot(ListDisk *drive)
	{
		this->InitDiskCol();
		this->ClearAll();
		LV_ITEM lv;

		if (drive != NULL) this->lstDrive_ = drive;
		if (this->lstDrive_ == NULL) return;

		for (int i = 0; i < this->lstDrive_->GetAmount(); i++)
		{
			// Load cột đầu tiên - thông tin chính tên icon v.v
			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = i;
			lv.iImage = this->lstDrive_->GetIconID(i);

			lv.iSubItem = 0;
			lv.pszText = this->lstDrive_->GetFullDiskName(i);
			lv.lParam = (LPARAM)this->lstDrive_->GetDiskPath(i);
			ListView_InsertItem(this->hListView_, &lv);

			// Load các cột còn lại lần lượt là (Type, Size, Free Space)
			lv.mask = LVIF_TEXT;

			// Load cột đầu tiên là Type
			lv.iSubItem = 1;
			lv.pszText = this->lstDrive_->GetType(i);
			ListView_SetItem(this->hListView_, &lv);

			// Load cột tiếp theo là Size
			lv.iSubItem = 2;

			if (this->lstDrive_->GetIconID(i) != IDI_CD)
				lv.pszText = this->lstDrive_->GetSizeStr(i);
			else
				lv.pszText = NULL;

			ListView_SetItem(this->hListView_, &lv);

			// Load cột cuối cùng là Free Space
			lv.iSubItem = 3;

			if (this->lstDrive_->GetIconID(i) != IDI_CD)
				lv.pszText = this->lstDrive_->GetFreeSizeStr(i);
			else
				lv.pszText = NULL;

			ListView_SetItem(this->hListView_, &lv);
		}
		LONG dNotView = ~(LVS_ICON | LVS_SMALLICON | LVS_LIST | LVS_REPORT);

		SetWindowLong(this->hListView_, GWL_STYLE, GetWindowLong(this->hListView_, GWL_STYLE) & dNotView | LVS_REPORT);
	}

	void ListView::LoadChild(LPCWSTR path)
	{
		if (path == NULL) return;
		else
		{
			std::wstring temp = path;
			if (temp == L"This PC") this->LoadRoot();
			else this->LoadFileAndFolder(path);
		}
	}

	void ListView::LoadCurSel()
	{
		LPCWSTR path = GetCurSelPath();

		std::wstring temp = path;
		if (temp == L"This PC")
		{
			this->LoadRoot();
			return;
		}

		WIN32_FIND_DATA fd;
		GetFileAttributesEx(path, GetFileExInfoStandard, &fd);

		//Nếu là thư mục
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			ListView_DeleteAllItems(this->hListView_);
			LoadFileAndFolder(path);
		}
		else //Nếu là tập tin thì chạy nó
		{
			ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
		}
	}

	void ListView::LoadFileAndFolder(LPCWSTR path)
	{
		this->InitFolderCol();
		this->ClearAll();

		std::wstring pathHandle;
		pathHandle = path;

		if (pathHandle.length() == 3) // Nếu quét các ổ đĩa
		{
			pathHandle += L"*";
		}
		else
		{
			pathHandle += L"\\*";
		}

		//Bắt đầu tìm các file và folder trong thư mục
		WIN32_FIND_DATA fd;
		HANDLE hFile;
		BOOL bFound = TRUE;
		LV_ITEM lv;

		TCHAR * folderPath = NULL;
		int nItemCount = 0;

		//Chạy lần thứ nhất lấy các thư mục
		hFile = FindFirstFileW(const_cast<LPWSTR>(pathHandle.c_str()), &fd);
		bFound = TRUE;

		if (hFile == INVALID_HANDLE_VALUE)
			bFound = FALSE;

		if (bFound)
		{
			folderPath = new TCHAR[wcslen(path) + 1];
			lstPointer_.AddPointer(folderPath);
			StrCpy(folderPath, this->GetParentPath(path));

			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = nItemCount;
			lv.iSubItem = 0;
			lv.pszText = L"..";
			lv.lParam = (LPARAM)folderPath;
			lv.iImage = IDI_FOLDER;
			ListView_InsertItem(this->hListView_, &lv);

			//Cột thứ ba là cột Type
			ListView_SetItemText(this->hListView_, nItemCount, 2, L"Back");
			++nItemCount;
		}

		while (bFound)
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN) &&
				(StrCmp(fd.cFileName, L".") != 0) && (StrCmp(fd.cFileName, L"..") != 0))
			{
				folderPath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
				lstPointer_.AddPointer(folderPath);
				StrCpy(folderPath, path);

				if (wcslen(path) != 3)
					StrCat(folderPath, L"\\");

				StrCat(folderPath, fd.cFileName);

				lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
				lv.iItem = nItemCount;
				lv.iSubItem = 0;
				lv.pszText = fd.cFileName;
				lv.lParam = (LPARAM)folderPath;
				lv.iImage = IDI_FOLDER;
				ListView_InsertItem(this->hListView_, &lv);

				//Bỏ qua cột thứ hai là Size không hiển thị dung lượng cho thư mục

				//Cột thứ ba là cột Type
				ListView_SetItemText(this->hListView_, nItemCount, 2, L"Thư mục");

				//Cột thứ tư là cột Date modified
				TCHAR* date = Convert::ConvertDate(fd.ftLastWriteTime);
				lstPointer_.AddPointer(date);
				ListView_SetItemText(this->hListView_, nItemCount, 3, date);
				++nItemCount;
			}

			bFound = FindNextFileW(hFile, &fd);
		}

		DWORD folderCount = nItemCount;
		/*************************************************************************************/
		//Chạy lần thứ hai để lấy các tập tin
		TCHAR *filePath = NULL;
		DWORD fileSizeCount = 0;
		DWORD fileCount = 0;

		hFile = FindFirstFileW(const_cast<LPWSTR>(pathHandle.c_str()), &fd);
		bFound = TRUE;

		if (hFile == INVALID_HANDLE_VALUE)
			bFound = FALSE;

		while (bFound)
		{
			if (((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) &&
				((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM) &&
				((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN))
			{
				filePath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
				lstPointer_.AddPointer(filePath);

				StrCpy(filePath, path);

				if (wcslen(path) != 3)
					StrCat(filePath, L"\\");

				StrCat(filePath, fd.cFileName);

				//Cột thứ nhất là tên hiển thị của tập tin
				lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
				lv.iItem = nItemCount;
				lv.iSubItem = 0;
				lv.pszText = fd.cFileName;
				lv.lParam = (LPARAM)filePath;
				lv.iImage = GetImageListIconIndex_(filePath);
				ListView_InsertItem(this->hListView_, &lv);

				TCHAR* size = new TCHAR[20];
				lstPointer_.AddPointer(size);

				StrCpy(size, const_cast<LPWSTR>(Convert::ConvertVol(fd.nFileSizeLow).c_str()));
				//Cột thứ hai là Size
				ListView_SetItemText(this->hListView_, nItemCount, 1, size);
				fileSizeCount += fd.nFileSizeLow;

				//Cột thứ ba là Type
				ListView_SetItemText(this->hListView_, nItemCount, 2, this->GetTypeInfo_(filePath));

				//Cột thứ tư là Date Modified	
				TCHAR* date = Convert::ConvertDate(fd.ftLastWriteTime);
				lstPointer_.AddPointer(date);
				ListView_SetItemText(this->hListView_, nItemCount, 3, date);

				++nItemCount;
				++fileCount;
			}

			bFound = FindNextFileW(hFile, &fd);
		}
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	HWND ListView::GetThisHandle()
	{
		return this->hListView_;
	}

	int	ListView::GetThisID()
	{
		return this->ID_;
	}

	void ListView::SetFocus_()
	{
		SetFocus(this->hListView_);
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	void ListView::Resize()
	{
		RECT treeRC;
		GetWindowRect(GetDlgItem(this->hParent_, IDC_TREEVIEW), &treeRC);

		RECT windowRC;
		GetWindowRect(this->hParent_, &windowRC);

		MoveWindow(this->hListView_, treeRC.right - treeRC.left, 0, windowRC.right - treeRC.right - 8, windowRC.bottom - windowRC.top - 60, TRUE);
		ListView_Arrange(this->hListView_, LVA_ALIGNTOP);
	}
	/*-----------------------------------------------------------------------------------------------------*/
}