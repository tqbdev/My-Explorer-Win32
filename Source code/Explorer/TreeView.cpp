#include "stdafx.h"
#include "TreeView.h"
#include "resource.h"

//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace MyExplorer
{
	DWORD WINAPI CollectGarbage(LPVOID lpParameter)
	{
		ListPointer* p = (ListPointer*)lpParameter;
		p->ClearAll();
		delete p;
		return 0;
	}
	/*-----------------------------------------------------------------------------------------------------*/
	TreeView::TreeView()
	{
		this->hInst_ = NULL;
		this->hParent_ = NULL;
		this->hTreeView_ = NULL;
		this->ID_ = -1;

		this->hIml = NULL;
		this->lstPointer_ = new ListPointer();
	}

	TreeView::~TreeView()
	{
		if (this->lstPointer_) if (this->lstPointer_) CreateThread(NULL, 0, CollectGarbage, this->lstPointer_, 0, 0);
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
		INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
		InitCommonControlsEx(&icex);

		this->hInst_ = hParentInst;
		this->hParent_ = parentWnd;
		this->ID_ = ID;

		this->hTreeView_ = CreateWindowEx(lExtStyle, WC_TREEVIEW, L"Tree View",
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_TABSTOP | lStyle,
			x, y, Width, Height,
			this->hParent_, (HMENU)this->ID_, this->hInst_, NULL);

		this->GetIconList();
	}

	void TreeView::GetIconList()
	{
		this->hIml = new HIMAGELIST;

		SHGetImageList(SHIL_SMALL, IID_IImageList, (void**)hIml);

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
	PIDLIST_ABSOLUTE TreeView::GetPIDL(HTREEITEM hItem)
	{
		TVITEMEX tv;
		tv.mask = TVIF_PARAM;
		tv.hItem = hItem;
		TreeView_GetItem(this->hTreeView_, &tv);
		return (PIDLIST_ABSOLUTE)tv.lParam;
	}

	PIDLIST_ABSOLUTE	TreeView::GetCurPIDL()
	{
		return GetPIDL(GetCurSel());
		return NULL;
	}

	HTREEITEM TreeView::GetCurSel()
	{
		return TreeView_GetNextItem(this->hTreeView_, NULL, TVGN_CARET);
	}

	HTREEITEM TreeView::GetChild(HTREEITEM hItem)
	{
		return TreeView_GetChild(this->hTreeView_, hItem);
	}
	/*-----------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------*/
	void TreeView::LoadRoot()
	{
		TV_INSERTSTRUCT tvInsert;
		tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;

		LPSHELLFOLDER psfDesktop = NULL;
		SHGetDesktopFolder(&psfDesktop);

		LPENUMIDLIST penumIDL = NULL;
		psfDesktop->EnumObjects(NULL, SHCONTF_FOLDERS, &penumIDL);

		PIDLIST_ABSOLUTE pidNext = NULL;
		HRESULT hr = NULL;
		
		hr = penumIDL->Next(1, &pidNext, NULL);
		HTREEITEM hThisPC = NULL;
		// Load ThisPC
		if (SUCCEEDED(hr))
		{
			SHFILEINFO sfi = { 0 };
			SHGetFileInfo((LPCTSTR)(LPCITEMIDLIST)pidNext, 0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_ATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

			tvInsert.hParent = NULL;
			tvInsert.hInsertAfter = TVI_ROOT;
			tvInsert.item.iImage = sfi.iIcon;
			tvInsert.item.iSelectedImage = sfi.iIcon;
			tvInsert.item.pszText = sfi.szDisplayName;
			tvInsert.item.cChildren = 0;

			if ((sfi.dwAttributes & SFGAO_HASSUBFOLDER) == SFGAO_HASSUBFOLDER)
			{
				tvInsert.item.cChildren = 1;
			}

			tvInsert.item.lParam = (LPARAM)pidNext;

			hThisPC = TreeView_InsertItem(this->hTreeView_, &tvInsert);
		}

		do
		{
			hr = penumIDL->Next(1, &pidNext, NULL);
			if (hr == S_OK)
			{	
				SHFILEINFO sfi = { 0 };
				SHGetFileInfo((LPCTSTR)(LPCITEMIDLIST)pidNext, 0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_ATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

				tvInsert.hParent = NULL;
				tvInsert.hInsertAfter = TVI_ROOT;
				tvInsert.item.iImage = sfi.iIcon;
				tvInsert.item.iSelectedImage = sfi.iIcon;
				tvInsert.item.pszText = sfi.szDisplayName;
				tvInsert.item.cChildren = 0;

				if ((sfi.dwAttributes & SFGAO_HASSUBFOLDER) == SFGAO_HASSUBFOLDER)
				{
					tvInsert.item.cChildren = 1;
				}
				
				tvInsert.item.lParam = (LPARAM)pidNext;
				TreeView_InsertItem(this->hTreeView_, &tvInsert);
			}
		} while (hr == S_OK);

		psfDesktop->Release();
		
		TreeView_SelectItem(this->hTreeView_, hThisPC);
	}

	void TreeView::LoadChild(HTREEITEM &hParent, PIDLIST_ABSOLUTE pidl)
	{
		TV_INSERTSTRUCT tvInsert;
		tvInsert.hParent = hParent;
		tvInsert.hInsertAfter = TVI_LAST;
		tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;
		tvInsert.item.cChildren = 0;
		
		HRESULT hr = NULL;

		LPSHELLFOLDER lpfParent;
		LPENUMIDLIST penumIDL = NULL;
		hr = SHBindToObject(NULL, pidl, NULL, IID_IShellFolder, (void**)&lpfParent);
		lpfParent->EnumObjects(NULL, SHCONTF_FOLDERS, &penumIDL);

		PIDLIST_RELATIVE pidNext = NULL;		
		PIDLIST_ABSOLUTE pidNextFull = NULL;

		do
		{
			hr = penumIDL->Next(1, &pidNext, NULL);
			if (hr == S_OK)
			{
				pidNextFull = ILCombine(pidl, pidNext);
				
				SHFILEINFO sfi = { 0 };
				sfi.iIcon = 0;
				SHGetFileInfo((LPCTSTR)(LPCITEMIDLIST)pidNextFull, 0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_ATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
				
				tvInsert.item.iImage = sfi.iIcon;
				tvInsert.item.iSelectedImage = sfi.iIcon;
				tvInsert.item.pszText = sfi.szDisplayName;
				tvInsert.item.cChildren = 0;

				if ((sfi.dwAttributes & SFGAO_HASSUBFOLDER) == SFGAO_HASSUBFOLDER)
				{
					tvInsert.item.cChildren = 1;
				}

				tvInsert.item.lParam = (LPARAM)pidNextFull;

				TreeView_InsertItem(this->hTreeView_, &tvInsert);
			}
		} while (hr == S_OK);
	}

	void TreeView::Expanding(HTREEITEM hPrev, HTREEITEM hCurSel)
	{
		LoadChild(hCurSel, GetPIDL(hCurSel));
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

		MoveWindow(this->hTreeView_, 0, 0, treeRC.right - treeRC.left, windowRC.bottom - windowRC.top - 82, SWP_SHOWWINDOW);
	}
	/*-----------------------------------------------------------------------------------------------------*/
}