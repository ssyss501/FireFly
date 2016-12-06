
// FireFlyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FireFly.h"
#include "FireFlyDlg.h"
#include "afxdialogex.h"
#include "GDIButton.h"

#define IDC_BASE      10000
#define IDC_BKG       IDC_BASE+1
#define IDC_CLOSE_BT  IDC_BASE+2
#define IDC_MIN_BT	  IDC_BASE+3
#define IDC_SET_BT	  IDC_BASE+4
#define IDC_MAINPAGE_BT	  IDC_BASE+5  //主页
#define IDC_PROC_MANAGE_BT  IDC_BASE+6 //进程管理
#define IDC_REMOTECMD_BT	IDC_BASE+7 //远程CMD

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFireFlyDlg 对话框


CFireFlyDlg::~CFireFlyDlg()
{
	CMemoryImage::UnLoad();
}

CFireFlyDlg::CFireFlyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFireFlyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ICO);

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_GdiplusToken,&gdiplusStartupInput,NULL);

	Image* pImage=NULL;
	m_bTracking=FALSE;
	m_iSystemButtonHovering=-1;
	m_iCommandButtonHovering=-1;
	m_iCommandButtonPushed=-1;
	m_iListHover=-1;
	m_iListPress=-1;
	m_rcTime.left=270,m_rcTime.top=578,m_rcTime.right=460,m_rcTime.bottom=596;
	//文件图片管理类的加载方式
	//pImage=CSkinManager::GetInstance()->GetSkinItem(L"bkg.jpg");
	//m_Image[IDC_BKG]=pImage;
	//pImage=CSkinManager::GetInstance()->GetSkinItem(L"Close.png");
	//m_Image[IDC_CLOSE_BT]=pImage;
	//pImage=CSkinManager::GetInstance()->GetSkinItem(L"Max.png");
	//m_Image[IDC_MAX_BT]=pImage;
	//pImage=CSkinManager::GetInstance()->GetSkinItem(L"Min.png");
	//m_Image[IDC_MIN_BT]=pImage;

	CMemoryImage::GetInstance()->ImageFromResource(IDB_BKG);//背景
	CMemoryImage::GetInstance()->ImageFromResource(IDB_CLOSE);//关闭
	CMemoryImage::GetInstance()->ImageFromResource(IDB_SET);//设置
	CMemoryImage::GetInstance()->ImageFromResource(IDB_MIN);//最小化
	CMemoryImage::GetInstance()->ImageFromResource(IDB_TITLE);//最小化
	CMemoryImage::GetInstance()->ImageFromResource(IDB_BUTTOMEDGE);//最小化
	CMemoryImage::GetInstance()->ImageFromResource(IDB_MAINPAGE);//主页
	CMemoryImage::GetInstance()->ImageFromResource(IDB_FRAME);//ListView背景
	CMemoryImage::GetInstance()->ImageFromResource(IDB_LINE);//ListView网格线
	CMemoryImage::GetInstance()->ImageFromResource(IDB_LISTHOVER);//ListView鼠标经过
	CMemoryImage::GetInstance()->ImageFromResource(IDB_LISTPRESS);//ListView鼠标按下
	CMemoryImage::GetInstance()->ImageFromResource(IDB_PROC_MANAGE);//进程管理
	CMemoryImage::GetInstance()->ImageFromResource(IDB_REMOTECMD);//远程CMD
}

void CFireFlyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFireFlyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CFireFlyDlg::LoadPng(UINT ID,CString path)
{
	CString myPath=_T("F:\\远控\\FireFly\\FireFly\\res\\")+path;
	m_Image[ID] = Image::FromFile(myPath);
}

// CFireFlyDlg 消息处理程序

