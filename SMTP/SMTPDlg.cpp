
// SMTPDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "SMTP.h"
#include "SMTPDlg.h"
#include "afxdialogex.h"
#include "winsock2.h"
#include "Base64Coder.h"
#include <vector>
#include <Windows.h>
#include <string>
#include <atlconv.h>
#include <math.h>
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()





// диалоговое окно COptionsDlg

class COptionsDlg : public CDialogEx
{
public:
	COptionsDlg();

	// Данные диалогового окна
	enum { IDD = DOptions };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
	CEdit CServer;
	CEdit CPort;
	CEdit CLogin;
	CEdit CPassword;
	CEdit CSender;

	// Структура конфигурации.
	struct OptionsStruct {
		CString Server;
		int Port;
		CString Login;
		CString Password;
		CString Sender;
	};
	struct OptionsStruct Options;
	afx_msg void OnBnClickedOk();
	
};

COptionsDlg::COptionsDlg() : CDialogEx(COptionsDlg::IDD)
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, SServ, CServer);
	DDX_Control(pDX, SPort, CPort);
	DDX_Control(pDX, SLogin, CLogin);
	DDX_Control(pDX, SPassword, CPassword);
	DDX_Control(pDX, SSender, CSender);

	// Заполнение данными.
	CServer.SetWindowTextW(Options.Server);
	
	CString Port;
	Port.Format(L"%d",Options.Port);
	CPort.SetWindowTextW(Port);
	
	CLogin.SetWindowTextW(Options.Login);
	CPassword.SetWindowTextW(Options.Password);
	CSender.SetWindowTextW(Options.Sender);
}

BEGIN_MESSAGE_MAP(COptionsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &COptionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void COptionsDlg::OnBnClickedOk()
{
	// Сохраняем данные для передачи.
	CServer.GetWindowTextW(Options.Server);

	CString Port;
	CPort.GetWindowTextW(Port);
	USES_CONVERSION;
	Options.Port = atoi(T2A((LPCTSTR)Port));
	
	CLogin.GetWindowTextW(Options.Login);
	CPassword.GetWindowTextW(Options.Password);
	CSender.GetWindowTextW(Options.Sender);

	COptionsDlg::OnOK();
}




// диалоговое окно CSMTPDlg




CSMTPDlg::CSMTPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSMTPDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMTPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, SFrom, CFrom);
	DDX_Control(pDX, STo, CTo);
	DDX_Control(pDX, SHead, CHead);
	DDX_Control(pDX, AText, CText);
	DDX_Control(pDX, AInfo, CInfo);
}

BEGIN_MESSAGE_MAP(CSMTPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BOptions, &CSMTPDlg::OnBnClickedBoptions)
	ON_BN_CLICKED(BSend, &CSMTPDlg::OnBnClickedBsend)
END_MESSAGE_MAP()


// обработчики сообщений CSMTPDlg

BOOL CSMTPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	Options.Server   = L"";
	Options.Port     = 25;
	Options.Login    = L"";
	Options.Password = L"";
	Options.Sender   = L"";
	CFrom.SetWindowTextW(Options.Sender);

	Process = 0;

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CSMTPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CSMTPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CSMTPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// Вызов окна настроек.
void CSMTPDlg::OnBnClickedBoptions()
{
	COptionsDlg OptionsDlg;

	// Заполнение форм данными.
	OptionsDlg.Options.Server   = Options.Server;
	OptionsDlg.Options.Port     = Options.Port;
	OptionsDlg.Options.Login    = Options.Login;
	OptionsDlg.Options.Password = Options.Password;
	OptionsDlg.Options.Sender   = Options.Sender;

	// Вызов диалога.
	int RCode = -1;
	RCode = OptionsDlg.DoModal();

	// Обработка результатов.
	switch(RCode) {

	case -1: 
		// Окно не открылось.
		PrintMessage(L"Не могу открыть окно!");
		break;

	case IDABORT:
		// Окно закрыто.
		PrintMessage(L"Настройки не сохранены!");
		break;

	case IDOK:
		// Применение настроек.
		Options.Server   = OptionsDlg.Options.Server;
		Options.Port     = OptionsDlg.Options.Port;
		Options.Login    = OptionsDlg.Options.Login;
		Options.Password = OptionsDlg.Options.Password;
		Options.Sender   = OptionsDlg.Options.Sender;
		CFrom.SetWindowTextW(Options.Sender);
		PrintMessage(L"Настройки сохранены.");
		break;

	case IDCANCEL:
		// Отмена.
		PrintMessage(L"Настройки не сохранены!");
		break;

	default:
		// Не определённая ошибка диалога настроек.
		PrintMessage(L"Неизвестная ошибка!");
		break;
	};
}


// Подготовка к отправке письма.
void CSMTPDlg::OnBnClickedBsend()
{
	// Добавляем новую запись очередь.
	struct SendDataStruct NewMessage;

	CFrom.GetWindowTextW(NewMessage.From);
	//CFrom.SetWindowTextW(L"");

	CTo.GetWindowTextW(NewMessage.To);
	CTo.SetWindowTextW(L"");

	CHead.GetWindowTextW(NewMessage.Head);
	CHead.SetWindowTextW(L"");

	CText.GetWindowTextW(NewMessage.Text);
	CText.SetWindowTextW(L"");

	SendData.push_back(NewMessage);

	// Вызываем поток
	AfxBeginThread((AFX_THREADPROC)Sending, this);
}


