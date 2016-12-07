#include "StdAfx.h"
#include "CustomFuntion.h"


CCustomFuntion::CCustomFuntion(void)
{
}


CCustomFuntion::~CCustomFuntion(void)
{
}


//设置时间
int CCustomFuntion::MyGetTime(CString& CurrentTime)
{
	CTime t=CTime::GetCurrentTime();
	CurrentTime.Format(L"%4.4d年%2.2d月%2.2d日%2.2d:%2.2d:%2.2d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
	return 0;
}

//获取操作系统版本
CString CCustomFuntion::MyGetOS(int id)
{

	switch (id)
	{
	case 2:
		return _T("Windows XP");
		break;
	case 3:
		return _T("Windows Server 2003");//Windows Server 2003
		break;
	case 4:
		return _T("Windows Vista");//	Windows Vista	
		break;
	case 5:
		return _T("Windows 7");//Windows Server 2008/Windows 7
		break;
	default:
		return _T("未知版本");
		break;
	}
}
