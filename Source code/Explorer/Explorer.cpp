// Explorer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Explorer.h"
//#include <vld.h> //Debug kiểm tra leak bộ nhớ sử dụng: Visual Leak Detector for Visual C++ 2008 - 2015

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_EXPLORER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXPLORER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(hwndMain, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXPLORER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EXPLORER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	const int BUFFERSIZE = 260;
	WCHAR buffer[BUFFERSIZE];
	GetCurrentDirectory(BUFFERSIZE, buffer);

	std::wstring configPath = buffer;
	configPath += L"\\";
	/*int strlen = configPath.length();

	while (configPath[strlen - 1] != _T('\\'))
	{
		strlen--;
	}
	configPath = configPath.substr(0, strlen);*/
	configPath += CONFIG_FILE;

	GetPrivateProfileString(L"Screen Resolution", L"Width", L"-2147483648", buffer, BUFFERSIZE, const_cast<LPWSTR>(configPath.c_str()));
	int width = std::stoi(buffer);

	GetPrivateProfileString(L"Screen Resolution", L"Height", L"0", buffer, BUFFERSIZE, const_cast<LPWSTR>(configPath.c_str()));
	int height = std::stoi(buffer);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);

	hwndMain = hWnd;

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONUP, OnLButtonUp);
		HANDLE_MSG(hWnd, WM_CLOSE, OnClose);

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	RECT mainWin;
	GetWindowRect(hwnd, &mainWin);

	g_StatusBar = new MyExplorer::StatusBar;
	g_StatusBar->Create(hwnd, IDC_STATUSBAR, hInst);

	g_TreeView = new MyExplorer::TreeView;
	g_TreeView->Create(hwnd, IDC_TREEVIEW, hInst, (mainWin.right - mainWin.left) * 2 / 7, mainWin.bottom - mainWin.top - 10);
	g_TreeView->LoadRoot();
	g_TreeView->SetFocus_();

	g_ListView = new MyExplorer::ListView;
	g_ListView->Create(hwnd, IDC_LISTVIEW, hInst, 0, mainWin.bottom - mainWin.top - 10);
	g_ListView->LoadRoot();

	g_InitComplete = TRUE;

	RECT treeView;
	GetWindowRect(g_TreeView->GetThisHandle(), &treeView);
	nleftWnd_width = treeView.right - treeView.left;

	return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;

	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	}
}

LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm)
{
	LPNMTOOLBAR lpnmToolBar = (LPNMTOOLBAR)pnm;
	int nCurSelIndex;

	if (g_InitComplete) //Để tránh vòng lặp
	{
		LPNMTREEVIEW lpnmTree = (LPNMTREEVIEW)pnm;

		switch (pnm->code)
		{
		case TVN_ITEMEXPANDING:
			g_TreeView->Expanding(lpnmTree->itemOld.hItem, lpnmTree->itemNew.hItem);

			break;
			//------------------------------------------------------------------------------
		case TVN_SELCHANGED:
			TerminateThread(g_thread_1, 0); // Tránh trường hợp đang load cái cũ giữa chừng load cái mới chồng lên
			CreateThread(NULL, 0, LoadTreeViewExpand, g_TreeView, 0, 0);
			g_thread_1 = CreateThread(NULL, 0, LoadListView, g_ListView, 0, 0);

			break;
		case NM_CLICK:
			if (pnm->hwndFrom == g_ListView->GetThisHandle())
			{
				nCurSelIndex = ListView_GetNextItem(g_ListView->GetThisHandle(), -1, LVNI_FOCUSED);
				if (nCurSelIndex != -1)
					g_ListView->DisplayInfoCurSel();

			}
			break;
		case NM_DBLCLK:
			if (pnm->hwndFrom == g_ListView->GetThisHandle())
			{
				TerminateThread(g_thread_1, 0); // Tránh trường hợp đang load cái cũ giữa chừng load cái mới chồng lên
				TerminateThread(g_thread_2, 0); // Tránh trường hợp đang load cái cũ giữa chừng load cái mới chồng lên
				g_thread_2 = CreateThread(NULL, 0, LoadListViewDBClick, g_ListView, 0, 0);
			}
			break;
		case NM_CUSTOMDRAW: // Khi người dùng kéo thay đổi kích thước TreeView
			if (pnm->hwndFrom == g_TreeView->GetThisHandle())
			{
				g_ListView->Resize();
			}
			break;
		}

	}

	return 0;
}

