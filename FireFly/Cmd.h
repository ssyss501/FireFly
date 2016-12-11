#pragma once
//#include "resource.h"
#include "sys.h"
//#include "ClientSock.h"
//#include "afxwin.h"
// CCmd 对话框
class CFireFlyDlg;

class CCmd : public CDialogEx
{
	DECLARE_DYNAMIC(CCmd)

public:
	CCmd(CWnd* pParent,SOCKET sk);   // 标准构造函数
	virtual ~CCmd();
	virtual void OnCancel();
// 对话框数据
	enum { IDD = IDD_CMDSHELL };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	SOCKET CurrentSock;
	CFireFlyDlg *pDlg;
public:
	virtual BOOL OnInitDialog();
	void CmdCommand(COMMAND cmd);
	CEdit m_CmdShow;
	void MyShowText(COMMAND cmd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
