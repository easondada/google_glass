// WinDui.cpp : 定义应用程序的入口点。
//第二次测试
#pragma once
#include "stdafx.h"
#define _AFXDLL
#include "afxdlgs.h"
#include "UIlib.h"
#include "stdio.h"

//#include "WndShadow.h"
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif
HANDLE g_Output = 0;
class CDuiFrameWnd : public WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const   { return _T("DUIMainFrame"); }
	virtual CDuiString GetSkinFile()                { return _T("WinDui.xml"); }
	virtual CDuiString GetSkinFolder()              { return _T(""); }
	COLORREF ColorConvert(COLORREF &clr)//(ABGR)-->(ARGB);A-->FF
	{//颜色转换，并设置不透明
	//小端模式
		char *pByte = (char*)&clr;
		char tmp;
		tmp = *(pByte);
		*(pByte) = *(pByte + 2);
		*(pByte + 2) = tmp;
		clr = clr | 0xFF000000;//将透明度A设为FF
		return clr;
	}
	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			CDuiString    strName = msg.pSender->GetName();
			if (strName == _T("colorbtn_1"))
			{
				CColorDialog  *ccd = new CColorDialog();
				if (ccd->DoModal() == IDOK)
				{
					cbtn_1 = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_1")));
					COLORREF clr = ccd->GetColor();//（ABGR）
					//TCHAR szChar[256] = {0};
					//wsprintf(szChar, L"clr = %08x\n", clr);
					//WriteConsole(g_Output, szChar, 16, NULL, NULL);
					//wsprintf(szChar, L"ColorConvert(clr) = %8x\n", ColorConvert(clr));
					//WriteConsole(g_Output, szChar, 28, NULL, NULL);
					cbtn_1->SetBkColor(ColorConvert(clr));
				}
				delete ccd;
			}	
		}
		if (msg.sType == _T("selectchanged"))
		{//分页处理
			CDuiString    strName = msg.pSender->GetName();
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab")));

			if (strName == _T("OptionDemo1"))
				pControl->SelectItem(0);
			else if (strName == _T("OptionDemo2"))
				pControl->SelectItem(1);
			else if (strName == _T("OptionDemo3"))
				pControl->SelectItem(2);
		}
		__super::Notify(msg);
	}
private:
	CButtonUI *cbtn_1;//颜色按钮1
};
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	g_Output = GetStdHandle(STD_OUTPUT_HANDLE);
	CPaintManagerUI::SetInstance(hInstance);
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	CDuiFrameWnd *pFrame = new CDuiFrameWnd;
	if (!pFrame) return 0;
	pFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow();
	pFrame->ShowModal();

	delete pFrame;
	return 0;
}


