#pragma once
#include "sys.h"
//class CHeiHuClientView;

class ServItem
{
public:
	SOCKET sk;
	CString ip;
	CString cmpname;
	DWORD os;
	UINT memsize;
};

extern CArray<ServItem,ServItem&>m_ServArray;


class CClientSock
{
public:
	CClientSock(void);
	~CClientSock(void);
	static UINT __cdecl MyControllingFunction(LPVOID pParam);
	BOOL MySelect(SOCKET sk);
	BOOL MySendCommand(SOCKET sk,int ID,char* buffer,int size,int fID=0);
	BOOL MyReciveCommand(SOCKET sk,COMMAND &cmd);
	static UINT __cdecl MyRecvFunction(LPVOID pParam);
	bool PanDuan(CString ip, int& idx);
};

