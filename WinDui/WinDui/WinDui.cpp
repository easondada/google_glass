// WinDui.cpp : 定义应用程序的入口点。
//第二次测试
#pragma once
#include "stdafx.h"
#define _AFXDLL
#include "afxdlgs.h"
#include "UIlib.h"
#include "stdio.h"
#define RADIAN 3.1415926/180.0
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
	CDuiFrameWnd ():flag(1){}
	virtual LPCTSTR    GetWindowClassName() const   { return _T("DUIMainFrame"); }
	virtual CDuiString GetSkinFile()                { return _T("WinDui.xml"); }
	virtual CDuiString GetSkinFolder()              { return _T(""); }
	void Init()
	{//初始化工作，控件对接,确保控件指针有所指的实体
		cbtn[0] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_1")));
		cbtn[1] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_2")));
		cbtn[2] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_3")));
		cbtn[3] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_4")));
		cbtn[4] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_5")));
		C_area = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("area_1")));
		PostMessage(WM_PAINT);
	}
	COLORREF ColorConvert(COLORREF &clr,DWORD Mode)//(ABGR)-->(ARGB);A-->FF
	{//颜色转换
		//Mode=0:透明
		//Mode=1:不透明
	//小端模式
		char *pByte = (char*)&clr;
		char tmp;
		tmp = *(pByte);
		*(pByte) = *(pByte + 2);
		*(pByte + 2) = tmp;
		if (Mode)
		{
			clr = clr | 0xFF000000;//将透明度A设为FF
		}
		else
		{
			clr = clr & 0x00FFFFFF;//透明度A设为00
		}
		return clr;
	}
	void doDraw(HDC hdc, POINTS centre, DWORD radian, float start, float angle)//角度，所占比例*360=度数
	{//画圆弧线图
		//DWORD radian;//半径
		//radian = (DWORD)GetRadian(beg, end);
		MoveToEx(hdc, (int)(centre.x + radian*cos(start*RADIAN)),
			(int)(centre.y - radian*sin(start*RADIAN)),
			NULL);
		AngleArc(hdc, centre.x, centre.y, radian, start, angle);
		return;
	}
	void OnPaint(HWND hWnd)
	{//完整的画饼图事件
		POINTS  centre = { 220, 400 };//圆心坐标
		DWORD r = 60;//半径
		float startAngle = 0;//开始角度
		ColorForDraw();//得到颜色
		GetPercentAngle();//获得比例
		HDC hdc = GetDC(hWnd);//绘图句柄
		//获得画笔:有多少种类型信息就有多少画笔
		HPEN hPen[5], hOldPen[5];
		//开画
		for (int i = 0; i < 5; i++)
		{
			hPen[i] = CreatePen(PS_SOLID, 3, ColorConvert(color[i],0));
			hOldPen[i] = (HPEN)SelectObject(hdc, hPen[i]);

			for (int j = 0; j < 18; j++)
			{//重复画弧线动作
				doDraw(hdc, centre, r += 2, startAngle,percentAngle[i]);
			}
			startAngle += percentAngle[i];//更新下次开始角度
			r = 60;//重置半径
			SelectObject(hdc, hOldPen[i]);
			DeleteObject(hPen[i]);
		}
		//释放DC句柄
		ReleaseDC(hWnd,hdc);
		//EndPaint(hWnd, &ps);
	}
	void GetButtonColor(int i, CDuiString  strName)
	{//用户更改按钮颜色
		CColorDialog  *ccd = new CColorDialog();
		if (ccd->DoModal() == IDOK)
		{
			cbtn[i] = static_cast<CButtonUI*>(m_PaintManager.FindControl(strName));
			COLORREF clr = ccd->GetColor();//（ABGR）
			cbtn[i]->SetBkColor(ColorConvert(clr,1));
			//flag = !flag;//只要用户更改过颜色才重绘饼图
			SendMessage(WM_PAINT);//直接send了
			//InvalidateRect(m_hWnd, NULL, TRUE);//重绘，为了触发WM_PAINT消息
		}
		delete ccd;
	}
	void ColorForDraw()
	{//获取颜色用来画饼图
		//Init();//确保控件指针有所指的实体
		for (int i = 0; i < 5; i++)
		{
			color[i] = cbtn[i]->GetBkColor();
		}
		/*COLORREF clr = RGB(255, 100, 0);
		TCHAR szText[256] = { 0 };
		swprintf_s(szText, _T("颜色=%08X/n"), ColorConvert(clr,0));
		WriteConsole(g_Output, szText, _tcslen(szText), NULL, NULL);*/
	}
	void GetPercentAngle()
	{//获取比例，测试阶段
		percentAngle[0] = 10;
		percentAngle[1] = 30;
		percentAngle[2] = 60;
		percentAngle[3] = 120;
		percentAngle[4] = 140;
		//和应为360
	}
	void OnButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{//测试窗口坐标
		TCHAR szText[256] = { 0 };
		swprintf_s(szText, _T("WM_LBUTTONDOWN:按键状态=%08X,x=%d,y=%d\n"), wParam, LOWORD(lParam), HIWORD(lParam));
		WriteConsole(g_Output, szText, _tcslen(szText), NULL, NULL);
	}
	virtual void Notify(TNotifyUI& msg)
	{//控件处理函数
		if (msg.sType == _T("click"))
		{
			CDuiString  strName = msg.pSender->GetName();
			//颜色按钮
			if (strName == _T("colorbtn_1"))
			{
				GetButtonColor(0, strName);
			}
			else if (strName == _T("colorbtn_2"))
			{
				GetButtonColor(1, strName);
			}
			else if (strName == _T("colorbtn_3"))
			{
				GetButtonColor(2, strName);
			}
			else if (strName == _T("colorbtn_4"))
			{
				GetButtonColor(3, strName);
			}
			else if (strName == _T("colorbtn_5"))
			{
				GetButtonColor(4, strName);
			}
			//标题按钮
			if (strName == _T("choice_1"))
			{
				C_area = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("area_1")));
				COLORREF clr = 0xFFFF3300;//边框颜色
				C_area->SetBorderColor(clr);
			}
			if (strName == _T("installbtn"))
			{//最大的按钮
				CProgressUI* pProgress = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("ProgressDemo1")));
				pProgress->SetValue(100);
			}
		}
		__super::Notify(msg);
	}
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{//系统消息处理函数
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			//OnButtonDown(m_hWnd, wParam, lParam);
		case WM_PAINT:
			if (flag)
			{//先等主窗口创建出来在画饼图
				//第一次WM_PAINT消息出现时代表主窗口已经创建了
				//可以进行对接控件
				Init();
				flag = 0;
			}
			else
				OnPaint(m_hWnd);
			break;
		}
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}
private:
	CButtonUI *cbtn[5];//颜色按钮
	CContainerUI *C_area;//布局容器
	COLORREF color[5];//5种颜色
	float percentAngle[5];//比例角度
	INT flag;//控制初始化控件和画饼图的先后顺序
};
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//AllocConsole();
	//g_Output = GetStdHandle(STD_OUTPUT_HANDLE);
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


