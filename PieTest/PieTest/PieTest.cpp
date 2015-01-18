// PieTest.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PieTest.h"
#include <cmath>
#include "stdio.h"

#define MAX_LOADSTRING 100
#define RADIAN 3.1415926/180.0

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HANDLE g_Output=0;//��׼����������

// �˴���ģ���а����ĺ�����ǰ������: 
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
//��AngleArc����������
void doDraw(HDC hdc, POINTS beg, POINTS end, float start,float angle)//�Ƕȣ���ռ����*360=����
{
	DWORD radian;//�뾶
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
 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;
	
	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PIETEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PIETEST));

	// ����Ϣѭ��: 
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
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
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
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
	HDC hdc;//��ͼ���
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
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��: 
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

// �����ڡ������Ϣ�������
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
