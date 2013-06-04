
// SMTP.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CSMTPApp:
// О реализации данного класса см. SMTP.cpp
//

class CSMTPApp : public CWinApp
{
public:
	CSMTPApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CSMTPApp theApp;