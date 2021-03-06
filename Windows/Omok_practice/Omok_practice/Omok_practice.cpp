// Omok.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "Omok_practice.h"
#include <list>
#include "GameManager.h"
#include "NetworkManager.h"
#include <commdlg.h>
#include "WriteAndRead.h"
#include <Logger.h>

#pragma warning(disable:4996)

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

GameManager gameManager;
NetworkManager networkManager;

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

	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OMOK_PRACTICE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OMOK_PRACTICE));

	// 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    const int screenWidth = GetSystemMetrics( SM_CXSCREEN );
    const int screenHeight = GetSystemMetrics( SM_CYSCREEN );
    const int windowWidth = 1280;
    const int windowHeight = 850;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		screenWidth / 2 - (windowWidth / 2), 
        screenHeight / 2 - (windowHeight / 2), 
        windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

    networkManager.initialize( hWnd, &gameManager );

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int x, y;

    //static int colorCount;
	
	switch (message)
	{
	case WM_CREATE:
		gameManager.initialize(25, 25, 30, 30);
		gameManager.getDrawing().initializeButton(hWnd, hInst);
		gameManager.setGridInMemoryZero();
		//g_writeAndRead.setDrawing(gameManager.getDrawingPtr());
		break;
		// case WM_DESTROY:
		//해당 HWND를 가진 윈도우가 종료될때 튄다.

    case WM_SOCKET:
        networkManager.onSocketMessage( wParam, lParam );
        break;

	case WM_LBUTTONDOWN:
	

		x = LOWORD(lParam);
		y = HIWORD(lParam);

		Stone stone;
		stone.x = x;
		stone.y = y;
        stone.color = networkManager.getInitStoneColor();

		
		gameManager.transToIndexValue(stone);

		//g_gridStone.push_back(stone);
		if (!gameManager.isAvailableDrawStone(stone))
		{
			break;
		}

        if( networkManager.isMyTurn() )
        {
            networkManager.sendStoneIndex( stone.x, stone.y, stone.color );
           
            networkManager.setMyTurn(false);
        }
        
		break;
        ////////////
        

		//gameManager.setStoneInMemory(stone);
		
		//InvalidateRect(hWnd, NULL, TRUE);

		//gameManager.insertDataInStack(stone);

		//colorCount++;

		//if (gameManager.checkWin(stone))
		//{
		//	if (stone.color == 1)
		//		MessageBox(hWnd, "Black Win", "Message", MB_OK);
		//	else if (stone.color == 2)
		//		MessageBox(hWnd, "White Win", "Message", MB_OK);
		//}

		//
		//InvalidateRect(hWnd, NULL, TRUE);
	
		//break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case ID_BUTTON_RETURN:
			networkManager.sendPacketTypeDelete();
			//try {
			//	gameManager.clearDataOnce(gameManager.getLastDataInStack());
			//	gameManager.removeLastDataInStack();
			//}
			//catch (int)
			//{
			//	MessageBox(hWnd, "THERE IS NO STONE ANYMORE", "Message", MB_OK);
			//}

			//InvalidateRect(hWnd, NULL, TRUE);
			break;
        case ID_BUTTON_CONNECT:
            networkManager.connectToServer( "192.168.0.9", 5000 );
            break;

        case ID_BUTTON_DISCONNECT:
            networkManager.disconnect();
            break;
		case ID_BUTTON_SENDMSG:
			networkManager.sendMessage();
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
		// TODO: 여기에 그리기 코드를 추가합니다.

		gameManager.drawGrid(hdc);

		//gameManager.getDrawing().drawGrid(hdc, 20, 20, 30, 30); // 아랫거 2줄 한번에 한거
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
// 정보 대화 상자의 메시지 처리기입니다.
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
