#pragma once
#include "TreeView.h"

namespace MyExplorer
{
	class ListView
	{
	private:
	private:
		HINSTANCE		hInst_;
		HWND			hParent_;
		HWND			hListView_;
		long			ID_;

		ListPointer*	lstPointer_;			// Giữ các con trỏ được cấp phát vùng nhớ - để giải phóng
		ListPointer*	lstCircle_;				// Giữ các pointer hiển thị listview ở một thời điểm khi chuyển qua view khác sẽ bị giải phóng
		HIMAGELIST*		hIml;					// Giữ lại icon list để giải phóng vùng nhớ

		void			InitIconList();			// Khởi tạo các icon hệ thống
		void			InitColumn();			// Khởi tạo các cột cơ bản
		void			InitDiskCol();			// Khởi tạo các cột để hiển thị thông tin cho ổ đĩa
		void			InitFolderCol();		// Khởi tạo các cột để hiện thị thông tin cho thư mục

		void			LoadFileAndFolder(PIDLIST_ABSOLUTE pidl);
	public:
		ListView();
		~ListView();

		void			Create(HWND parentWnd, long ID, HINSTANCE hParentInst, 
							int Width, int Height, int x = CW_USEDEFAULT, int y = 0,
							long lExtStyle = WS_EX_CLIENTEDGE, long lStyle = LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS);

		HWND			GetThisHandle();
		int				GetThisID();
		void			SetFocus_();

		PIDLIST_ABSOLUTE			GetPIDL(int iItem);
		PIDLIST_ABSOLUTE			GetCurSelPIDL();

		void			ClearAll();					// Clear thông tin hiện tại

		void			DisplayInfoCurSel();

		void			LoadRoot();	// Load ThisPC
		void			LoadChild(PIDLIST_ABSOLUTE pidl);	// Xem nên load cái nào Load lại Root hoặc File and Folder
		void			LoadCurSel();
		void			Resize();
	};
}