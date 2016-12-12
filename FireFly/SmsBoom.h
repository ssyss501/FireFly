#pragma once
#include "sys.h"
#include "ClientSock.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <map>
#include "GDIButton.h"

// CSmsBoom 对话框
class CFireFlyDlg;
class CSmsBoom : public CDialogEx
{
	DECLARE_DYNAMIC(CSmsBoom)

public:
	CSmsBoom(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~CSmsBoom();

// 对话框数据
	enum { IDD = IDD_SMSBOOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CFireFlyDlg *pDlg;
	//系统按钮坐标
	int m_iSystemButtonHovering;
	map<UINT,CGDIButton> m_rcBtn;
	int m_iCommandButtonHovering;//功能按钮经过的ID 默认-1
	int m_iCommandButtonPushed;  //功能按钮按下的ID 默认-1
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnCancel();

	CEdit m_PhoneNumber;
	afx_msg void OnBnClickedSend();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	UINT ButtonHitTest(CPoint pt,CRect& rcButton);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnOKBTN();
};
