#pragma once
#include <map>
#include "GDIButton.h"

// CMessageBox 对话框

class CMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageBox)

public:
	//CMessageBox(CWnd* pParent=NULL);   // 标准构造函数
	CMessageBox(UINT weight,UINT height,CString str,CString strTitle);   
	virtual ~CMessageBox();

// 对话框数据
	enum { IDD = IDD_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	UINT weight;
	UINT height;
	CString str;
	CString strTitle;
	//系统按钮坐标
	int m_iSystemButtonHovering;
	map<UINT,CGDIButton> m_rcBtn;
	int m_iCommandButtonHovering;//功能按钮经过的ID 默认-1
	int m_iCommandButtonPushed;  //功能按钮按下的ID 默认-1
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	UINT ButtonHitTest(CPoint pt,CRect& rcButton);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnOKBTN();
};
