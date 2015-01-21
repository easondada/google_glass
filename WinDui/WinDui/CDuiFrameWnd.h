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
	//��ʼ���ؼ�
	void Init();
	//��ɫת��
	COLORREF ColorConvert(COLORREF &clr, DWORD Mode);//(ABGR)-->(ARGB);A-->FF
	//�������¼�
	void doDraw(HDC hdc, POINTS centre, DWORD radian, float start, float angle);//�Ƕȣ���ռ����*360=����
	//��ͼ�¼�
	void OnPaint(HWND hWnd);
	//��ȡ��ť��ɫ
	void GetButtonColor(UINT i, CDuiString  strName);
	//������ɫ
	void ColorForDraw();
	//��ȡ��������
	void GetDiskCap();
	//����Ŀ¼
	ULONGLONG TraverseDir(CString& dir);
	//���Ŀ¼��С
	ULONGLONG GetFolderSize();
	//����Ŀ¼ʱ���½Ƕȱ���
	void GetUserPercentAngle(UINT i, ULONGLONG size);
	//��ȡ�Ƕȱ���
	void GetPercentAngle();
public:
	//�ؼ���Ϣ����
	virtual void Notify(TNotifyUI& msg);
	//ϵͳ��Ϣ����
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
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
	CLabelUI *percent;
};