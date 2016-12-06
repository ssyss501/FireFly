#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;

class CGDIButton
{
public:
	// 标准构造函数
	CGDIButton()
	{
		m_bCheck = FALSE;
		m_bHovering = FALSE;
		m_bPressed = FALSE;
		m_bDisabled = FALSE;
		m_pIcon = NULL;
		m_pParent = NULL;
	}

	virtual ~CGDIButton(){}

	// 设置按钮矩形位置
	void SetRect(LPCRECT lpRect);
	// 获取按钮矩形位置
	void GetRect(LPRECT lpRect){*lpRect = *m_rcRect;}
	// 检测指定点是否处于按钮内
	BOOL PtInButton(CPoint& point){return m_rcRect.PtInRect(point);}
	// 绘制按钮
	void DrawButton( Gdiplus::Graphics&  graphics);
	void DrawButton( Gdiplus::Graphics&  graphics,Image* pImage);

	// 创建按钮
	void CreateButton(LPCTSTR lpszCaption,LPRECT lpRect,CWnd* pParent, Image* pImage, UINT nCount = 4, UINT nID = 0);
	//设置按钮图标信息
	void  SetIconInfo(Image *pIcon,CPoint ptPos = CPoint(5,3),BOOL bRedraw=FALSE);
	// 设置按钮状态
	void EnableButton(BOOL bEnable)
	{
		m_bDisabled = !bEnable;
		m_pParent->InvalidateRect(m_rcRect);
	}

	// 设置按钮选定状态
	void SetCheck(BOOL bCheck)
	{
		m_bCheck = bCheck;
		m_pParent->InvalidateRect(m_rcRect);
	}
	// 获取按钮Id
	UINT GetControlId(){return m_nID;};
	// 触发按钮事件
	void DoCommand()
	{
		if(!m_bDisabled)
		{
			m_bCheck = !m_bCheck;
			m_pParent->InvalidateRect(m_rcRect);
			m_pParent->PostMessage(WM_COMMAND, m_nID);
		}
	}

	// 鼠标按下按钮事件
	void LButtonDown()
	{
		if(!m_bDisabled)
		{
			m_bPressed = TRUE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	};
	// 鼠标放下按钮事件
	void LButtonUp()
	{
		if(!m_bDisabled)
		{
			if(m_bPressed == TRUE)
			{
				m_bCheck = !m_bCheck;
				m_bPressed = FALSE;
				m_pParent->InvalidateRect(m_rcRect);
				m_pParent->PostMessage(WM_COMMAND, m_nID);
			}
		}
	}
	// 鼠标进入按钮区域事件
	void MouseHover()
	{
		if(!m_bDisabled)
		{
			m_bHovering = TRUE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	}
	// 鼠标离开按钮区域事件
	void MouseLeave()
	{
		if(!m_bDisabled)
		{
			m_bPressed = FALSE;
			m_bHovering = FALSE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	}

protected:
	// 按钮矩形位置
	CRect	m_rcRect;
	// 标识鼠标进入按钮区域
	BOOL	m_bHovering;
	// 标识已经点击按钮
	BOOL	m_bPressed;
	// 标识按钮已经被禁用
	BOOL	m_bDisabled;
	// 标识按钮已经被选中
	BOOL	m_bCheck;
	// 按钮图片
	Image *m_pImage;
	// 按钮图片中包含的图片个数
	UINT	m_nImageCount;

	//图标(一个按钮可以给它设置一个小图标)
	Image *m_pIcon;
	//图标左上角位置
	CPoint m_ptIcon;

	// 按钮Id号
	int		m_nID;
	// 按钮父窗口
	CWnd*	m_pParent;
	//按钮标题文本
	CString m_strCaption;
};
