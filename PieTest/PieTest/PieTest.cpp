// PieTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PieTest.h"
#include <cmath>
#include "stdio.h"

#define MAX_LOADSTRING 100
#define RADIAN 3.1415926/180.0

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
HANDLE g_Output=0;//标准输入输出句柄

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
double GetRadian(POINTS beg, POINTS end)
{
	double x, y;
	DWORD radian;
	x = end.x - beg.x;
	y = end.y - beg.y;
	radian = (DWORD)sqrt(x*x + y*y);
	return radian;
}
//用AngleArc函数画弧线
void doDraw(HDC hdc, POINTS beg, POINTS end, float start,float angle)//角度，所占比例*360=度数
{
	DWORD radian;//半径
	radian = (DWORD)GetRadian(beg, end);
	MoveToEx(hdc, (int)(beg.x + radian*cos(start*RADIAN)),
		(int)(beg.y - radian*sin(start*RADIAN)),
		NULL);
	AngleArc(hdc, beg.x, beg.y, radian, start, angle);
	return;
}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	AllocConsole();
	g_Output = GetStdHandle(STD_OUTPUT_HANDLE);
 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;
	
	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PIETEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PIETEST));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PIETEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PIETEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

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
void DrawPie(HWND hWnd)
{
	InvalidateRect(hWnd, NULL, TRUE);
}
void OnPaint(HWND hWnd)
{
	POINTS beg = {100,100};
	POINTS end = {60,60};
	PAINTSTRUCT ps = {0};
	HDC hdc;//绘图句柄
	hdc = BeginPaint(hWnd, &ps);
	SetPixel(hdc, beg.x, beg.y, RGB(0, 0, 0));
	SetPixel(hdc, end.x, end.y, RGB(255, 0, 0));
	HPEN hPen_1, hPen_2, hOldPen_1, hOldPen_2;
	hPen_1 = CreatePen(PS_SOLID, 10, RGB(0, 100, 255));
	hPen_2 = CreatePen(PS_SOLID, 10, RGB(100, 100, 0));

	hOldPen_1 = (HPEN)SelectObject(hdc, hPen_1);
	doDraw(hdc,beg,end,0,270);
	hOldPen_2 = (HPEN)SelectObject(hdc, hPen_2);
	doDraw(hdc, beg, end,270,90);

	SelectObject(hdc, hOldPen_1);
	SelectObject(hdc, hOldPen_2);

	DeleteObject(hPen_1);
	DeleteObject(hPen_2);

	EndPaint(hWnd, &ps);
}
void OnClick(HWND hWnd,LPARAM lParam)
{
	char szText[256] = "";
	sprintf_s(szText,"x=%d,y=%d \n",LOWORD(lParam), HIWORD(lParam));
	WriteConsole(g_Output, szText, strlen(szText), NULL, NULL);
}
//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		OnClick(hWnd,lParam);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case ID_Draw:
			DrawPie(hWnd);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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
