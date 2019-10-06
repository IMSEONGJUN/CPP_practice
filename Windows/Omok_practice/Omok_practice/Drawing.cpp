#include "stdafx.h"
#include "Drawing.h"

void Drawing::drawGrid(HDC hdc, int xCount, int yCount, int xInterval, int yInterval)
{
	for (int i = 1; i <= xCount; i++)
	{
		MoveToEx(hdc, i * xInterval, yInterval, NULL);
		LineTo(hdc, i * xInterval, yCount * yInterval); // MoveToEx의 x좌표,y좌표 지점에서 시작하여 LineTo의 x좌표y좌표 지점으로 선을 긋는다.
	}

	for (int i = 1; i <= yCount; i++)
	{
		MoveToEx(hdc, xInterval, i * yInterval, NULL);
		LineTo(hdc, xCount * xInterval, i * yInterval);
	}

}

void Drawing::initializeButton(HWND hWnd, HINSTANCE hInst)
{
	m_hwndReturn = CreateWindow("BUTTON", "RETURN",
		WS_CHILD | WS_VISIBLE,
		800, 50, 90, 30, hWnd,
		(HMENU)ID_BUTTON_RETURN, hInst, NULL);

	m_hwndConnect = CreateWindow("BUTTON", "CONNECT",
		WS_CHILD | WS_VISIBLE,
		800, 150, 100, 30, hWnd,
		(HMENU)ID_BUTTON_CONNECT, hInst, NULL);

	CreateWindow("BUTTON", "DISCONNECT",
		WS_CHILD | WS_VISIBLE,
		800, 200, 100, 30, hWnd,
		(HMENU)ID_BUTTON_DISCONNECT, hInst, NULL);

	HWND hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
		WS_CHILD | WS_VISIBLE,
		800, 250, 250, 100, hWnd, (HMENU)ID_EDIT, NULL, NULL);
}

void Drawing::drawStone2(HDC hdc, int x, int y, int color, int xInterval, int yInterval)
{
	HGDIOBJ original = SelectObject(hdc, GetStockObject(DC_PEN));

	SelectObject(hdc, GetStockObject(BLACK_PEN));
	SelectObject(hdc, GetStockObject(DC_BRUSH));

	if (color == 1)
	{
		SetDCBrushColor(hdc, RGB(0, 0, 0));
	}
	else if (color == 2)
	{
		SetDCBrushColor(hdc, RGB(255, 255, 255));
	}

	Ellipse(hdc, (x + 1) * xInterval - (xInterval / 2), (y + 1) * yInterval - (yInterval / 2),
		(x + 1) * xInterval + (xInterval / 2), (y + 1) * yInterval + (yInterval / 2));

	SelectObject(hdc, original);
}


//void drawStone(HDC hdc, int x, int y, int color)
//{
//	
//	HGDIOBJ original = SelectObject(hdc, GetStockObject(DC_PEN));
//
//	SelectObject(hdc, GetStockObject(BLACK_PEN));
//	SelectObject(hdc, GetStockObject(DC_BRUSH));
//	
//	if (color == 1)
//	{
//		SetDCBrushColor(hdc, RGB(0, 0, 0));
//	}
//	else if (color == 2)
//	{
//		SetDCBrushColor(hdc, RGB(255, 255, 255));
//	}
//
//	int z = x % 30;
//	int t = y % 30;
//		
//	if (z >= 0 && z <= 15 && t >= 0 && t <= 15)
//	{
//		x = x - z;
//		y = y - t;
//	}
//	else if (z >= 16 && z <= 29 && t >= 16 && t <= 29)
//	{
//		x = x + (30 - z);
//		y = y + (30 - t);
//	}
//	else if (z >= 0 && z <= 15 && t >= 16 && t <= 29)
//	{
//		x = x - z;
//		y = y + (30 - t);
//	}
//	else if (z >= 16 && z <= 29 && t >= 0 && t <= 15)
//	{
//		x = x +(30 - z);
//		y = y - t;
//	}
//	Ellipse(hdc, x - 15, y - 15, x + 15, y + 15);
//		
//	SelectObject(hdc, original);
//
//}



