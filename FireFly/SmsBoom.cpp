// SmsBoom.cpp : 实现文件
//

#include "stdafx.h"
#include "FireFly.h"
#include "SmsBoom.h"
#include "afxdialogex.h"
#include "FireFlyDlg.h"

// CSmsBoom 对话框

IMPLEMENT_DYNAMIC(CSmsBoom, CDialogEx)

CSmsBoom::CSmsBoom(CWnd* pParent)
	: CDialogEx(CSmsBoom::IDD, pParent)
{
	//	pDlg= (CFireFlyDlg*)AfxGetMainWnd();完成同样功能
	pDlg = (CFireFlyDlg*)AfxGetApp()->m_pMainWnd;

}

CSmsBoom::~CSmsBoom()
{
}

void CSmsBoom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PhoneNumber, m_PhoneNumber);
}


BEGIN_MESSAGE_MAP(CSmsBoom, CDialogEx)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SEND, &CSmsBoom::OnBnClickedSend)
END_MESSAGE_MAP()


// CSmsBoom 消息处理程序


//void CSmsBoom::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnMouseMove(nFlags, point);
//}


//void CSmsBoom::OnPaint()
//{
//	CClientDC dc(this);
//	CMemoryDC mDC(&dc);
//
//	CRect rcClient;
//	GetClientRect(&rcClient);
//
//	// 获取重绘矩形位置
//	CRect rcPaint;
//	mDC.GetClipBox(&rcPaint);
//	Gdiplus::Graphics graphics(mDC.m_hDC);
////	graphics.
//	StringFormat stringFormat;
//	LOGFONT lfFont;
//	memset(&lfFont, 0, sizeof(lfFont));
//	lfFont.lfHeight = -13;
//	lfFont.lfWeight = 300;
//	lstrcpy(lfFont.lfFaceName, _T("微软雅黑"));
//	Gdiplus::Font font(mDC.GetSafeHdc(),&lfFont);
//
//	//所有的图片都在类里面CMemoryImage，
//	//绘制背景
//	graphics.DrawImage(CMemoryImage::GetInstance()->GetImage(IDB_BOOMBKG),0, 0, rcClient.Width(), rcClient.Height());
//}


//BOOL CSmsBoom::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	return CDialogEx::OnEraseBkgnd(pDC);
//}


//void CSmsBoom::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnLButtonDown(nFlags, point);
//}
//
//
//void CSmsBoom::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnLButtonUp(nFlags, point);
//}


void CSmsBoom::OnCancel()
{
	CDialogEx::OnCancel();
	pDlg->MyDeleteSmsBoom();
}


int CSmsBoom::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CenterWindow(GetParent());
	return 0;
}


void CSmsBoom::OnBnClickedSend()
{
	// TODO: 在此添加控件通知处理程序代码
}



BOOL CSmsBoom::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->m_PhoneNumber.SetLimitText(11);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
