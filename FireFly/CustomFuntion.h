#pragma once
class CCustomFuntion
{
public:
	CCustomFuntion(void);
	~CCustomFuntion(void);
	int MyGetTime(CString &CurrentTime);
	CString MyGetOS(int id);
	CString MyCharToWide(char * ch);
	char* MyWideToChar(CString st);
};