// Отправка письма.
AFX_THREADPROC CSMTPDlg::Sending(LPVOID pParam)
{
	// Получаем указатель.
	CSMTPDlg *Dlg = (CSMTPDlg*)pParam;

	// Проверяем, не запущен ли уже процесс.
	if(Dlg->Process == 0) {
		Dlg->Process = 1;

		// Определяем переменную для буфера.
		std::string buff;
		int check = 0;
		Base64Coder Coder;

		// Проверяем наличие в очереди заданий.
		while(Dlg->SendData.size() > 0) {
			struct SendDataStruct *Data = &(Dlg->SendData.front());

			// Инициализация Winsock 2.2
			WSADATA wsadata;
			if(WSAStartup(MAKEWORD(2, 2), &wsadata)) {
				PrintMessage(Dlg, L"Ошибка инициализации сокета №" + intToCStr(WSAGetLastError()));
				check = 1;
			}

			// Проверка версии Winsock.
			if(check == 0) {
				if(LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) { 
					PrintMessage(Dlg, L"Ошибка инициализации сокета №" + intToCStr(WSAGetLastError()));
					check = 1;
				}
			}

			// Регистрация сокета.
			SOCKET s;
			if(check == 0) {
				if(INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0))) {
					PrintMessage(Dlg, L"Ошибка инициализации сокета №" + intToCStr(WSAGetLastError()));
					check = 1;
				}
			}

			// Подключение к серверу.
			sockaddr_in SendAddr;
			ZeroMemory(&SendAddr, sizeof(SendAddr));

			if(check == 0) {

				// Получаем IP-адрес из имени хоста.
				SendAddr.sin_family           = AF_INET;
				SendAddr.sin_addr.S_un.S_addr = hNameToIP(Dlg->Options.Server);
				SendAddr.sin_port             = htons(Dlg->Options.Port);

				// Подключение к серверу.
				PrintMessage(Dlg, L"Подключение к серверу " + Dlg->Options.Server +
					L" [" + CString(inet_ntoa(SendAddr.sin_addr)) + L"]...");

				if(connect(s, (struct sockaddr *) &SendAddr, sizeof(SendAddr)) < 0) {
					PrintMessage(Dlg, L"Ошибка соединения с сервером SMTP! (№" + intToCStr(WSAGetLastError()) + L")");
					check = 1;
				}
			}
			if(check == 0) {
				PrintMessage(Dlg, L"Соединение установлено!");

				// Получаем приглашение.
				check = RecvRCode(Dlg, s, 220, 0);
			}

			// Здороваемся с сервером (EHLO).
			if(check == 0) {
				buff = "EHLO " + CStrToStr(Dlg->Options.Login) + "\r\n";
				check = SendQuery(s, Dlg, buff);
			}
			if(check == 0)
				check = RecvRCode(Dlg, s, 250, 0);

			// Переходим к авторизации.
			if(check == 0)
				check = SendQuery(s, Dlg, "AUTH LOGIN\r\n");
			if(check == 0)
				check = RecvRCode(Dlg, s, 334, 0);
			
			// Авторизуемся.
			if(check == 0) {
				buff = Coder.base64_encode((const unsigned char*)CStrToStr(Dlg->Options.Login).c_str(),
					Dlg->Options.Login.GetLength()) + "\r\n";
				check = SendQuery(s, Dlg, buff);
			}
			if(check == 0)
				check = RecvRCode(Dlg, s, 334, 0);
			
			if(check == 0) {
				buff = Coder.base64_encode((const unsigned char*)CStrToStr(Dlg->Options.Password).c_str(),
					Dlg->Options.Password.GetLength()) + "\r\n";
				check = SendQuery(s, Dlg, buff);
			}
			if(check == 0)
				check = RecvRCode(Dlg, s, 235, 0);
			
			// Указываем отправителя.
			if(check == 0) {
				buff = "MAIL FROM:" + CStrToStr(Dlg->SendData[0].From) + "\r\n";
				check = SendQuery(s, Dlg, buff);
			}
			if(check == 0)
				check = RecvRCode(Dlg, s, 250, 0);

			// Указываем получателя.
			if(check == 0) {
				buff = "RCPT TO:" + CStrToStr(Dlg->SendData[0].To) + "\r\n";
				check = SendQuery(s, Dlg, buff);
			}
			if(check == 0)
				check = RecvRCode(Dlg, s, 250, 251);
			
			// Указываем получателя.
			if(check == 0)
				check = SendQuery(s, Dlg, "DATA\r\n");
			if(check == 0)
				check = RecvRCode(Dlg, s, 354, 0);

			// Указываем отправителя.
			if(check == 0) {
				buff = "From:" + CStrToStr(Dlg->SendData[0].From) + "\r\n";
				check = SendQuery(s, Dlg, buff);
			}

			// Указываем получателя.
			if(check == 0) {
				buff = "To:" + CStrToStr(Dlg->SendData[0].To) + "\r\n";
				check = SendQuery(s, Dlg, buff);
			}

			// Указываем заголовок.
			if(check == 0) {
				/*
				buff = "Subject: " +
					Coder.base64_encode((const unsigned char*)CStrToStr(Dlg->SendData[0].Head).c_str(),
					Dlg->SendData[0].Head.GetLength()) + "\r\n";
				for(int c = 0; c < buff.size(); c++) {
					if(buff[c] == '+') buff[c] = '_';
					if(buff[c] == '%') buff[c] = '=';
				}
				*/
				buff = "Subject: " + CStrToStr(Dlg->SendData[0].Head) + "\r\n";
				check = SendQuery(s, Dlg, buff);
			}

			// Указываем данные.
			if(check == 0) {
				buff = "\r\n" + CStrToStr(Dlg->SendData[0].Text) + "\r\n.\r\n";
				check = SendQuery(s, Dlg, buff);
			}
			if(check == 0)
				check = RecvRCode(Dlg, s, 250, 0);

			// Завершаем сеанс.
			if(check == 0)
				check = SendQuery(s, Dlg, "QUIT\r\n");

			// Сообщение отправлено.
			if(check == 0)
				PrintMessage(Dlg, L"Сообщение отправлено!");
			
			closesocket(s);
			WSACleanup();

			// Удаляем задание из очереди.
			Dlg->SendData.erase(Dlg->SendData.begin());
			check = 0;
		}
	}

	// Снимаем метку процесса.
	Dlg->Process = 0;

	return AFX_THREADPROC();
}


