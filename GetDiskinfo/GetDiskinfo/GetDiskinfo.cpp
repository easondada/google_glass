#include "stdafx.h"
#include <iostream>
#include <Windows.h>
using namespace std;
int main()
{
	LONG fResult;
	unsigned _int64 i64FreeBytesToCaller;
	unsigned _int64 i64TotalBytes;
	unsigned _int64 i64FreeBytes;
	fResult = GetDiskFreeSpaceEx(_T("D:"), (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes);
	if (fResult)
	{
		cout << "�����ܿռ�:" << (float)(_int64)i64TotalBytes / 1024 / 1024 << "MB"<<endl;
		cout << "���̿��пռ�:" << (float)(_int64)i64FreeBytesToCaller / 1024 / 1024 << "MB"<<endl;
	}
	else
	{
		cout << "�豸δ׼����";
	}
	cout << endl;
	system("pause");
	return 0;
}
