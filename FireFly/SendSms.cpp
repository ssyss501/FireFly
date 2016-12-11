#include "StdAfx.h"
#include "SendSms.h"


SendSms::SendSms(void)
{
}


SendSms::~SendSms(void)
{
}

UINT SendSms::Send(CString PhoneNumber)
{


	IWinHttpRequestPtr pHttpReq = NULL;
	HRESULT hr = pHttpReq.CreateInstance(__uuidof(WinHttpRequest));
	if (FAILED(hr)) return 0;

	hr = pHttpReq->Open(_T("POST"), _T("http://www.idc3389.com/user/userlogin.asp"));
	if (FAILED(hr)) return 0;

	pHttpReq->SetRequestHeader(_T("Content-Type"), _T("application/x-www-form-urlencoded"));
	CString strBody;

	COleVariant varBody;
	varBody = strBody;
	hr = pHttpReq->Send(varBody);
	if (FAILED(hr)) return 0;


	return 0;
}