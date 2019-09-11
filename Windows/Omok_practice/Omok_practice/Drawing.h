#pragma once

#include <Windows.h>
#define ID_BUTTON_RETURN 1000

class Drawing
{
public:

	void drawGrid(HDC hdc, int xCount, int yCount, int xInterval, int yInterval);

	//void drawStone(HDC hdc, int x, int y, int color);

	void drawStone2(HDC hdc, int x, int y, int color, int xInterval, int yInterval);

	void initializeButton(HWND hWnd, HINSTANCE hInst);

private:

	HWND m_hwndReturn;
};
