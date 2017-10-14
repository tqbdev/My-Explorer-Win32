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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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

	g_ListDisk = new MyExplorer::ListDisk;
	g_ListDisk->GetDisksOnSystem();

	g_TreeView = new MyExplorer::TreeView;
	g_TreeView->Create(hwnd, IDC_TREEVIEW, hInst, (mainWin.right - mainWin.left) * 2 / 7, mainWin.bottom - mainWin.top - 10);
	g_TreeView->LoadRoot(g_ListDisk);
	g_TreeView->SetFocus_();

	g_ListView = new MyExplorer::ListView;
	g_ListView->Create(hwnd, IDC_LISTVIEW, hInst, 0, mainWin.bottom - mainWin.top - 10);
	g_ListView->LoadRoot(g_ListDisk);

	g_InitComplete = TRUE;

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
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);

	// Giải phóng vùng nhớ đã cấp phát
	delete g_ListDisk;
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
	p->LoadChild(g_TreeView->GetCurPath());

	return 0;
}