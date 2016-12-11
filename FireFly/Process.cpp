// Process.cpp : 实现文件
//

#include "stdafx.h"
#include "FireFly.h"
#include "Process.h"
#include "afxdialogex.h"
#include "FireFlyDlg.h"

// CProcess 对话框

IMPLEMENT_DYNAMIC(CProcess, CDialogEx)

CProcess::CProcess(CWnd* pParent,SOCKET sk)
	: CDialogEx(CProcess::IDD, pParent)
{
	CurrentSock=sk;
	pDlg = (CFireFlyDlg*)AfxGetApp()->m_pMainWnd;
}

CProcess::~CProcess()
{
}

void CProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_LIST, m_ProcessList);
	DDX_Control(pDX, IDC_MODULE_LIST, m_ModuleList);
}


BEGIN_MESSAGE_MAP(CProcess, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_PROCESS_LIST, &CProcess::OnNMRClickProcessList)
	ON_COMMAND(ID_EXITPROCESS, &CProcess::OnExitprocess)
	ON_COMMAND(ID_MODULELIST, &CProcess::OnModulelist)
	ON_COMMAND(ID_REFRESH, &CProcess::OnRefresh)
END_MESSAGE_MAP()


// CProcess 消息处理程序


BOOL CProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ProcessList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ProcessList.InsertColumn(0,_T("进程名称"),0,120,-1);
	m_ProcessList.InsertColumn(0,_T("PID"),0,120,-1);
	m_ProcessList.InsertColumn(0,_T("程序路径"),0,120,-1);
	m_ModuleList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ModuleList.InsertColumn(0,_T("模块名称"),0,120,-1);
	m_ModuleList.InsertColumn(0,_T("模块路径"),0,120,-1);

	pDlg->m_ClientSock.MySendCommand(CurrentSock,MYGETPROCESS,NULL,0,PROCESSLIST);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProcess::ProcessComment(COMMAND cmd)
{
	switch(cmd.fID)
	{
	case PROCESSLIST:
		MyShowList(cmd);
		break;
	case MODULELIST:
		MyShowModle(cmd);
		break;
	case EXITPROCESS:
		AfxMessageBox(L"进程已结束，请刷新列表！");
		break;
	default:
		break;
	}
}

void CProcess::MyShowList(COMMAND cmd)
{
	PROCESSINFO proinfo;
	ZeroMemory(&proinfo,sizeof(PROCESSINFO));
	memcpy(&proinfo,cmd.buffer,sizeof(PROCESSINFO));
	int idx=m_ProcessList.GetItemCount();
	m_ProcessList.InsertItem(idx,proinfo.pName);
	CString Pid;
	Pid.Format(_T("%d"),proinfo.Pid);
	m_ProcessList.SetItemText(idx,1,Pid);
	m_ProcessList.SetItemText(idx,2,proinfo.pPath);
}
void CProcess::OnCancel()
{
	CDialogEx::OnCancel();
	pDlg->MyDeleteProcess();

}

void CProcess::MyShowModle(COMMAND cmd)
{
	wchar_t StartPackage[]={L"19880620"};
	WCHAR x64Process[5]={L"5338"};
	wchar_t tem[MAX_PATH];
	memcpy(tem,cmd.buffer,MAX_PATH);

	if(memcmp(tem,x64Process,10)==0)
	{
		AfxMessageBox(L"该进程为64位进程或不存在，无法查看模块！");
		m_ModuleList.DeleteAllItems();
		return;
	}
	if(memcmp(tem,StartPackage,18)==0)
	{
		m_ModuleList.DeleteAllItems();
	}
	wchar_t *pos = wcsrchr(tem,_T('\\'));
	pos++;

	int idx = m_ModuleList.GetItemCount();

	m_ModuleList.InsertItem(idx,pos);
	m_ModuleList.SetItemText(idx,1,tem);
}

void CProcess::OnNMRClickProcessList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int idx=pNMItemActivate->iItem;
	if(idx<0)
	{
		return;
	}
	CMenu menu,*pMenu=NULL;
	menu.LoadMenuW(IDR_PROCESS);
	pMenu=menu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);

	*pResult = 0;
}


void CProcess::OnExitprocess()
{
	int idx = m_ProcessList.GetSelectionMark();
	wchar_t pid[20] ={0};
	m_ProcessList.GetItemText(idx,1,pid,20);
	pDlg->m_ClientSock.MySendCommand(CurrentSock,MYGETPROCESS,(char*)pid,20,EXITPROCESS);
}


void CProcess::OnModulelist()
{
	int idx = m_ProcessList.GetSelectionMark();
	wchar_t pid[20] ={0};
	m_ProcessList.GetItemText(idx,1,pid,20);
	pDlg->m_ClientSock.MySendCommand(CurrentSock,MYGETPROCESS,(char*)pid,20,MODULELIST);
}


void CProcess::OnRefresh()
{
	m_ProcessList.DeleteAllItems();
	pDlg->m_ClientSock.MySendCommand(CurrentSock,MYGETPROCESS,NULL,0,PROCESSLIST);
}
