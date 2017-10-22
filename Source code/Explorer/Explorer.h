#pragma once

#include "resource.h"
#include "ListView.h"
#include "StatusBar.h"
#include <windowsx.h>
#include <vld.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100
#define CONFIG_FILE L"config.ini"

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Handle Message declarations of functions
void				OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void				OnPaint(HWND hwnd);
void				OnDestroy(HWND hwnd);
void				OnClose(HWND hwnd);
BOOL				OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void				OnSize(HWND hwnd, UINT state, int cx, int cy);
LRESULT				OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);

void				OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
void				OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void				OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);

// Xử lý Threads
DWORD WINAPI		LoadListViewDBClick(LPVOID lpParameter);
DWORD WINAPI		LoadTreeViewExpand(LPVOID lpParameter);
DWORD WINAPI		LoadListView(LPVOID lpParameter);

// Global Variables:
HINSTANCE			hInst;                                // current instance
WCHAR				szTitle[MAX_LOADSTRING];              // The title bar text
WCHAR				szWindowClass[MAX_LOADSTRING];        // the main window class name

HANDLE				g_thread_1;							  // Giữ lại HANDLE của Thread load ListView của lúc load chung với TreeView
HANDLE				g_thread_2;							  // Giữ lại HANDLE của Thread load ListView của lúc dùng DBClick

HWND				hwndMain;

MyExplorer::TreeView* g_TreeView;
MyExplorer::ListView* g_ListView;

MyExplorer::StatusBar* g_StatusBar;

BOOL g_InitComplete = FALSE;

BOOL xSizing = FALSE;
int nleftWnd_width = 0;