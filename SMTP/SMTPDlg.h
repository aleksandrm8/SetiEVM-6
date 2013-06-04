
// SMTPDlg.h : ���� ���������
//

#pragma once
#include "afxwin.h"
#include "winsock2.h"
#include <vector>
#include <string>


// ���������� ���� CSMTPDlg
class CSMTPDlg : public CDialogEx
{
// ��������
public:
	CSMTPDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_SMTP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
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

	// ��������� ���������.
	struct SendDataStruct {
		CString From;
		CString To;
		CString Head;
		CString Text;
	};
	std::vector <SendDataStruct> SendData;

	// ��������� �������� SMTP.
	struct OptionsStruct {
		CString Server;
		int Port;
		CString Login;
		CString Password;
		CString Sender;
	};
	struct OptionsStruct Options;

	// ���� �������.
	int Process;

	// ����� ���� ��������.
	afx_msg void OnBnClickedBoptions();
	// ���������� � �������� ������.
	afx_msg void OnBnClickedBsend();

	// �������� ������.
	static AFX_THREADPROC Sending(LPVOID pParam);

	// ������� ����� ���������.
	void PrintMessage(CString Message);
	static void PrintMessage(CSMTPDlg* Dlg, CString Message);
	// ������� ������� ����� � CString.
	static CString intToCStr(int value);
	// ������� ������� CString � String.
	static std::string CStrToStr(CString Value);
	// ������� ��������� IP �� �����.
	static DWORD hNameToIP(CString Name);
	// ������� ��������� ���� ����������.
	static int GetRCode(char buff[]);
	// ������� ���� ���� ����������.
	static int RecvRCode(CSMTPDlg* Dlg, SOCKET s, int FirstValidCode, int SecondValidCode);
	// ������� �������� �������.
	static int SendQuery(SOCKET s, CSMTPDlg * Dlg, std::string buffer);
};
