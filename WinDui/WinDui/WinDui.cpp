// WinDui.cpp : ����Ӧ�ó������ڵ㡣
//�ڶ��β���
#pragma once
#include "stdafx.h"
#define _AFXDLL
#include "afxdlgs.h"
#include "UIlib.h"
#include "stdio.h"
#include <shlobj.h>
#define RADIAN 3.1415926/180.0
#define MAX_TEXT 50

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
class CDuiFrameWnd : public WindowImplBase
{
public:
	CDuiFrameWnd() :flag(1){}
	virtual LPCTSTR    GetWindowClassName() const   { return _T("DUIMainFrame"); }
	virtual CDuiString GetSkinFile()                { return _T("WinDui.xml"); }
	virtual CDuiString GetSkinFolder()              { return _T(""); }
	void Init()
	{//��ʼ���������ؼ��Խ�,ȷ���ؼ�ָ������ָ��ʵ��
		cbtn[0] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_1")));
		cbtn[1] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_2")));
		cbtn[2] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_3")));
		cbtn[3] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_4")));
		cbtn[4] = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn_5")));
		spaceinfo[0] = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("spaceinfo_1")));
		spaceinfo[1] = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("spaceinfo_2")));
		spaceinfo[2] = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("spaceinfo_3")));
		spaceinfo[3] = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("spaceinfo_4")));
		spaceinfo[4] = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("spaceinfo_5")));
		C_area = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("area_1")));
		for (UINT i = 0; i < 5;i++)
			percentAngle[i] = 0;
		GetDiskCap();
		PostMessage(WM_PAINT);
	}
	COLORREF ColorConvert(COLORREF &clr,DWORD Mode)//(ABGR)-->(ARGB);A-->FF
	{//��ɫת��
		//Mode=0:͸��
		//Mode=1:��͸��
	//С��ģʽ
		char *pByte = (char*)&clr;
		char tmp;
		tmp = *(pByte);
		*(pByte) = *(pByte + 2);
		*(pByte + 2) = tmp;
		if (Mode)
			clr = clr | 0xFF000000;//��͸����A��ΪFF
		else
			clr = clr & 0x00FFFFFF;//͸����A��Ϊ00
		return clr;
	}
	void doDraw(HDC hdc, POINTS centre, DWORD radian, float start, float angle)//�Ƕȣ���ռ����*360=����
	{//��Բ����ͼ
		//DWORD radian;//�뾶
		//radian = (DWORD)GetRadian(beg, end);
		MoveToEx(hdc, (int)(centre.x + radian*cos(start*RADIAN)),
			(int)(centre.y - radian*sin(start*RADIAN)),
			NULL);
		AngleArc(hdc, centre.x, centre.y, radian, start, angle);
		return;
	}
	void OnPaint(HWND hWnd)
	{//�����Ļ���ͼ�¼�
		POINTS  centre = { 220, 400 };//Բ������
		DWORD r = 60;//�뾶
		float startAngle = 0;//��ʼ�Ƕ�
		ColorForDraw();//�õ���ɫ
		GetPercentAngle();//��ñ���
		HDC hdc = GetDC(hWnd);//��ͼ���
		//��û���:�ж�����������Ϣ���ж��ٻ���
		HPEN hPen[5], hOldPen[5];
		//����
		for (UINT i = 0; i < 5; i++)
		{
			hPen[i] = CreatePen(PS_SOLID, 3, ColorConvert(color[i],0));
			hOldPen[i] = (HPEN)SelectObject(hdc, hPen[i]);

			for (UINT j = 0; j < 18; j++)
			{//�ظ������߶���
				if (percentAngle[i])
					doDraw(hdc, centre, r += 2, startAngle,percentAngle[i]);
			}
			startAngle += percentAngle[i];//�����´ο�ʼ�Ƕ�
			r = 60;//���ð뾶
			SelectObject(hdc, hOldPen[i]);
			DeleteObject(hPen[i]);
		}
		//�ͷ�DC���
		ReleaseDC(hWnd,hdc);
		//EndPaint(hWnd, &ps);
	}
	void GetButtonColor(UINT i, CDuiString  strName)
	{//�û����İ�ť��ɫ
		CColorDialog  *ccd = new CColorDialog();
		if (ccd->DoModal() == IDOK)
		{
			cbtn[i] = static_cast<CButtonUI*>(m_PaintManager.FindControl(strName));
			COLORREF clr = ccd->GetColor();//��ABGR��
			cbtn[i]->SetBkColor(ColorConvert(clr,1));
			SendMessage(WM_PAINT);//ֱ��send��
		}
		delete ccd;
	}
	void ColorForDraw()
	{//��ȡ��ɫ��������ͼ
		for (UINT i = 0; i < 5; i++)
		{
			color[i] = cbtn[i]->GetBkColor();
		}
	}
	void GetDiskCap()
	{//��ȡ����������Ϣ
		//Ϊ��Ч�����ԣ�ֻ����һ����D:
		LONG fResult;
		freespace = 0;
		allspace = 0;
		unsigned _int64 i64FreeBytesToCaller;
		unsigned _int64 i64TotalBytes;
		unsigned _int64 i64FreeBytes;
		fResult = GetDiskFreeSpaceEx(_T("D:"), (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes);
		if (fResult)
		{//��λMB
			allspace = (float)(_int64)i64TotalBytes / 1024 / 1024;
			freespace = (float)(_int64)i64FreeBytesToCaller / 1024 / 1024;
		}
		else
		{//cout << "�豸δ׼����";
		}
	}
	ULONGLONG TraverseDir(CString& dir)
	{//�����ļ��к���,�����ֽ���,����ƫС
		ULONGLONG filesize = 0;
		CFileFind ff;
		if (dir.Right(1) != "\\")
			dir += "\\";
		dir += "*.*";

		BOOL ret = ff.FindFile(dir);
		while (ret)
		{
			ret = ff.FindNextFile();
			if (ret != 0)
			{
				if (ff.IsDirectory() && !ff.IsDots())
				{
					CString path = ff.GetFilePath();
					TraverseDir(path);
				}
				else if (!ff.IsDirectory() && !ff.IsDots())
				{
					CString name = ff.GetFileName();
					CString path = ff.GetFilePath();
					filesize += ff.GetLength();
				}
			}
		}
		return filesize;
	}
	ULONGLONG GetFolderSize()
	{//��������·����ť���һϵ�в���
		BROWSEINFO lpbi;
		TCHAR lpDir[MAX_PATH];
		CString path;
		Foldersize = 0;

		lpbi.hwndOwner = m_hWnd;
		lpbi.pidlRoot = NULL;  // Ĭ��·��
		lpbi.pszDisplayName = lpDir;
		lpbi.lpszTitle = _T("��ѡ���ļ��У�");
		lpbi.ulFlags = BIF_NEWDIALOGSTYLE;
		lpbi.lpfn = NULL;
		lpbi.lParam = NULL;
		lpbi.iImage = NULL;

		LPITEMIDLIST lpidl = ::SHBrowseForFolder(&lpbi);
		if (lpidl&&SHGetPathFromIDList(lpidl, lpDir))
		{//�����ǻ���ļ��д�С�Ĵ���
			path.Format(L"%s", lpDir);
			Foldersize = TraverseDir(path);
			return Foldersize;
		}
		return 0;
	}
	void GetUserPercentAngle(UINT i, ULONGLONG size)
	{//��ȡ�ɱ�·���ĽǶȱ���
		//GetDiskCap();
		ULONGLONG res = (size / 1024 /1024);
		percentAngle[i]=(res/allspace)*360;
		if (res==0)
			::MessageBox(NULL, _T("ûѡ���κ�Ŀ¼��"), NULL, NULL);
		if (res>1024)
			swprintf_s(szText, _T("%0.2fGB"), (float)res/1024);
		else
			swprintf_s(szText, _T("%0.2fMB"), (float)res);

		spaceinfo[i]->SetText(szText);
	}
	void GetPercentAngle()
	{//��ȡ���������Խ�	,�����bug��ֱ�ӿ�������
	/*	percentAngle[0] = 0;
		percentAngle[1] = 40;
		percentAngle[2] = 60;
		percentAngle[3] = 120;
		percentAngle[4] = 140;*/
		//��ӦΪ360
		//GetDiskCap();
		float sum = 0;
		for (UINT i = 0; i < 3; i++)
		{
			sum += percentAngle[i];
		}
		percentAngle[4] = (freespace / allspace) * 360;
		if (percentAngle[4] >= 360)
			::MessageBox(NULL, _T("error2"), NULL, NULL);
		percentAngle[3] = 360 - sum - percentAngle[4];
		if (percentAngle[3] >= 360)
			::MessageBox(NULL, _T("error3"), NULL, NULL);
		swprintf_s(szText, _T("%0.2fGB"), (allspace-freespace-(sum*360)/allspace)/1024);
		spaceinfo[3]->SetText(szText);//����
		swprintf_s(szText, _T("%0.2fGB"), freespace/1024);
		spaceinfo[4]->SetText(szText);//����
	}
	virtual void Notify(TNotifyUI& msg)
	{//�ؼ�������
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
				SendMessage(WM_PAINT);
			}
			if (strName == _T("installbtn"))
			{//��װ��ť
				CProgressUI* pProgress = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("ProgressDemo1")));
				pProgress->SetValue(100);
			}
			if (strName == _T("path_1"))
			{//����·����ť1
				GetUserPercentAngle(0, GetFolderSize());
			}
			else if (strName == _T("path_2"))
			{//����·����ť2
				GetUserPercentAngle(1, GetFolderSize());
			}
			else if (strName == _T("path_3"))
			{//����·����ť3
				GetUserPercentAngle(2, GetFolderSize());
			}
		}
		__super::Notify(msg);
	}
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{//ϵͳ��Ϣ������
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			//OnButtonDown(m_hWnd, wParam, lParam);
		case WM_PAINT:
			if (flag)
			{//�ȵ������ڴ��������ڻ���ͼ
				//��һ��WM_PAINT��Ϣ����ʱ�����������Ѿ�������
				//���Խ��жԽӿؼ�
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
	CButtonUI *cbtn[5];//��ɫ��ť
	CContainerUI *C_area;//��������
	COLORREF color[5];//5����ɫ
	float percentAngle[5];//�����Ƕ�
	CLabelUI *spaceinfo[5];//������С��Ϣ
	ULONGLONG Foldersize;//�ļ���С
	float freespace;//��������
	float allspace;//������
	TCHAR szText[MAX_TEXT];//�ı���Ϣ
	INT flag;//���Ƴ�ʼ���ؼ��ͻ���ͼ���Ⱥ�˳��
};
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


