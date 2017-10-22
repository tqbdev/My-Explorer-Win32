// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#define STRICT_TYPED_ITEM­IDS
#include <Shlobj.h>
#include <Shellapi.h>

#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")

#include <CommonControls.h>

#define SPLITTER_BAR_WIDTH 10