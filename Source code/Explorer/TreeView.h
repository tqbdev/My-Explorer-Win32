#pragma once

#include <windows.h>
#include <tchar.h>

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#include "Convert.h"
#include "ListPointer.h"

namespace MyExplorer
{
	DWORD WINAPI CollectGarbage(LPVOID lpParameter);
	class TreeView
	{
	private:
		HINSTANCE		hInst_;
		HWND			hParent_;
		HWND			hTreeView_;
		long			ID_;

		ListPointer*	lstPointer_;			// Giữ các con trỏ được cấp phát vùng nhớ - để giải phóng
		HIMAGELIST*		hIml;					// Giữ lại icon list để giải phóng vùng nhớ

		void			GetIconList();
	public:
		TreeView();
		~TreeView();

		void			Create(HWND hParent, long ID, HINSTANCE hParentInst,
							int Width, int Height, int x = CW_USEDEFAULT, int y = 0,
							long lExtStyle = 0, long lStyle = TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS);

		HWND			GetThisHandle();
		int				GetThisID();
		HTREEITEM		GetRoot();
		void			SetFocus_();

		PIDLIST_ABSOLUTE			GetPIDL(HTREEITEM hItem);
		PIDLIST_ABSOLUTE			GetCurPIDL();

		HTREEITEM		GetCurSel();
		HTREEITEM		GetChild(HTREEITEM hItem);
		
		void			LoadRoot();
		void			LoadChild(HTREEITEM &hParent, PIDLIST_ABSOLUTE pidl);
		void			Expanding(HTREEITEM hPrev, HTREEITEM hCurSel);
		void			Expand(HTREEITEM hItem);
		void			Resize();
	};
}