// Удобный вывод сообщения.
void CSMTPDlg::PrintMessage(CString Message)
{
	// Вывод сообщения.
	CString Buff;
	CInfo.GetWindowTextW(Buff);
	CInfo.SetWindowTextW(Buff + Message + L"\r\n");

	// Прокрутка текста.
	CInfo.LineScroll(CInfo.GetLineCount());
	//CInfo.SendMessage(EM_SETSEL,-1,-1);
}
void CSMTPDlg::PrintMessage(CSMTPDlg* Dlg, CString Message)
{
	// Вывод сообщения.
	CString Buff;
	Dlg->CInfo.GetWindowTextW(Buff);
	Dlg->CInfo.SetWindowTextW(Buff + Message + L"\r\n");

	// Прокрутка текста.
	Dlg->CInfo.LineScroll(Dlg->CInfo.GetLineCount());
	//Dlg->CInfo.SendMessage(EM_SETSEL,-1,-1);
}


// Удобный перевод числа в CString.
CString CSMTPDlg::intToCStr(int value)
{
	CString MSG;
	MSG.Format(L"%d", value);
	return MSG;
}

// Удобный перевод CString в String.
std::string CSMTPDlg::CStrToStr(CString Value)
{
	USES_CONVERSION;
	return T2A((LPCTSTR)Value);
}


// Удобное получение IP из имени.
DWORD CSMTPDlg::hNameToIP(CString Name)
{
	hostent* hn = gethostbyname(CStrToStr(Name).c_str());
	return *(DWORD*)hn->h_addr_list[0];
}


// Удобное выделение кода результата.
int CSMTPDlg::GetRCode(char buff[])
{
	int result = 0;
	for(int c = 0; c < 3; c++)
		if((byte)buff[c] > 47 && (byte)buff[c] < 58)
			result += (int)(((byte)buff[c] - 48) * pow((float)10, (float)(2 - c)));
	return result;
}


// Удобный приём кода результата.
int CSMTPDlg::RecvRCode(CSMTPDlg* Dlg, SOCKET s, int FirstValidCode, int SecondValidCode)
{
	char rbuff[65536];
	int recived = 0;
	int check = 0;
	int RCode = 0;
	
	if((recived = recv(s, rbuff, 65536, 0)) < 0) {
		PrintMessage(Dlg, L"Ошибка приёма сообщения! (№" + intToCStr(WSAGetLastError()) + L")");
		return 1;
	}
	else {

		RCode = GetRCode(rbuff);

		if(FirstValidCode > 0 && SecondValidCode > 0) {
			if(RCode != FirstValidCode && RCode != SecondValidCode) {
				check = 1;
			}
		}
		else {
			if(FirstValidCode > 0)
				if(RCode != FirstValidCode)
					check = 1;
			if(SecondValidCode > 0)
				if(RCode != SecondValidCode)
					check = 1;
		}
		
		if(check == 1) {
			PrintMessage(Dlg, L"Ошибка протокола! (Код результата: " + intToCStr(RCode) + L")");
			return 1;
		}
	}
		
	return 0;
}


// Удобная отправка запроса.
int CSMTPDlg::SendQuery(SOCKET s, CSMTPDlg * Dlg, std::string buffer)
{
	if(SOCKET_ERROR == (send(s, buffer.c_str(), buffer.size(), 0))) {
		PrintMessage(Dlg, L"Ошибка отправки сообщения! (№" + intToCStr(WSAGetLastError()) + L")");
		return 1;
	}
	return 0;
}
