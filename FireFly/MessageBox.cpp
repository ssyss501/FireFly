// MessageBox.cpp : 实现文件
//

#include "stdafx.h"
#include "FireFly.h"
#include "MessageBox.h"
#include "afxdialogex.h"
#include "GDIButton.h"

// CMessageBox 对话框

IMPLEMENT_DYNAMIC(CMessageBox, CDialogEx)

//CMessageBox::CMessageBox(CWnd* pParent /*=NULL*/)
//	: CDialogEx(CMessageBox::IDD, pParent)
//{
//
//}

CMessageBox::CMessageBox(UINT weight,UINT height,CString str,CString strTitle)
	: CDialogEx(CMessageBox::IDD)
{
	this->weight=weight;
	this->height=height;
	this->str=str;
	this->strTitle=strTitle;
	m_iSystemButtonHovering=-1;
	m_iCommandButtonHovering=-1;
	m_iCommandButtonPushed=-1;
}

CMessageBox::~CMessageBox()
{
}

void CMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageBox, CDialogEx)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_OK_BT,&CMessageBox::OnOKBTN)
END_MESSAGE_MAP()


// CMessageBox 消息处理程序


int CMessageBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;


	CWnd::SetWindowPos(NULL,0,0,weight,height,SWP_NOZORDER|SWP_NOMOVE);
	CenterWindow(GetParent());

	// 关闭按钮图片188*22
	//关闭按钮
	//关闭按钮的位置
	CRect rtButton;
	CGDIButton dButton;
	rtButton.left=weight-47;
	rtButton.top=0;
	rtButton.right=weight;
	rtButton.bottom=22;

	//存在MAP里 调用拷贝构造函数，生成对象副本，这个函数走完析构临时对象，不影响map中的对象
	dButton.CreateButton(_T("关闭"),&rtButton,this,CMemoryImage::GetInstance()->GetImage(IDB_CLOSE),4,IDC_CLOSE_BT);
	m_rcBtn[IDC_CLOSE_BT]=dButton;

	rtButton.left=weight/2-30;
	rtButton.top=height-50;
	rtButton.right=weight/2+30;
	rtButton.bottom=height-20;

	//存在MAP里 调用拷贝构造函数，生成对象副本，这个函数走完析构临时对象，不影响map中的对象
	dButton.CreateButton(_T("确定"),&rtButton,this,CMemoryImage::GetInstance()->GetImage(IDB_MESSAGEBOXBUTTON),1,IDC_OK_BT);
	m_rcBtn[IDC_OK_BT]=dButton;
	return 0;
}


void CMessageBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rtButton;
	//CRect rtWindow;
	//GetClientRect(&rtWindow);

	//测试鼠标的位置在不在按钮范围内
	int iButton = ButtonHitTest(point, rtButton);

	//如果在自绘范围内 把鼠标指针设为手指
	if(iButton!=-1)
	{
		::SetCursor(::LoadCursor(NULL,IDC_HAND));
	}
	//iButton  现在鼠标停在位置的控件ID
	//m_iButtonHovering 原来鼠标停在位置的控件ID
	//下面这个为了判断 鼠标位置有没有离开或重新进入
	//如果鼠标在控件上面移动，上面的函数还会走的，
	//当前的停留控件ID==之前的停留控件ID 这是不需要重绘的
	//如果不等证明是从不是控件的区域进入，或从控件中离开，或从一个控件到另一个控件上。

	//m_iButtonHovering默认值-1证明在控件外面。

	//如果iButton != m_iButtonHovering 当前鼠标范围 不等于上次的范围，证明从控件离开了
	//m_iButtonHovering != -1上一个空间不是空白，所以是一个控件移动到另一个空间，老的空间需要重绘
	//iButton != -1 当前没有移动到空白，所以新的控件需要重绘
	if(iButton != m_iSystemButtonHovering)//证明状态发生变化
	{
		//这里需要处理两种情况
		//这种是上一个按钮的离开 比如从A移动到B  A需要回复原来状态需要重绘， B变为现在的激活状态需要重绘
		if(m_iSystemButtonHovering != -1) //
		{
			m_rcBtn[m_iSystemButtonHovering].MouseLeave();
			m_iSystemButtonHovering = -1;
		}
		//现在一个按钮的进入
		if(iButton != -1)
		{
			m_iSystemButtonHovering = iButton;
			m_rcBtn[m_iSystemButtonHovering].MouseHover();
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMessageBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rcButton;
	int iButton=this->ButtonHitTest(point,rcButton);

	//弹起，把-1赋值 证明没有按下
	if(iButton!=-1)
	{
		m_iCommandButtonPushed=-1;
		this->m_rcBtn[iButton].LButtonUp();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMessageBox::OnLButtonDown(UINT nFlags, CPoint point)
{
//	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CRect rcButton;
	int iButton=this->ButtonHitTest(point,rcButton);

	if(iButton!=-1)
	{
		m_iCommandButtonPushed=iButton;
		this->m_rcBtn[iButton].LButtonDown();
		return;
	}

	//一定要放在最后 或者精确声明可拖动的区域，先识别这个就影响以后的判断了
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMessageBox::OnPaint()
{
	CClientDC dc(this);
	CMemoryDC mDC(&dc);

	CRect rcClient;
	GetClientRect(&rcClient);

	// 获取重绘矩形位置
	CRect rcPaint;
	mDC.GetClipBox(&rcPaint);
	Gdiplus::Graphics graphics(mDC.m_hDC);

	StringFormat stringFormat;
	LOGFONT lfFont;
	memset(&lfFont, 0, sizeof(lfFont));
	lfFont.lfHeight = -13;
	lfFont.lfWeight = 300;
	lstrcpy(lfFont.lfFaceName, _T("微软雅黑"));
	Gdiplus::Font font(mDC.GetSafeHdc(),&lfFont);

	//所有的图片都在类里面CMemoryImage，
	//绘制背景
	graphics.DrawImage(CMemoryImage::GetInstance()->GetImage(IDB_MESSAGEBOXDLG),0, 0, rcClient.Width(), rcClient.Height());

	// 绘制标题栏
	RectF TitleRect;
	SolidBrush brush((ARGB)Color::Black);
	TitleRect.X=5, TitleRect.Y=3; TitleRect.Width = 200; TitleRect.Height = 17;
	graphics.DrawString(strTitle.GetBuffer(), strTitle.GetLength(),&font, TitleRect,&stringFormat, &brush);


	//绘制关闭按钮
	map<UINT,CGDIButton>::iterator it;
	for(it=m_rcBtn.begin();it!=m_rcBtn.end();it++)
	{
		CGDIButton& dButton = it->second;
		CRect rcControl;
		// 当前按钮是窗口关闭按钮
		dButton.GetRect(&rcControl);

		// 判断当前按钮是否需要重绘
		if(!rcPaint.IsRectEmpty() && !CRect().IntersectRect(&rcControl, rcPaint))
		{
			continue;
		}
		dButton.DrawButton(graphics);
	}

	//绘制 确定 文字
	CString Correct=L"确定";
	TitleRect.X=(REAL)weight/2-30+15, TitleRect.Y=(REAL)height-45; TitleRect.Width = (REAL)60; TitleRect.Height = (REAL)30;
	graphics.DrawString(Correct.GetBuffer(), Correct.GetLength(),&font, TitleRect,&stringFormat, &brush);

	//绘制 对话框内容
	TitleRect.X=(REAL)5, TitleRect.Y=(REAL)height/2; TitleRect.Width = (REAL)weight; TitleRect.Height = (REAL)height/4;
	graphics.DrawString(str.GetBuffer(), str.GetLength(),&font, TitleRect,&stringFormat, &brush);

}

UINT CMessageBox::ButtonHitTest(CPoint pt,CRect& rcButton)
{
	map<UINT,CGDIButton>::iterator it; //迭代器 遍历map
	for(it=m_rcBtn.begin();it!=m_rcBtn.end();it++)
	{
		CGDIButton& dButton=it->second; //取出按钮对象
		if(dButton.PtInButton(pt))      //鼠标位置在不在按钮范围内
		{
			dButton.GetRect(&rcButton);//获得当前按钮范围
			return it->first;          //返回当前ID first是按钮ID,second 是按钮对象
		}
	}
	return -1;
}

BOOL CMessageBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}


BOOL CMessageBox::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDC_CLOSE_BT:
		{
			SendMessage( WM_SYSCOMMAND,SC_CLOSE);
		}
		break;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

void CMessageBox::OnOKBTN()
{
	SendMessage( WM_SYSCOMMAND,SC_CLOSE);
}
