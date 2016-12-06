
#pragma once
#include <map>
using namespace std;

//内存图片管理类
class CMemoryImage
{
public:
	static CMemoryImage * GetInstance();
	map<UINT, Image*> m_pImage;
	~CMemoryImage(void);

	// 释放所有界面资源
	void	Release();
	static void UnLoad();
	// 释放界面资源Id加载界面资源
	Image*	ImageFromResource(UINT uId);
	//通过控件ID获得图片指针
	Image* CMemoryImage::GetImage(UINT uID);
private:
	CMemoryImage(void);
	static CMemoryImage* m_Instance; //唯一的实例
	Image*	FromResource(UINT uId);
	//map<UINT, Image*> m_pImage;
};
