
// SMTPDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"
#include "winsock2.h"
#include <vector>
#include <string>


// диалоговое окно CSMTPDlg
class CSMTPDlg : public CDialogEx
{
// Создание
public:
	CSMTPDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_SMTP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit CFrom;
	CEdit CTo;
	CEdit CHead;
	CEdit CText;
	CEdit CInfo;

	// Структура сообщений.
	struct SendDataStruct {
		CString From;
		CString To;
		CString Head;
		CString Text;
	};
	std::vector <SendDataStruct> SendData;

	// Структура настроек SMTP.
	struct OptionsStruct {
		CString Server;
		int Port;
		CString Login;
		CString Password;
		CString Sender;
	};
	struct OptionsStruct Options;

	// Флаг очереди.
	int Process;

	// Вызов окна настроек.
	afx_msg void OnBnClickedBoptions();
	// Подготовка к отправке письма.
	afx_msg void OnBnClickedBsend();

	// Отправка письма.
	static AFX_THREADPROC Sending(LPVOID pParam);

	// Удобный вывод сообщения.
	void PrintMessage(CString Message);
	static void PrintMessage(CSMTPDlg* Dlg, CString Message);
	// Удобный перевод числа в CString.
	static CString intToCStr(int value);
	// Удобный перевод CString в String.
	static std::string CStrToStr(CString Value);
	// Удобное получение IP из имени.
	static DWORD hNameToIP(CString Name);
	// Удобное выделение кода результата.
	static int GetRCode(char buff[]);
	// Удобный приём кода результата.
	static int RecvRCode(CSMTPDlg* Dlg, SOCKET s, int FirstValidCode, int SecondValidCode);
	// Удобная отправка запроса.
	static int SendQuery(SOCKET s, CSMTPDlg * Dlg, std::string buffer);
};
