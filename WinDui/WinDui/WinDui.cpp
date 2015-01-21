// WinDui.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "CDuiFrameWnd.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

	CDuiFrameWnd *pFrame = new CDuiFrameWnd;
	if (!pFrame) return 0;
	pFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow();
	pFrame->ShowModal();

	delete pFrame;
	return 0;
}


