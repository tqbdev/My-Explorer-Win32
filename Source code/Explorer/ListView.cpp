#include "stdafx.h"
#include "ListView.h"
#include "resource.h"

#include <Shellapi.h>
#pragma comment(lib, "Shell32.lib")

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

		this->hIml = NULL;
		this->lstPointer_ = new ListPointer();
		this->lstCircle_ = new ListPointer();
	}

	ListView::~ListView()
	{
		if (this->lstPointer_) CreateThread(NULL, 0, CollectGarbage, this->lstPointer_, 0, 0);
		if (this->lstCircle_) CreateThread(NULL, 0, CollectGarbage, this->lstCircle_, 0, 0);
		if (this->hIml) delete this->hIml;
		if (this->hListView_) DestroyWindow(this->hListView_);
	}

	void ListView::Create(HWND parentWnd, long ID, HINSTANCE hParentInst,
		int Width, int Height, int x, int y,
		long lExtStyle, long lStyle)
	{
		INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
		InitCommonControlsEx(&icex);

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
		lvCol.pszText = L"Name";
		ListView_InsertColumn(this->hListView_, 0, &lvCol);

		lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
		lvCol.cx = 100;
		lvCol.pszText = L"Type";
		ListView_InsertColumn(this->hListView_, 1, &lvCol);

		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.pszText = L"Size";
		lvCol.cx = 130;
		ListView_InsertColumn(this->hListView_, 2, &lvCol);

		lvCol.pszText = L"Description";
		lvCol.cx = 130;
		ListView_InsertColumn(this->hListView_, 3, &lvCol);
	}

	void ListView::InitDiskCol()
	{
		LVCOLUMN lvCol;

		lvCol.mask = LVCF_TEXT | LVCF_FMT;

		lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
		//lvCol.cx = 100;
		lvCol.pszText = L"Type";
		ListView_SetColumn(this->hListView_, 1, &lvCol);

		lvCol.fmt = LVCFMT_RIGHT | LVCF_WIDTH;
		//lvCol.cx = 80;
		lvCol.pszText = L"Total size";
		ListView_SetColumn(this->hListView_, 2, &lvCol);

		//lvCol.cx = 80;
		lvCol.pszText = L"Free space";
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
		lvCol.pszText = L"Size";
		ListView_SetColumn(this->hListView_, 1, &lvCol);


		lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = 130;
		lvCol.pszText = L"Type";
		ListView_SetColumn(this->hListView_, 2, &lvCol);

		lvCol.cx = 180;
		lvCol.pszText = L"Date modified";
		ListView_SetColumn(this->hListView_, 3, &lvCol);
	}

	void ListView::InitIconList()
	{
		this->hIml = new HIMAGELIST;

		SHGetImageList(SHIL_SMALL, IID_IImageList, (void**)hIml);

		ListView_SetImageList(this->hListView_, *hIml, LVSIL_SMALL);
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	PIDLIST_ABSOLUTE ListView::GetPIDL(int iItem)
	{
		LVITEM lv;
		lv.mask = LVIF_PARAM;
		lv.iItem = iItem;
		lv.iSubItem = 0;
		ListView_GetItem(this->hListView_, &lv);

		return (PIDLIST_ABSOLUTE)lv.lParam;
	}

	PIDLIST_ABSOLUTE ListView::GetCurSelPIDL()
	{
		return GetPIDL(ListView_GetSelectionMark(this->hListView_));
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	void ListView::DisplayInfoCurSel()
	{
		int nCurSelIndex = ListView_GetNextItem(GetDlgItem(this->hParent_, IDC_LISTVIEW), -1, LVNI_FOCUSED);
		TCHAR *text = new TCHAR[256];
		this->lstPointer_->AddPointer(text);
		LVITEM lv;
		lv.mask = LVIF_TEXT;
		lv.iItem = nCurSelIndex;
		lv.iSubItem = 0;
		lv.pszText = text;
		lv.cchTextMax = 256;

		ListView_GetItem(this->hListView_, &lv);
		SendMessage(GetDlgItem(this->hParent_, IDC_STATUSBAR), SB_SETTEXT, 2, (LPARAM)text);

		lv.iSubItem = 2;
		ListView_GetItem(this->hListView_, &lv);

		if (!StrCmpI(lv.pszText, L"Folder"))
			SendMessage(GetDlgItem(this->hParent_, IDC_STATUSBAR), SB_SETTEXT, 1, NULL);
		else
		{
			lv.iSubItem = 1;
			ListView_GetItem(this->hListView_, &lv);
			SendMessage(GetDlgItem(this->hParent_, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)text);
		}
	}

	void ListView::ClearAll()
	{
		ListView_DeleteAllItems(this->hListView_);
	}

	void ListView::LoadRoot()
	{
		this->InitDiskCol();
		this->ClearAll();
		LV_ITEM lv;

		LPITEMIDLIST pidlThisPC = NULL;
		HRESULT hr;
		hr = SHGetFolderLocation(NULL, CSIDL_DRIVES, NULL, NULL, &pidlThisPC);

		LPSHELLFOLDER psfThisPC;
		LPENUMIDLIST penumIDL = NULL;
		hr = SHBindToObject(NULL, pidlThisPC, NULL, IID_IShellFolder, (void**)&psfThisPC);
		psfThisPC->EnumObjects(NULL, SHCONTF_FOLDERS, &penumIDL);

		PIDLIST_RELATIVE pidNext = NULL;
		PIDLIST_ABSOLUTE pidNextFull = NULL;

		int countItem = 0;
		do
		{
			hr = penumIDL->Next(1, &pidNext, NULL);
			if (hr == S_OK)
			{
				pidNextFull = ILCombine(pidlThisPC, pidNext);

				SHFILEINFO sfi = { 0 };
				SHGetFileInfo((LPCTSTR)(LPCITEMIDLIST)pidNextFull, 0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_TYPENAME | SHGFI_ATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

				// Load cột đầu tiên - thông tin chính tên icon v.v
				lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
				lv.iItem = countItem;
				lv.iImage = sfi.iIcon;

				lv.iSubItem = 0;
				lv.pszText = sfi.szDisplayName;
				lv.lParam = (LPARAM)pidNextFull;
				ListView_InsertItem(this->hListView_, &lv);

				// Load các cột còn lại lần lượt là (Type, Size, Free Space)
				lv.mask = LVIF_TEXT;

				// Load cột đầu tiên là Type
				lv.iSubItem = 1;
				lv.pszText = sfi.szTypeName;
				ListView_SetItem(this->hListView_, &lv);

				std::wstring temp = sfi.szTypeName;
				if (temp == L"Local Disk" || temp == L"Network Drive")
				{
					WCHAR path[20];
					SHGetPathFromIDList(pidNextFull, path);

					long long totalSize = 0, freeSize = 0;
					SHGetDiskFreeSpaceEx(path, NULL, (PULARGE_INTEGER)&totalSize, (PULARGE_INTEGER)&freeSize);

					temp = Convert::ConvertVol(totalSize);

					// Load cột tiếp theo là totalSize
					lv.iSubItem = 2;
					lv.pszText = const_cast<LPWSTR>(temp.c_str());

					ListView_SetItem(this->hListView_, &lv);

					temp = Convert::ConvertVol(freeSize);
					// Load cột cuối cùng là Free Space
					lv.iSubItem = 3;
					lv.pszText = const_cast<LPWSTR>(temp.c_str());

					ListView_SetItem(this->hListView_, &lv);
				}

				countItem++;
			}
		} while (hr == S_OK);

		LONG dNotView = ~(LVS_ICON | LVS_SMALLICON | LVS_LIST | LVS_REPORT);

		SetWindowLong(this->hListView_, GWL_STYLE, GetWindowLong(this->hListView_, GWL_STYLE) & dNotView | LVS_REPORT);

		TCHAR *buffer = new TCHAR[20];
		this->lstPointer_->AddPointer(buffer);
		wsprintf(buffer, L"%d item%s", countItem, countItem > 1 ? L"s" : L"");
		SendMessage(GetDlgItem(this->hParent_, IDC_STATUSBAR), SB_SETTEXT, 0, (LPARAM)buffer);
	}

	void ListView::LoadChild(PIDLIST_ABSOLUTE pidl)
	{
		SHFILEINFO sfi = { 0 };
		SHGetFileInfo((LPCTSTR)(LPCITEMIDLIST)pidl, 0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_DISPLAYNAME);
		if (pidl == NULL) return;
		else
		{
			std::wstring temp = sfi.szDisplayName;
			if (temp == L"This PC") this->LoadRoot();
			else this->LoadFileAndFolder(pidl);
		}
	}

	void ListView::LoadCurSel()
	{
		PIDLIST_ABSOLUTE  pidl = GetCurSelPIDL();

		WIN32_FIND_DATA fd;
		WCHAR path[10240];
		SHGetPathFromIDList(pidl, path);
		GetFileAttributesEx(path, GetFileExInfoStandard, &fd);

		//Nếu là thư mục
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			ListView_DeleteAllItems(this->hListView_);
			LoadFileAndFolder(pidl);
		}
		else //Nếu là tập tin thì chạy nó
		{
			ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
		}
	}

	void ListView::LoadFileAndFolder(PIDLIST_ABSOLUTE pidl)
	{
		this->InitFolderCol();
		this->ClearAll();

		HRESULT hr = NULL;

		LPSHELLFOLDER lpfParent;
		LPENUMIDLIST penumIDL = NULL;
		hr = SHBindToObject(NULL, pidl, NULL, IID_IShellFolder, (void**)&lpfParent);
		lpfParent->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &penumIDL);

		PIDLIST_RELATIVE pidNext = NULL;
		PIDLIST_ABSOLUTE pidNextFull = NULL;

		LV_ITEM lv;
		WIN32_FIND_DATA fd;
		int itemCount = 0;
		do
		{
			hr = penumIDL->Next(1, &pidNext, NULL);
			if (hr == S_OK)
			{
				pidNextFull = ILCombine(pidl, pidNext);

				SHFILEINFO sfi = { 0 };
				sfi.iIcon = 0;
				SHGetFileInfo((LPCTSTR)(LPCITEMIDLIST)pidNextFull, 0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_TYPENAME | SHGFI_DISPLAYNAME | SHGFI_ATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

				SHGetDataFromIDList(lpfParent, pidNext, SHGDFIL_FINDDATA, &fd, sizeof(WIN32_FIND_DATA));

				lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
				lv.iItem = itemCount;
				lv.iSubItem = 0;
				lv.pszText = sfi.szDisplayName;
				lv.lParam = (LPARAM)pidNextFull;
				lv.iImage = sfi.iIcon;
				ListView_InsertItem(this->hListView_, &lv);

				if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY &&
					(fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM &&
					(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN)
				{
					TCHAR* size = new TCHAR[20];
					lstCircle_->AddPointer(size);

					StrCpy(size, const_cast<LPWSTR>(Convert::ConvertVol(fd.nFileSizeLow).c_str()));
					//Cột thứ hai là Size
					ListView_SetItemText(this->hListView_, itemCount, 1, size);
				}

				ListView_SetItemText(this->hListView_, itemCount, 2, sfi.szTypeName);

				TCHAR* date = Convert::ConvertDate(fd.ftLastWriteTime);
				lstCircle_->AddPointer(date);
				ListView_SetItemText(this->hListView_, itemCount, 3, date);

				itemCount++;
			}
		} while (hr == S_OK);

		TCHAR *buffer = new TCHAR[20];
		this->lstPointer_->AddPointer(buffer);
		wsprintf(buffer, L"%d item%s", itemCount, itemCount > 1 ? L"s" : L"");
		SendMessage(GetDlgItem(this->hParent_, IDC_STATUSBAR), SB_SETTEXT, 0, (LPARAM)buffer);
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

		RECT listRC;
		GetWindowRect(this->hListView_, &listRC);

		RECT windowRC;
		GetWindowRect(this->hParent_, &windowRC);

		MoveWindow(this->hListView_, treeRC.right - treeRC.left + SPLITTER_BAR_WIDTH - 5, 0, windowRC.right - treeRC.right - 2 - SPLITTER_BAR_WIDTH, windowRC.bottom - windowRC.top - 82, TRUE);
		ListView_Arrange(this->hListView_, LVA_ALIGNTOP);
	}
	/*-----------------------------------------------------------------------------------------------------*/
}