
// FireFlyDlg.h : 头文件
//

#pragma once
#include <map>
#include "GDIButton.h"
#include "CustomFuntion.h"
#include "ClientSock.h"

#include "Process.h"
using namespace std;

#define WM_ONLINE (WM_USER+100)   //上线
#define WM_OFFLINE (WM_USER+101)  //下线

// CFireFlyDlg 对话框
class CFireFlyDlg : public CDialogEx
{
// 构造
public:
	CFireFlyDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CFireFlyDlg();	
// 对话框数据
	enum { IDD = IDD_FIREFLY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void LoadPng(UINT ID,CString path);
private:
	ULONG_PTR	m_GdiplusToken; //GDI+ token
	//公共图片集合
	map<UINT,Image*> m_Image;
	//系统按钮坐标
	map<UINT,CGDIButton> m_rcBtn;
	//判断MouseMove
	BOOL m_bTracking;
	int m_iSystemButtonHovering;//原来鼠标停在位置的控件ID 不在控件内则为-1
	int m_iCommandButtonHovering;//功能按钮经过的ID 默认-1
	int m_iCommandButtonPushed;  //功能按钮按下的ID 默认-1
	int m_iListHover; //鼠标经过的Listview项目索引
	int m_iListPress;//鼠标按下的Listview项目索引
	CCustomFuntion m_custom;
	CRect m_rcTime;
	BOOL m_bUpdataListView;//是否有主机上下线，需要重绘

public:
	CClientSock m_ClientSock;//socket管理类
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	UINT ButtonHitTest(CPoint pt,CRect& rcButton);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT MyOnline(WPARAM wParam,LPARAM lParam); //上线
	LRESULT MyOffline(WPARAM wParam,LPARAM lParam);//下线

	afx_msg void OnProcess();//进程管理
	void MyDeleteProcess();
public:
	CProcess* m_Process;    //进程管理对象
};
