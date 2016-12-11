#pragma once
#import "C:\\Windows\\System32\\WinHttp.dll" no_namespace
class SendSms
{
public:
	SendSms(void);
	~SendSms(void);
	UINT Send(CString PhoneNumber);
private:
	CString PhoneNumber;
};