void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	g_ListView->Resize();
	g_TreeView->Resize();
	g_StatusBar->Resize();
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);
}

void OnClose(HWND hwnd)
{
	const int BUFFERSIZE = 260;
	WCHAR buffer[BUFFERSIZE];
	GetCurrentDirectory(BUFFERSIZE, buffer);
	
	std::wstring configPath = buffer;
	configPath += L"\\";
	/*int strlen = configPath.length();
	while (configPath[strlen - 1] != _T('\\'))
	{
		strlen--;
	}
	configPath = configPath.substr(0, strlen);*/
	configPath += CONFIG_FILE;

	RECT mainRect;
	GetWindowRect(hwnd, &mainRect);

	std::wstring width = std::to_wstring(mainRect.right - mainRect.left);
	WritePrivateProfileString(L"Screen Resolution", L"Width", const_cast<LPWSTR>(width.c_str()), const_cast<LPWSTR>(configPath.c_str()));

	std::wstring height = std::to_wstring(mainRect.bottom - mainRect.top);
	WritePrivateProfileString(L"Screen Resolution", L"Height", const_cast<LPWSTR>(height.c_str()), const_cast<LPWSTR>(configPath.c_str()));

	DestroyWindow(hwnd);
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);

	// Giải phóng vùng nhớ đã cấp phát
	delete g_ListView;
	delete g_TreeView;
}

DWORD WINAPI LoadListViewDBClick(LPVOID lpParameter)
{
	MyExplorer::ListView* p = (MyExplorer::ListView*)lpParameter;
	p->LoadCurSel();

	return 0;
}

DWORD WINAPI LoadTreeViewExpand(LPVOID lpParameter)
{
	MyExplorer::TreeView* p = (MyExplorer::TreeView*)lpParameter;
	p->Expand(p->GetCurSel());

	return 0;
}

DWORD WINAPI LoadListView(LPVOID lpParameter)
{
	MyExplorer::ListView* p = (MyExplorer::ListView*)lpParameter;
	p->ClearAll(); //Xóa sạch List View để nạp cái mới
	p->LoadChild(g_TreeView->GetCurPIDL());

	return 0;
}

void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
	RECT treeView;
	GetWindowRect(g_TreeView->GetThisHandle(), &treeView);

	RECT listView;
	GetWindowRect(g_ListView->GetThisHandle(), &listView);
	if (keyFlags == MK_LBUTTON)
	{
		if (xSizing)
		{
			if (xSizing)
			{
				nleftWnd_width = x;

				RECT rect;
				GetClientRect(hwnd, &rect);

				MoveWindow(g_TreeView->GetThisHandle(), rect.left,
					rect.top + 0, rect.left + (nleftWnd_width - 2),
					(rect.bottom - (0 + 22)), TRUE);

				MoveWindow(g_ListView->GetThisHandle(), rect.left + nleftWnd_width + 2,
					rect.top + 0, rect.right - (nleftWnd_width + 2),
					rect.bottom - (0 + 22), TRUE);
			}
		}
	}

	if (x >= (treeView.right - treeView.left) && x <= (listView.left - treeView.left))
	{
		SetCursor(LoadCursor(hInst, IDC_SIZEWE));
		ShowCursor(TRUE);
	}
}

void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	RECT treeView;
	GetWindowRect(g_TreeView->GetThisHandle(), &treeView);

	RECT listView;
	GetWindowRect(g_ListView->GetThisHandle(), &listView);

	xSizing = x >= (treeView.right - treeView.left) && x <= (listView.left - treeView.left);

	if (xSizing)
	{
		SetCapture(hwnd);
		if (xSizing)
		{
			SetCursor(LoadCursor(hInst, IDC_SIZEWE));
			ShowCursor(TRUE);
		}
	}
}

void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	ReleaseCapture();
	if (xSizing)
	{
		xSizing = FALSE;
	}
}