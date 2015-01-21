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
	CDuiFrameWnd();
	virtual LPCTSTR    GetWindowClassName()const; 
	virtual CDuiString GetSkinFile();               
	virtual CDuiString GetSkinFolder();
private:
	//初始化控件
	void Init();
	//颜色转码
	COLORREF ColorConvert(COLORREF &clr, DWORD Mode);//(ABGR)-->(ARGB);A-->FF
	//画弧线事件
	void doDraw(HDC hdc, POINTS centre, DWORD radian, float start, float angle);//角度，所占比例*360=度数
	//画图事件
	void OnPaint(HWND hWnd);
	//获取按钮颜色
	void GetButtonColor(UINT i, CDuiString  strName);
	//画笔颜色
	void ColorForDraw();
	//获取磁盘容量
	void GetDiskCap();
	//遍历目录
	ULONGLONG TraverseDir(CString& dir);
	//获得目录大小
	ULONGLONG GetFolderSize();
	//更改目录时更新角度比例
	void GetUserPercentAngle(UINT i, ULONGLONG size);
	//获取角度比例
	void GetPercentAngle();
public:
	//控件消息处理
	virtual void Notify(TNotifyUI& msg);
	//系统消息处理
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
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
	CLabelUI *percent;
};