// WinDui.cpp : 定义应用程序的入口点。
//第二次测试
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
	{//初始化工作，控件对接,确保控件指针有所指的实体
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
			clr = clr | 0xFF000000;//将透明度A设为FF
		else
			clr = clr & 0x00FFFFFF;//透明度A设为00
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
		for (UINT i = 0; i < 5; i++)
		{
			hPen[i] = CreatePen(PS_SOLID, 3, ColorConvert(color[i],0));
			hOldPen[i] = (HPEN)SelectObject(hdc, hPen[i]);

			for (UINT j = 0; j < 18; j++)
			{//重复画弧线动作
				if (percentAngle[i])
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
	void GetButtonColor(UINT i, CDuiString  strName)
	{//用户更改按钮颜色
		CColorDialog  *ccd = new CColorDialog();
		if (ccd->DoModal() == IDOK)
		{
			cbtn[i] = static_cast<CButtonUI*>(m_PaintManager.FindControl(strName));
			COLORREF clr = ccd->GetColor();//（ABGR）
			cbtn[i]->SetBkColor(ColorConvert(clr,1));
			SendMessage(WM_PAINT);//直接send了
		}
		delete ccd;
	}
	void ColorForDraw()
	{//获取颜色用来画饼图
		for (UINT i = 0; i < 5; i++)
		{
			color[i] = cbtn[i]->GetBkColor();
		}
	}
	void GetDiskCap()
	{//获取磁盘容量信息
		//为了效果明显，只基于一个卷D:
		LONG fResult;
		freespace = 0;
		allspace = 0;
		unsigned _int64 i64FreeBytesToCaller;
		unsigned _int64 i64TotalBytes;
		unsigned _int64 i64FreeBytes;
		fResult = GetDiskFreeSpaceEx(_T("D:"), (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes);
		if (fResult)
		{//单位MB
			allspace = (float)(_int64)i64TotalBytes / 1024 / 1024;
			freespace = (float)(_int64)i64FreeBytesToCaller / 1024 / 1024;
		}
		else
		{//cout << "设备未准备好";
		}
	}
	ULONGLONG TraverseDir(CString& dir)
	{//遍历文件夹函数,返回字节数,可能偏小
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
	{//按下设置路径按钮后的一系列操作
		BROWSEINFO lpbi;
		TCHAR lpDir[MAX_PATH];
		CString path;
		Foldersize = 0;

		lpbi.hwndOwner = m_hWnd;
		lpbi.pidlRoot = NULL;  // 默认路径
		lpbi.pszDisplayName = lpDir;
		lpbi.lpszTitle = _T("请选择文件夹：");
		lpbi.ulFlags = BIF_NEWDIALOGSTYLE;
		lpbi.lpfn = NULL;
		lpbi.lParam = NULL;
		lpbi.iImage = NULL;

		LPITEMIDLIST lpidl = ::SHBrowseForFolder(&lpbi);
		if (lpidl&&SHGetPathFromIDList(lpidl, lpDir))
		{//里面是获得文件夹大小的代码
			path.Format(L"%s", lpDir);
			Foldersize = TraverseDir(path);
			return Foldersize;
		}
		return 0;
	}
	void GetUserPercentAngle(UINT i, ULONGLONG size)
	{//获取可变路径的角度比例
		//GetDiskCap();
		ULONGLONG res = (size / 1024 /1024);
		percentAngle[i]=(res/allspace)*360;
		if (res==0)
			::MessageBox(NULL, _T("没选择任何目录！"), NULL, NULL);
		if (res>1024)
			swprintf_s(szText, _T("%0.2fGB"), (float)res/1024);
		else
			swprintf_s(szText, _T("%0.2fMB"), (float)res);

		spaceinfo[i]->SetText(szText);
	}
	void GetPercentAngle()
	{//获取比例，测试阶	,如果有bug，直接开放这里
	/*	percentAngle[0] = 0;
		percentAngle[1] = 40;
		percentAngle[2] = 60;
		percentAngle[3] = 120;
		percentAngle[4] = 140;*/
		//和应为360
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
		spaceinfo[3]->SetText(szText);//其他
		swprintf_s(szText, _T("%0.2fGB"), freespace/1024);
		spaceinfo[4]->SetText(szText);//可用
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
				SendMessage(WM_PAINT);
			}
			if (strName == _T("installbtn"))
			{//安装按钮
				CProgressUI* pProgress = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("ProgressDemo1")));
				pProgress->SetValue(100);
			}
			if (strName == _T("path_1"))
			{//设置路径按钮1
				GetUserPercentAngle(0, GetFolderSize());
			}
			else if (strName == _T("path_2"))
			{//设置路径按钮2
				GetUserPercentAngle(1, GetFolderSize());
			}
			else if (strName == _T("path_3"))
			{//设置路径按钮3
				GetUserPercentAngle(2, GetFolderSize());
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
	CLabelUI *spaceinfo[5];//容量大小信息
	ULONGLONG Foldersize;//文件大小
	float freespace;//空闲容量
	float allspace;//总容量
	TCHAR szText[MAX_TEXT];//文本信息
	INT flag;//控制初始化控件和画饼图的先后顺序
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


