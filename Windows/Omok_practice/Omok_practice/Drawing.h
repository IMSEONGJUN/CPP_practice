#pragma once

#include <Windows.h>
#define ID_BUTTON_RETURN    1000
#define ID_BUTTON_CONNECT   1001
#define ID_BUTTON_DISCONNECT   1002
#define ID_LIST				1003
#define ID_EDIT				1004
#define ID_BUTTON_SENDMSG	1005

class Drawing
{
public:

	void drawGrid(HDC hdc, int xCount, int yCount, int xInterval, int yInterval);
	
	//void drawStone(HDC hdc, int x, int y, int color);

	void drawStone2(HDC hdc, int x, int y, int color, int xInterval, int yInterval);

	void initializeButton(HWND hWnd, HINSTANCE hInst);
	HWND getListbox() const;
	HWND getEditbox() const;

private:

	HWND m_hwndReturn;
	HWND m_hwndConnect;
	HWND m_hWndList;
	HWND m_hWndEditBox;
};
