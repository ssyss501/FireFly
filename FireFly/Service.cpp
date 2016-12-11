// SvcManage.cpp : 实现文件
//

#include "stdafx.h"
#include "FireFly.h"
#include "Service.h"
#include "afxdialogex.h"
#include "FireFlyDlg.h"

#include <WinSvc.h>
// CService 对话框

IMPLEMENT_DYNAMIC(CService, CDialogEx)

CService::CService(CWnd* pParent,SOCKET sk)
	: CDialogEx(CService::IDD, pParent)
{
	CurrentSock=sk;
	pDlg = (CFireFlyDlg*)AfxGetApp()->m_pMainWnd;
}

CService::~CService()
{
}

void CService::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICELIST, m_ServiceList);
}


BEGIN_MESSAGE_MAP(CService, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_SERVICELIST, &CService::OnNMRClickServicelist)
	ON_COMMAND(ID_REFRESHSERVICE, &CService::OnRefreshservice)
	ON_COMMAND(ID_STARTSERVICE, &CService::OnStartservice)
END_MESSAGE_MAP()


// CService 消息处理程序


void CService::OnCancel()
{
	CDialogEx::OnCancel();
	pDlg->MyDeleteService();
}


BOOL CService::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ServiceList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//分成四个栏目
	m_ServiceList.InsertColumn(0,L"名称",0,120,-1);
	m_ServiceList.InsertColumn(1,L"显示",0,120,-1);
	m_ServiceList.InsertColumn(2,L"状态",0,120,-1);
	m_ServiceList.InsertColumn(3,L"类型",0,120,-1);
	m_ServiceList.InsertColumn(4,L"描述",0,120,-1);
	m_ServiceList.InsertColumn(5,L"路径",0,120,-1);
	pDlg->m_ClientSock.MySendCommand(CurrentSock,MYSVCMANAGE,NULL,0,MYGETSVCLIST);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CService::MySVCCommand(COMMAND cmd)
{
	WCHAR temp[MAX_PATH]={0};
	switch(cmd.fID)
	{
	case MYGETSVCLIST:
		ShowSVCList(cmd);
		break;
	case SVCMSG:		
		memcpy(temp,cmd.buffer,MAX_PATH);
		AfxMessageBox(temp);
		break;
	default:
		break;
	}
}


void CService::ShowSVCList(COMMAND cmd)
{
	SVICELISTINFO svcin;
	ZeroMemory(&svcin,sizeof(SVICELISTINFO));
	memcpy(&svcin,cmd.buffer,sizeof(SVICELISTINFO));
	int idx = m_ServiceList.GetItemCount();
	m_ServiceList.InsertItem(idx,svcin.ServiceName); //名称
	m_ServiceList.SetItemText(idx,1,svcin.DisplayName);//显示
	m_ServiceList.SetItemText(idx,2,MyGetCurrentState(svcin.CurrentState));//状态
	m_ServiceList.SetItemText(idx,3,MyGetStartType(svcin.stype));//类型
	m_ServiceList.SetItemText(idx,4,svcin.Description);//描述
	m_ServiceList.SetItemText(idx,5,svcin.BinPath);//路径
}

CString CService::MyGetCurrentState(DWORD State)
{
	CString sst;
	switch (State)
	{
	case SERVICE_RUNNING:
		sst = _T("已启动");
		break;
	case SERVICE_PAUSED:
		sst = _T("暂停");
		break;
	case SERVICE_PAUSE_PENDING:
		sst = _T("正在暂停中...");
		break;
	case SERVICE_START_PENDING:
		sst = _T("正在启动中..");
		break;
	case SERVICE_STOPPED:
		sst = _T("");
		break;
	case SERVICE_STOP_PENDING:
		sst = _T("正在停止中..");
		break;
	default:
		sst = _T("");
		break;
	}
	return sst;
}
CString CService::MyGetStartType(DWORD StartType)
{
	CString sttr;
	switch(StartType)
	{
	case SERVICE_AUTO_START:
		sttr = _T("自动");
		break;
	case SERVICE_DEMAND_START:
		sttr = _T("手动");
		break;
	case SERVICE_DISABLED:
		sttr = _T("已禁用");
		break;
	default:
		sttr = _T("");
		break;
	}
	return sttr;
}

void CService::OnNMRClickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int idx=pNMItemActivate->iItem;
	if(idx<0)
	{
		return;
	}
	CMenu menu,*pMenu=NULL;
	menu.LoadMenuW(IDR_SERVICE);
	pMenu=menu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);

	*pResult = 0;
	*pResult = 0;
}


void CService::OnRefreshservice()
{
	this->m_ServiceList.DeleteAllItems();
	pDlg->m_ClientSock.MySendCommand(CurrentSock,MYSVCMANAGE,NULL,0,MYGETSVCLIST);
}


void CService::OnStartservice()
{
	int idx = m_ServiceList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_ServiceList.GetItemText(idx,0,svcname,MAX_PATH);
	pDlg->m_ClientSock.MySendCommand(CurrentSock,MYSVCMANAGE,(char*)svcname,MAX_PATH,MYSTARTSVC);
}
