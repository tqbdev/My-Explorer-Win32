#pragma once
#include "TreeView.h"

namespace MyExplorer
{
	class ListView
	{
	private:
	private:
		HINSTANCE	hInst_;
		HWND		hParent_;
		HWND		hListView_;
		long		ID_;
		ListDisk*	lstDrive_;				// Giữ ListDisk đã lấy vào lúc LoadRoot

		ListPointer	lstPointer_;			// Giữ các con trỏ được cấp phát vùng nhớ - để giải phóng
		HIMAGELIST*	hIml;					// Giữ lại icon list để giải phóng vùng nhớ

		int			countIconInList;		// Đếm số lượng Icon hiện có trong IconList

		void		InitIconList();			// Khởi tạo các icon cơ bản ban đầu, icon của các tập tin sẽ load sau
		void		InitColumn();			// Khởi tạo các cột cơ bản
		void		InitDiskCol();			// Khởi tạo các cột để hiển thị thông tin cho ổ đĩa
		void		InitFolderCol();		// Khởi tạo các cột để hiện thị thông tin cho thư mục

		void		LoadFileAndFolder(LPCWSTR path);
		void		LoadRoot();

		LPCWSTR		GetParentPath(LPCWSTR path);

		int			GetImageListIconIndex_(LPCWSTR path);
		LPWSTR		GetTypeInfo_(LPCWSTR path);
	public:
		ListView();
		~ListView();

		void		Create(HWND parentWnd, long ID, HINSTANCE hParentInst, 
							int Width, int Height, int x = CW_USEDEFAULT, int y = 0,
							long lExtStyle = WS_EX_CLIENTEDGE, long lStyle = LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS);

		HWND		GetThisHandle();
		int			GetThisID();
		void		SetFocus_();

		LPCWSTR		GetPath(int iItem);
		LPCWSTR		GetCurSelPath();
		LPCWSTR		GetCurParentPath();

		void		ClearAll();		// Clear thông tin hiện tại

		void		LoadRoot(ListDisk *drive); // Load ThisPC
		void		LoadChild(LPCWSTR path); // Xem nên load cái nào Load lại Root hoặc File and Folder
		void		LoadCurSel();
		void		Resize();
	};
}