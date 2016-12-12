#pragma once
#import "C:\\Windows\\System32\\WinHttp.dll" no_namespace
class CSendSms
{
public:
	CSendSms(void);
	~CSendSms(void);
	int Send(CString PhoneNumber);
	BOOL Action1();
	BOOL Action2();
private:
	IWinHttpRequestPtr pHttpReq;
	CString PhoneNumber;
};

