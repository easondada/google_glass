// WinDui.cpp : ����Ӧ�ó������ڵ㡣
//�ڶ��β���
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
	virtual LPCTSTR    GetWindowClassName() const   { return _T("DUIMainFrame"); }
	virtual CDuiString GetSkinFile()                { return _T("WinDui.xml"); }
	virtual CDuiString GetSkinFolder()              { return _T(""); }
	void Init()
	{//��ʼ��������δ����
		cbtn[0] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_1")));
		cbtn[1] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_2")));
		cbtn[2] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_3")));
		cbtn[3] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_4")));
		cbtn[4] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_5")));
		C_area = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("area_1")));
	}
	COLORREF ColorConvert(COLORREF &clr)//(ABGR)-->(ARGB);A-->FF
	{//��ɫת���������ò�͸��
	//С��ģʽ
		char *pByte = (char*)&clr;
		char tmp;
		tmp = *(pByte);
		*(pByte) = *(pByte + 2);
		*(pByte + 2) = tmp;
		clr = clr | 0xFF000000;//��͸����A��ΪFF
		return clr;
	}
	void doDraw(HDC hdc, POINTS centre, DWORD radian, float start, float angle)//�Ƕȣ���ռ����*360=����
	{//����ͼ
		//DWORD radian;//�뾶
		//radian = (DWORD)GetRadian(beg, end);
		MoveToEx(hdc, (int)(centre.x + radian*cos(start*RADIAN)),
			(int)(centre.y - radian*sin(start*RADIAN)),
			NULL);
		AngleArc(hdc, centre.x, centre.y, radian, start, angle);
		return;
	}
	void OnPaint(HWND hWnd)
	{//�����Ļ�ͼ�¼�
		POINTS  centre = { 200, 200 };//Բ������
		DWORD r = 70;
		//POINTS end = {60,60};
		//PAINTSTRUCT ps = { 0 };
		HDC hdc;//��ͼ���
		hdc = GetDC(hWnd);
		//hdc = BeginPaint(hWnd, &ps);
		//SetPixel(hdc, centre.x, centre.y, RGB(0, 0, 0));
		//��û���:�ж�����������Ϣ���ж��ٻ���
		HPEN hPen_1, hPen_2, hOldPen_1, hOldPen_2;
		hPen_1 = CreatePen(PS_SOLID, 5, RGB(0, 100, 255));
		hPen_2 = CreatePen(PS_SOLID, 5, RGB(100, 100, 0));
		//����
		hOldPen_1 = (HPEN)SelectObject(hdc, hPen_1);
		for (int i = 0; i < 10; i++)
		{
			doDraw(hdc, centre, r += 4, 0, 270);
		}
		r = 70;
		hOldPen_2 = (HPEN)SelectObject(hdc, hPen_2);
		for (int i = 0; i < 10; i++)
		{
			doDraw(hdc, centre, r += 4, 270, 90);
		}
		//�ͷ�
		SelectObject(hdc, hOldPen_1);
		SelectObject(hdc, hOldPen_2);

		DeleteObject(hPen_1);
		DeleteObject(hPen_2);
		ReleaseDC(hWnd,hdc);
		//EndPaint(hWnd, &ps);
	}
	void GetButtonColor(int i, CDuiString  strName)
	{//��ȡ��ť��ɫ
		CColorDialog  *ccd = new CColorDialog();
		if (ccd->DoModal() == IDOK)
		{
			cbtn[i] = static_cast<CButtonUI*>(m_PaintManager.FindControl(strName));
			COLORREF clr = ccd->GetColor();//��ABGR��
			cbtn[i]->SetBkColor(ColorConvert(clr));
			InvalidateRect(m_hWnd, NULL, TRUE);//�ػ棬Ϊ�˴���WM_PAINT��Ϣ
		}
		delete ccd;
	}
	void OnButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{//���Դ�������
		TCHAR szText[256] = { 0 };
		swprintf_s(szText, _T("WM_LBUTTONDOWN:����״̬=%08X,x=%d,y=%d\n"), wParam, LOWORD(lParam), HIWORD(lParam));
		WriteConsole(g_Output, szText, _tcslen(szText), NULL, NULL);
	}
	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			CDuiString  strName = msg.pSender->GetName();
			//��ɫ��ť
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
			//���ⰴť
			if (strName == _T("choice_1"))
			{
				C_area = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("area_1")));
				COLORREF clr = 0xFFFF3300;//�߿���ɫ
				C_area->SetBorderColor(clr);
			}
			if (strName == _T("installbtn"))
			{//���İ�ť
				CProgressUI* pProgress = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("ProgressDemo1")));
				pProgress->SetValue(100);
			}
		}
		__super::Notify(msg);
	}
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{case WM_LBUTTONDOWN:
			OnButtonDown(m_hWnd, wParam, lParam);
			break;
		case WM_PAINT:
			OnPaint(m_hWnd);
			break;
		default:
			break;
		}
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	}
private:
	CButtonUI *cbtn[5];//��ɫ��ť
	CContainerUI *C_area;//��������
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


