#pragma once

#include <map>
#include <GdiPlus.h>
using namespace Gdiplus;

//图片内存管理类
class CSkinManager
{
private:
	CSkinManager();
	~CSkinManager();
public:
    static CSkinManager* GetInstance();
	void  SetSkinPath(LPCTSTR lpszSkinPath);
	CString GetSkinPath();
	Image* GetSkinItem(CString lpszFileName);
	static void UnLoad();
private:
	std::map<CString,Image*>  m_mapSkinItems;
	CString m_strSkinPath;
	static CSkinManager* m_Instance;
	
	ULONG_PTR gdiplusToken;

};