BOOL CFireFlyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	 this->SetWindowText(L"FireFly远程控制"); 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CFireFlyDlg::OnPaint()
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
	graphics.DrawImage(CMemoryImage::GetInstance()->GetImage(IDB_BKG),0, 0, rcClient.Width(), rcClient.Height());
	//绘制图标
	graphics.DrawImage(CMemoryImage::GetInstance()->GetImage(IDB_TITLE),3, 3, 22, 22);
	//绘制底边栏背景
	graphics.DrawImage(CMemoryImage::GetInstance()->GetImage(IDB_BUTTOMEDGE),0, 578, 900, 22);//height是向下增长


	// 绘制标题栏版本
	RectF TitleRect;
	SolidBrush brush((ARGB)Color::White);
	CString sTitleName = L"Firefly远程控制 1.0 Beta版";
	TitleRect.X=30, TitleRect.Y=5; TitleRect.Width = 200; TitleRect.Height = 17;
	graphics.DrawString(sTitleName.GetBuffer(), sTitleName.GetLength(),&font, TitleRect,&stringFormat, &brush);


	//绘制内容显示区 ListView
	RectF InfoRect;
	InfoRect.X = 0, InfoRect.Y = 115, InfoRect.Width = 1120, InfoRect.Height = 560;
	//graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor); 
	graphics.DrawImage(CMemoryImage::GetInstance()->GetImage(IDB_FRAME),InfoRect,60,10,
						(REAL)CMemoryImage::GetInstance()->GetImage(IDB_FRAME)->GetWidth(),
						(REAL)CMemoryImage::GetInstance()->GetImage(IDB_FRAME)->GetHeight(),UnitPixel);

	//ListView网格线
	//for(int i=0;i<21;i++)
	//{
	//	RectF LineRect;
	//	LineRect.X = 0, LineRect.Y = REAL(111 + i*23), LineRect.Width = 900, LineRect.Height = 10; 

	//	graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor); 
	//	graphics.DrawImage(CMemoryImage::GetInstance()->GetImage(IDB_LINE),LineRect,5,0,
	//		(REAL)CMemoryImage::GetInstance()->GetImage(IDB_LINE)->GetWidth(),
	//		(REAL)CMemoryImage::GetInstance()->GetImage(IDB_LINE)->GetHeight(),UnitPixel);
	//}
	if(m_iListHover>=0)
	{
		RectF ListHoverRect;
		ListHoverRect.X = 0, ListHoverRect.Y = REAL(138 + m_iListHover*23), ListHoverRect.Width = 900-1, ListHoverRect.Height = 23; 
		graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor); 
		//图片我从5开始，结尾为895 拉伸是怎么算的呢？
		//先高度后宽度
		graphics.DrawImage(CMemoryImage::GetInstance()->GetImage(IDB_LISTHOVER),ListHoverRect,5,0,
			893,
			23,UnitPixel);
	}

	if(m_iListPress>=0)
	{
		RectF ListHoverRect;
		ListHoverRect.X = 0, ListHoverRect.Y = REAL(138 + m_iListPress*23), ListHoverRect.Width = 900-1, ListHoverRect.Height = 23; 
		graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor); 
		//图片我从5开始，结尾为895 拉伸是怎么算的呢？
		//先高度后宽度
		graphics.DrawImage(CMemoryImage::GetInstance()->GetImage(IDB_LISTPRESS),ListHoverRect,0,0,
			900,
			23,UnitPixel);
	}

	//ListView标题
	RectF ListViewTitleRect;
	SolidBrush ListViewTitlebrush((ARGB)Color::Black);
	CString sListViewTitleName = L"IP地址";
	TitleRect.X=10, TitleRect.Y=117; TitleRect.Width = 50; TitleRect.Height = 17;
	graphics.DrawString(sListViewTitleName.GetBuffer(), sListViewTitleName.GetLength(),&font, TitleRect,&stringFormat, &ListViewTitlebrush);

	sListViewTitleName = L"计算机名";
	TitleRect.X=150, TitleRect.Y=117; TitleRect.Width = 150; TitleRect.Height = 17;
	graphics.DrawString(sListViewTitleName.GetBuffer(), sListViewTitleName.GetLength(),&font, TitleRect,&stringFormat, &ListViewTitlebrush);

	sListViewTitleName = L"操作系统";
	TitleRect.X=400, TitleRect.Y=117; TitleRect.Width = 150; TitleRect.Height = 17;
	graphics.DrawString(sListViewTitleName.GetBuffer(), sListViewTitleName.GetLength(),&font, TitleRect,&stringFormat, &ListViewTitlebrush);

	sListViewTitleName = L"内存";
	TitleRect.X=600, TitleRect.Y=117; TitleRect.Width = 150; TitleRect.Height = 17;
	graphics.DrawString(sListViewTitleName.GetBuffer(), sListViewTitleName.GetLength(),&font, TitleRect,&stringFormat, &ListViewTitlebrush);

	sListViewTitleName = L"地理位置";
	TitleRect.X=700, TitleRect.Y=117; TitleRect.Width = 150; TitleRect.Height = 17;
	graphics.DrawString(sListViewTitleName.GetBuffer(), sListViewTitleName.GetLength(),&font, TitleRect,&stringFormat, &ListViewTitlebrush);

	//绘制底边栏信息
	SolidBrush Bottombrush((ARGB)Color::Black);
	RectF BottomRect;
	CString sBottomName = L"主程序版本：1.0.0.0000";
	BottomRect.X=3,BottomRect.Y=578,BottomRect.Width=900,TitleRect.Height=18;
	graphics.DrawString(sBottomName.GetBuffer(), sBottomName.GetLength(),&font, BottomRect,&stringFormat, &Bottombrush);

	sBottomName=L"当前时间：";
	BottomRect.X=200,BottomRect.Y=578,BottomRect.Width=80,TitleRect.Height=18;
	graphics.DrawString(sBottomName.GetBuffer(), sBottomName.GetLength(),&font, BottomRect,&stringFormat, &Bottombrush);

	sBottomName=L"在线主机：";
	BottomRect.X=600,BottomRect.Y=578,BottomRect.Width=80,TitleRect.Height=18;
	graphics.DrawString(sBottomName.GetBuffer(), sBottomName.GetLength(),&font, BottomRect,&stringFormat, &Bottombrush);

	//显示具体时间
	CString sTime;
	m_custom.MyGetTime(sTime);
	BottomRect.X=270,BottomRect.Y=578,BottomRect.Width=180,TitleRect.Height=18;
	graphics.DrawString(sTime.GetBuffer(), sTime.GetLength(),&font, BottomRect,&stringFormat, &Bottombrush);


	//绘制右上角三个按钮
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

	/*
	rtButton.left=25;
	rtButton.top=45;
	rtButton.right=25+48;
	rtButton.bottom=45+48;
	*/

	SolidBrush Funbrush((ARGB)Color::White);
	CString sFunName=L"主页";
	BottomRect.X=32,BottomRect.Y=95-7,BottomRect.Width=60,TitleRect.Height=30;
	graphics.DrawString(sFunName.GetBuffer(), sFunName.GetLength(),&font, BottomRect,&stringFormat, &Funbrush);

	sFunName=L"进程管理";
	BottomRect.X=102,BottomRect.Y=95-7,BottomRect.Width=60,TitleRect.Height=30;
	graphics.DrawString(sFunName.GetBuffer(), sFunName.GetLength(),&font, BottomRect,&stringFormat, &Funbrush);

	sFunName=L"远程CMD";
	BottomRect.X=177,BottomRect.Y=95-7,BottomRect.Width=70,TitleRect.Height=30;
	graphics.DrawString(sFunName.GetBuffer(), sFunName.GetLength(),&font, BottomRect,&stringFormat, &Funbrush);

	//绘制功能按钮,如果是经过状态 还需要再上面贴一个图片 >IDC_MAINPAGE_BT功能类按钮才需要绘制背景
	if(m_iCommandButtonHovering>=IDC_MAINPAGE_BT) //如果是鼠标盘旋
	{
		Image *pBackHover = CMemoryImage::GetInstance()->ImageFromResource(IDB_HOVER);
		m_rcBtn[m_iCommandButtonHovering].DrawButton(graphics,pBackHover);
	}

	if(m_iCommandButtonPushed>=IDC_MAINPAGE_BT) //如果是鼠标按下
	{
		Image *pBackPushed = CMemoryImage::GetInstance()->ImageFromResource(IDB_PUSHED);
		m_rcBtn[m_iCommandButtonPushed].DrawButton(graphics,pBackPushed);
	}
	CDialogEx::OnPaint();

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFireFlyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CFireFlyDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//居中显示一个800*600的对话框
	CRect DlgRect(0,0,900,600);
	CWnd::SetWindowPos(NULL,0,0,DlgRect.Width(),DlgRect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	CenterWindow(GetParent());

	CRect rcClient;
	GetClientRect(&rcClient);
	
	//创建一个圆角边框
	CRgn rgnClient;													//6位圆形角度
	rgnClient.CreateRoundRectRgn(0,0,rcClient.Width(),rcClient.Height(),6,6);
	SetWindowRgn(rgnClient,FALSE); 

	//关闭按钮的位置
	CRect rtButton;
	CRect rtTempButton;
	CGDIButton dButton;

	GetClientRect(&rtButton);
	rtTempButton=rtButton;

	//相对于对话框的按钮位置

	//根据900*600的对话框计算  关闭按钮图片188*22
	//关闭按钮
	rtButton.left=900-47;
	rtButton.top=0;
	rtButton.right=900;
	rtButton.bottom=22;

	//存在MAP里 调用拷贝构造函数，生成对象副本，这个函数走完析构临时对象，不影响map中的对象
	dButton.CreateButton(_T("关闭"),&rtButton,this,CMemoryImage::GetInstance()->GetImage(IDB_CLOSE),4,IDC_CLOSE_BT);
	m_rcBtn[IDC_CLOSE_BT]=dButton;

	////最大化按钮     最大化按钮图片 132*22
	rtButton.left=900-47-33;
	rtButton.top=0;
	rtButton.right=900-47;
	rtButton.bottom=22;

	dButton.CreateButton(_T("最大化"),&rtButton,this,CMemoryImage::GetInstance()->GetImage(IDB_SET),4,IDC_SET_BT);
	m_rcBtn[IDC_SET_BT]=dButton;

	////最小化按钮 最小化按钮图片 132*22
	rtButton.left=900-47-33-33;
	rtButton.top=0;
	rtButton.right=900-47-33;
	rtButton.bottom=22;

	dButton.CreateButton(_T("最小化"),&rtButton,this,CMemoryImage::GetInstance()->GetImage(IDB_MIN),4,IDC_MIN_BT);
	m_rcBtn[IDC_MIN_BT]=dButton;

	//主页按钮
	//所有功能按钮图片都是48*48
	rtButton.left=25;
	rtButton.top=38;
	rtButton.right=25+48;
	rtButton.bottom=38+48;
	dButton.CreateButton(_T("主页"),&rtButton,this,CMemoryImage::GetInstance()->GetImage(IDB_MAINPAGE),1,IDC_MAINPAGE_BT);
	m_rcBtn[IDC_MAINPAGE_BT]=dButton;

	rtButton.left=25+80;
	rtButton.top=38;
	rtButton.right=25+48+80;
	rtButton.bottom=38+48;
	dButton.CreateButton(_T("进程管理"),&rtButton,this,CMemoryImage::GetInstance()->GetImage(IDB_PROC_MANAGE),1,IDC_PROC_MANAGE_BT);
	m_rcBtn[IDC_PROC_MANAGE_BT]=dButton;

	rtButton.left=25+80+80;
	rtButton.top=38;
	rtButton.right=25+48+80+80;
	rtButton.bottom=38+48;
	dButton.CreateButton(_T("远程CMD"),&rtButton,this,CMemoryImage::GetInstance()->GetImage(IDB_REMOTECMD),1,IDC_REMOTECMD_BT);
	m_rcBtn[IDC_REMOTECMD_BT]=dButton;

	//下边栏时间定时器
	//SetTimer(1,1000,NULL);

	AfxBeginThread(m_ClientSock.MyControllingFunction,this);

	return 0;
}


void CFireFlyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize=sizeof(TRACKMOUSEEVENT);
		tme.dwFlags=TME_LEAVE|TME_HOVER;
		tme.hwndTrack=m_hWnd;
		tme.dwHoverTime=10;
		m_bTracking=TRUE;
	}

	CRect rtButton;
	//CRect rtWindow;
	//GetClientRect(&rtWindow);

	//测试鼠标的位置在不在按钮范围内
	int iButton = ButtonHitTest(point, rtButton);

	if(iButton>=IDC_MAINPAGE_BT)
	{
		m_iCommandButtonHovering=iButton;
	}

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
			m_iCommandButtonHovering= -1;
		}
		//现在一个按钮的进入
		if(iButton != -1)
		{
			m_iSystemButtonHovering = iButton;
			m_rcBtn[m_iSystemButtonHovering].MouseHover();
		}
	}

	//计算鼠标停留在哪一行
	if(point.y>138 && point.y<575)
	{
		CRect rcListLine;
		m_iListHover=int(point.y/23)-6;
		rcListLine.left=0;
		rcListLine.top=138 + m_iListHover*23;
		rcListLine.right=900;
		rcListLine.bottom=138+m_iListHover*23+23;
		InvalidateRect(rcListLine);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CFireFlyDlg::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_iSystemButtonHovering=-1;
	CDialogEx::OnMouseLeave();
}


void CFireFlyDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnMouseHover(nFlags, point);
}

UINT CFireFlyDlg::ButtonHitTest(CPoint pt,CRect& rcButton)
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


//
//OnEraseBkgnd :在窗口背景需要重绘时调用.
//OnPaint : 此时OnEraseBkgnd已经调用过了,所以在此响应函数体内对背景进行的操作将覆盖
//先擦除再绘制，return TRUE 则不需要擦除，不会产生白色背景闪烁
BOOL CFireFlyDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}


void CFireFlyDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rcButton;
	int iButton=this->ButtonHitTest(point,rcButton);

	//按下m_iCommandButtonPushed把按下的ID赋值
	if(iButton!=-1)
	{
		m_iCommandButtonPushed=iButton;
		this->m_rcBtn[iButton].LButtonDown();
		return;
	}
	//窗口顶部向下105的区域 为标题栏
	if(point.y < 115 /*&& point.x<787*/)   // 欺骗系统认为是在拖动标题栏 让出三个按钮的位置
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
		return;
	}

	//计算鼠标停按下哪一行
	if(point.y>138 && point.y<575)
	{
		CRect rcListLine;
		m_iListPress=int(point.y/23)-6;
		rcListLine.left=0;
		rcListLine.top=138 + m_iListPress*23;
		rcListLine.right=900;
		rcListLine.bottom=138+m_iListPress*23+23;
		InvalidateRect(rcListLine);
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}


void CFireFlyDlg::OnLButtonUp(UINT nFlags, CPoint point)
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


BOOL CFireFlyDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case IDC_CLOSE_BT:
			{
				SendMessage( WM_SYSCOMMAND,SC_CLOSE);
			}
			break;
		case IDC_SET_BT:
			{
				//SendMessage( WM_SYSCOMMAND,SC_MAXIMIZE);
			}
			break;
		case IDC_MIN_BT:
			{
				SendMessage( WM_SYSCOMMAND,SC_MINIMIZE);
			}
			break;
		case IDC_MAINPAGE_BT:
			{
				MessageBoxW(L"主页",0,0);
			}
			break;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}


void CFireFlyDlg::OnTimer(UINT_PTR nIDEvent)
{
	this->InvalidateRect(m_rcTime);
	//bUpdateTime=TRUE;
	CDialogEx::OnTimer(nIDEvent);
}
