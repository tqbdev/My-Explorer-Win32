#include "stdafx.h"
#include "TreeView.h"
#include "resource.h"

//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace MyExplorer
{
	/*-----------------------------------------------------------------------------------------------------*/
	TreeView::TreeView()
	{
		this->hInst_ = NULL;
		this->hParent_ = NULL;
		this->hTreeView_ = NULL;
		this->ID_ = -1;

		this->hIml = NULL;
	}

	TreeView::~TreeView()
	{
		if (this->hIml) delete hIml;
		if (this->hTreeView_) DestroyWindow(this->hTreeView_);
	}

	HWND TreeView::GetThisHandle()
	{
		return this->hTreeView_;
	}

	int	TreeView::GetThisID()
	{
		return this->ID_;
	}

	void TreeView::Create(HWND parentWnd, long ID, HINSTANCE hParentInst,
		int Width, int Height, int x, int y,
		long lExtStyle, long lStyle)
	{
		InitCommonControls();

		this->hInst_ = hParentInst;
		this->hParent_ = parentWnd;
		this->ID_ = ID;

		this->hTreeView_ = CreateWindowEx(lExtStyle, WC_TREEVIEW, L"Tree View",
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_VSCROLL | WS_TABSTOP | lStyle,
			x, y, Width, Height,
			this->hParent_, (HMENU)this->ID_, this->hInst_, NULL);

		this->GetIconList();
	}

	void TreeView::GetIconList()
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

		TreeView_SetImageList(this->hTreeView_, *hIml, TVSIL_NORMAL);
	}

	HTREEITEM TreeView::GetRoot()
	{
		return TreeView_GetRoot(this->hTreeView_);
	}

	void TreeView::SetFocus_()
	{
		SetFocus(this->hTreeView_);
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	LPCWSTR TreeView::GetPath(HTREEITEM hItem)
	{
		TVITEMEX tv;
		tv.mask = TVIF_PARAM;
		tv.hItem = hItem;
		TreeView_GetItem(this->hTreeView_, &tv);
		return (LPCWSTR)tv.lParam;
	}

	LPCWSTR	TreeView::GetCurPath()
	{
		return GetPath(GetCurSel());
	}

	HTREEITEM TreeView::GetCurSel()
	{
		return TreeView_GetNextItem(this->hTreeView_, NULL, TVGN_CARET);
	}

	LPCWSTR	TreeView::GetCurSelText()
	{
		TVITEMEX tv;
		TCHAR *buffer = new TCHAR[256];
		lstPointer_.AddPointer(buffer);

		tv.mask = TVIF_TEXT;
		tv.hItem = GetCurSel();
		tv.pszText = buffer;
		tv.cchTextMax = 256;
		TreeView_GetItem(this->hTreeView_, &tv);
		return (LPCWSTR)tv.pszText;
	}

	HTREEITEM TreeView::GetChild(HTREEITEM hItem)
	{
		return TreeView_GetChild(this->hTreeView_, hItem);
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	void TreeView::LoadRoot(ListDisk *listDisk)
	{
		TV_INSERTSTRUCT tvInsert;
		tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

		// This PC
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = TVI_ROOT;
		tvInsert.item.iImage = IDI_THISPC;
		tvInsert.item.iSelectedImage = IDI_THISPC;
		tvInsert.item.pszText = L"This PC";
		tvInsert.item.lParam = (LPARAM)L"This PC";
		HTREEITEM hThisPC = TreeView_InsertItem(this->hTreeView_, &tvInsert);

		// Load các ổ đĩa đang có trong máy
		for (int i = 0; i < listDisk->GetAmount(); ++i)
		{
			tvInsert.hParent = hThisPC; //Them
			tvInsert.item.iImage = listDisk->GetIconID(i);
			tvInsert.item.iSelectedImage = listDisk->GetIconID(i);
			tvInsert.item.pszText = listDisk->GetFullDiskName(i);
			tvInsert.item.lParam = (LPARAM)listDisk->GetDiskPath(i);
			HTREEITEM hlistDisk = TreeView_InsertItem(this->hTreeView_, &tvInsert);

			//Preload
			if ((listDisk->GetIconID(i) == IDI_DISK) || (listDisk->GetIconID(i) == IDI_USB))
			{
				tvInsert.hParent = hlistDisk; //Them
				tvInsert.item.pszText = L"PreLoad"; //Them
				tvInsert.item.lParam = (LPARAM)L"PreLoad";
				TreeView_InsertItem(this->hTreeView_, &tvInsert);
			}
		}

		// Mặc định cho ThisPC expand và select luôn
		TreeView_Expand(this->hTreeView_, hThisPC, TVE_EXPAND);
		TreeView_SelectItem(this->hTreeView_, hThisPC);
	}

	void TreeView::PreLoad(HTREEITEM hItem)
	{
		std::wstring pathHandle = GetPath(hItem);

		if (pathHandle.length() == 3)
		{
			if (pathHandle == L"A:" || pathHandle == L"B:") // Đĩa mềm bỏ qua
				return;
		}
		else
		{
			pathHandle += L"\\";
		}

		pathHandle += L"*";

		WIN32_FIND_DATA fd;
		HANDLE hFile = FindFirstFileW(const_cast<LPWSTR>(pathHandle.c_str()), &fd);

		if (hFile == INVALID_HANDLE_VALUE)
			return;

		BOOL bFound = true;

		//Trong khi còn tìm thấy file hay thư mục
		while (bFound)
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (StrCmp(fd.cFileName, L".") != 0) && (StrCmp(fd.cFileName, L"..") != 0))
			{
				TV_INSERTSTRUCT tvInsert;
				tvInsert.hParent = hItem;
				tvInsert.hInsertAfter = TVI_LAST;
				tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM;
				tvInsert.item.pszText = NULL;
				tvInsert.item.lParam = (LPARAM)L"PreLoad";
				TreeView_InsertItem(this->hTreeView_, &tvInsert);
				bFound = FALSE;
			}
			else
				bFound = FindNextFileW(hFile, &fd);
		}
	}

	void TreeView::LoadChild(HTREEITEM &hParent, LPCWSTR path, BOOL bShowHiddenSystem)
	{
		std::wstring pathHandle;
		pathHandle = path;

		pathHandle += L"\\*";

		TV_INSERTSTRUCT tvInsert;
		tvInsert.hParent = hParent;
		tvInsert.hInsertAfter = TVI_LAST;
		tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
		tvInsert.item.iImage = IDI_FOLDER;
		tvInsert.item.iSelectedImage = IDI_FOLDER;

		WIN32_FIND_DATA fd;
		HANDLE hFile = FindFirstFileW(const_cast<LPWSTR>(pathHandle.c_str()), &fd);
		BOOL bFound = 1;

		if (hFile == INVALID_HANDLE_VALUE)
			bFound = FALSE;

		TCHAR* folderPath = NULL;
		while (bFound)
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& ((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN)
				&& ((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM)
				&& (StrCmp(fd.cFileName, L".") != 0) && (StrCmp(fd.cFileName, L"..") != 0))
			{
				tvInsert.item.pszText = fd.cFileName;
				folderPath = new TCHAR[pathHandle.length() + wcslen(fd.cFileName) + 2];
				lstPointer_.AddPointer(folderPath);

				StrCpy(folderPath, path);
				if (wcslen(path) != 3)
					StrCat(folderPath, L"\\");
				StrCat(folderPath, fd.cFileName);

				tvInsert.item.lParam = (LPARAM)folderPath;
				HTREEITEM hItem = TreeView_InsertItem(this->hTreeView_, &tvInsert);
				//Preload
				PreLoad(hItem);
			}

			bFound = FindNextFileW(hFile, &fd);
		}//while
	}

	void TreeView::PreloadExpanding(HTREEITEM hPrev, HTREEITEM hCurSel)
	{
		if (hCurSel == GetRoot()) //Nếu ThisPC chưa nạp thoát
			return;

		HTREEITEM hCurSelChild = TreeView_GetChild(this->hTreeView_, hCurSel);

		if (!StrCmp(GetPath(hCurSelChild), L"PreLoad"))
		{
			TreeView_DeleteItem(this->hTreeView_, hCurSelChild);
			LoadChild(hCurSel, GetPath(hCurSel));
		}
	}

	void TreeView::Expand(HTREEITEM hItem)
	{
		TreeView_Expand(this->hTreeView_, hItem, TVE_EXPAND);
	}

	void TreeView::Resize()
	{
		RECT treeRC;
		GetWindowRect(this->hTreeView_, &treeRC);

		RECT windowRC;
		GetWindowRect(this->hParent_, &windowRC);

		MoveWindow(this->hTreeView_, 0, 0, treeRC.right - treeRC.left, windowRC.bottom - windowRC.top - 60, SWP_SHOWWINDOW);
	}
	/*-----------------------------------------------------------------------------------------------------*/
}