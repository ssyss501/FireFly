#pragma once
#include "afxcmn.h"


// CProcess 对话框

class CFireFlyDlg;

class CProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent,SOCKET sk);   // 标准构造函数
	virtual ~CProcess();

// 对话框数据
	enum { IDD = IDD_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	SOCKET CurrentSock;
	CFireFlyDlg *pDlg;
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ProcessList;
	CListCtrl m_ModuleList;
	virtual void OnCancel();
};
