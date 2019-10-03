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
#include <WinSock2.h>

#define WM_SOCKET           WM_USER + 100

#define ID_BUTTON_LISTEN    1001
#define ID_BUTTON_VIEW_CONNECTED_COUNT    1002
#define ID_BUTTON_START     1003

struct Stone
{
    int x;
    int y;
    int color;
};