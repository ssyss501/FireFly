#include "StdAfx.h"
#include "SendSms.h"


CSendSms::CSendSms(void)
{
	pHttpReq.CreateInstance(__uuidof(WinHttpRequest));
}


CSendSms::~CSendSms(void)
{
}

int CSendSms::Send(CString PhoneNumber)
{
	int iCount=0;
	this->PhoneNumber=PhoneNumber;
	
	if(Action1())iCount++;
	if(Action2())iCount++;
	return iCount;
}

BOOL CSendSms::Action1()
{
	HRESULT hr = pHttpReq->Open(_T("POST"), _T("https://operation.cardniu.com/cardniuoperation-webservice/operationloan/service/sendVcode.action "));
	if (FAILED(hr)) return FALSE;

	pHttpReq->SetRequestHeader(_T("Content-Type"), _T("application/x-www-form-urlencoded"));
	CString strBody=L"phone="+PhoneNumber;

	COleVariant varBody;
	varBody = strBody;
	hr = pHttpReq->Send(varBody);
	if (FAILED(hr)) return FALSE;
	return TRUE;
}


BOOL CSendSms::Action2()
{
	HRESULT hr = pHttpReq->Open(_T("POST"), _T("https://operation.cardniu.com/suishoudai-webservice/phoneService/sendVcode.do"));
	if (FAILED(hr)) return FALSE;

	pHttpReq->SetRequestHeader(_T("Content-Type"), _T("application/x-www-form-urlencoded"));
	CString strBody=L"phone="+PhoneNumber+L"&type=loan_suishoudai";

	COleVariant varBody;
	varBody = strBody;
	hr = pHttpReq->Send(varBody);
	if (FAILED(hr)) return FALSE;
	return TRUE;
}