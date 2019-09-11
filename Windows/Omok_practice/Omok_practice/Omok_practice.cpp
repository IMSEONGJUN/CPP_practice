// Omok.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Omok_practice.h"
#include <list>
#include "GameManager.h"
#include <commdlg.h>
#include "WriteAndRead.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

GameManager gameManager;

WriteAndRead g_writeAndRead;

void saveFile(HWND hWnd);
void loadFile(HWND hWnd);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OMOK_PRACTICE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OMOK_PRACTICE));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OMOK_PRACTICE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_OMOK_PRACTICE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int x, y;
	static int colorCount = 0;
	
	switch (message)
	{
	case WM_CREATE:
		gameManager.initialize(25, 25, 30, 30);
		gameManager.getDrawing().initializeButton(hWnd, hInst);
		gameManager.setGridInMemoryZero();
		//g_writeAndRead.setDrawing(gameManager.getDrawingPtr());
		break;
		// case WM_DESTROY:
		//�ش� HWND�� ���� �����찡 ����ɶ� Ƥ��.
	case WM_LBUTTONDOWN:
	

		x = LOWORD(lParam);
		y = HIWORD(lParam);

		Stone stone;
		stone.x = x;
		stone.y = y;
		stone.color = (colorCount % 2) + 1;

		
		gameManager.transToIndexValue(stone);

		//g_gridStone.push_back(stone);
		if (!gameManager.isAvailableDrawStone(stone))
		{
			break;
		}

		gameManager.setStoneInMemory(stone);
		
		InvalidateRect(hWnd, NULL, TRUE);

		gameManager.insertDataInStack(stone);

		colorCount++;

		if (gameManager.checkWin(stone))
		{
			if (stone.color == 1)
				MessageBox(hWnd, "Black Win", "Message", MB_OK);
			else if (stone.color == 2)
				MessageBox(hWnd, "White Win", "Message", MB_OK);
		}

		
		InvalidateRect(hWnd, NULL, TRUE);
	
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case ID_BUTTON_RETURN:
			gameManager.clearDataOnce(gameManager.getLastDataInStack());
			
			if (!gameManager.isStackEmpty())
			{
				gameManager.removeLastDataInStack();
			}
			else
			{
				MessageBox(hWnd, "THERE IS NO STONE ANYMORE", "Message", MB_OK);
			}

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_FILE_SAVE:
			saveFile(hWnd);
			break;
		case ID_FILE_LOAD:
			loadFile(hWnd);
			break;
		case ID_FILE_NEW:
			gameManager.setGridInMemoryZero();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

		gameManager.drawGrid(hdc);

		//gameManager.getDrawing().drawGrid(hdc, 20, 20, 30, 30); // �Ʒ��� 2�� �ѹ��� �Ѱ�
		//Drawing &drawing = gameManager.getDrawing();
		//drawing.drawGrid(hdc, 25, 25, 30, 30);

		
		Stone *temp = gameManager.getGridFromMemory();

		for (int i = 0; i < gameManager.getXcount() * gameManager.getYcount(); i++)
		{
			if (temp[i].color != 0)
			{
				//gameManager.getDrawing().drawStone2(hdc, temp[i].x, temp[i].y, temp[i].color);
				gameManager.drawStone(hdc, temp[i]);
			}
			else
				continue;
		}
		
	
		//if (!g_gridStone.empty())
		//{
		//	std::list<Stone> ::iterator iter = g_gridStone.begin();
		//	for (; iter != g_gridStone.end(); iter++)
		//	{
		//		drawStone2(hdc, iter->x, iter->y, iter->color);
		//	}
		//}

		//HGDIOBJ original = SelectObject(hdc, GetStockObject(DC_PEN));

		//SelectObject(hdc, GetStockObject(BLACK_PEN));
		//SelectObject(hdc, GetStockObject(DC_BRUSH));
		//SetDCBrushColor(hdc, RGB(0, 0, 0));
		//Ellipse(hdc, 150, 150, 200, 200);

		//SetDCBrushColor(hdc, RGB(255, 255, 255));
		//Ellipse(hdc, 350, 350, 400, 400);

		//SetDCBrushColor(hdc, RGB(255, 0, 0));
		//Ellipse(hdc, 550, 550, 600, 600);

		//SelectObject(hdc, original);

		EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void saveFile(HWND hWnd)
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260] = { 0 };       // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0data file\0*.dat");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		g_writeAndRead.writeFile(ofn.lpstrFile, gameManager);

	}

}

void loadFile(HWND hWnd)
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260] = { 0 };       // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0data file\0*.dat");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_SHOWHELP;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		gameManager.setGridInMemoryZero();
		g_writeAndRead.readFile(ofn.lpstrFile, gameManager.getGridFromMemory());
		InvalidateRect(hWnd, NULL, TRUE);

		//paintReadFile(hWnd, g_coordinates2);
	}
}
// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
