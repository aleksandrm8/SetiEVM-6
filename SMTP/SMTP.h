
// SMTP.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CSMTPApp:
// � ���������� ������� ������ ��. SMTP.cpp
//

class CSMTPApp : public CWinApp
{
public:
	CSMTPApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CSMTPApp theApp;