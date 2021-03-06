
// ClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
void CString2Char(CString str, char ch[]);
SOCKET sock;
UINT server_thd(LPVOID p);
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listwords);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Control(pDX, IDC_BtnConnect, btnconn);
	DDX_Control(pDX, IDC_EDIT2, ip_edit);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnConnect, &CClientDlg::OnBnClickedBtnconnect)
	ON_BN_CLICKED(IDC_BtnSend, &CClientDlg::OnBnClickedBtnsend)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	send_edit=(CEdit *)GetDlgItem(IDC_EDIT1);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CClientDlg::update(CString s)
{
	m_listwords.AddString(s);
}
UINT recv_thd(LPVOID p)
{
	int res;
	char msg[1024];
	//CString s;
	CClientDlg * dlg = (CClientDlg  *)AfxGetApp()->GetMainWnd();
	////////////接收数据
	while (1)
	{
		if ((res = recv(sock, msg, 1024, 0)) == -1)//接收服务器的数据
		{
			dlg->update(_T("失去连接"));
			break;
		}
		else
		{
			msg[res] = '\0';
			dlg->update(_T("server:") + CString(msg));
		}
	}
	//closesocket(sock);
	return 0;
}


void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnBnClickedBtnconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	WSADATA wsaData;
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	WORD wVersion;
	wVersion = MAKEWORD(2, 2);
	WSAStartup(wVersion, &wsaData);
	// WSAStartup(0x0202, &wsaData);
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		update(_T("create socket error !!!"));
	}
	//CString ip;
	//ip_edit->GetWindowTextW(ip);//取得服务器的IP地址
	//server_addr.sin_addr.s_addr = inet_addr((LPSTR)(LPCSTR)ip.GetBuffer());
	BYTE nArrIP[4];
	m_ip.GetAddress(nArrIP[0], nArrIP[1], nArrIP[2], nArrIP[3]);
	CString str;
	str.Format(_T("%d.%d.%d.%d"), nArrIP[0], nArrIP[1], nArrIP[2], nArrIP[3]);
	ip_edit.SetWindowTextW(str);
	char cp[50];
	CString2Char(str, cp);
	server_addr.sin_addr.s_addr = inet_addr(cp);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8888);
	bind(sock, (SOCKADDR*)&server_addr, sizeof(SOCKADDR));
	if (connect(sock, (struct sockaddr *) &server_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		update(_T("连接失败"));
	}
	else
	{
		//show_edit->SetWindowText(_T(""));
		update(_T("连接成功"));
		btnconn.EnableWindow(FALSE);//按钮变灰
		AfxBeginThread(recv_thd, NULL);
	}
}
void CString2Char(CString str, char ch[])
{
	int i;
	char *tmpch;
	int wLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);//得到Char的长度
	tmpch = new char[wLen + 1];                                             //分配变量的地址大小
	WideCharToMultiByte(CP_ACP, 0, str, -1, tmpch, wLen, NULL, NULL);       //将CString转换成char*


	for (i = 0; tmpch[i] != '\0'; i++) ch[i] = tmpch[i];
	ch[i] = '\0';
}


void CClientDlg::OnBnClickedBtnsend()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString s;
//char * msg;
//send_edit->GetWindowTextW(s);
//msg = (LPSTR)(LPCTSTR)s;
//CString2Char(s, msg);
	CString s;
	char msg[1024];
	send_edit->GetWindowTextW(s);
	CString2Char(s, msg);
	if (send(sock, msg, strlen(msg), 0) == SOCKET_ERROR)
	{
		update(_T("发送失败"));
	}
	else if (s == "")
	{
		MessageBox(_T("请输入信息"));
	}
	else
	{
		s = msg;
		update(_T("client:") + s);//消息上屏，清空输入，并重获焦点
		send_edit->SetWindowText(_T(""));
		send_edit->SetFocus();
	}
	/*CString s;
	char * msg;
	send_edit->GetWindowText(s);
	msg = (char*)s.GetBuffer(s.GetLength());
	if (send(sock, msg, strlen(msg), 0) == SOCKET_ERROR)
	{
	show_edit->ReplaceSel(_T("发送失败/r/n"));
	}
	else if (s == "")
	{
	MessageBox(_T("请输入信息"));
	}
	else
	{
	show_edit->ReplaceSel(_T("client:") + s + "/r/n");//消息上屏，清空输入，并重获焦点
	send_edit->SetWindowText(_T(""));
	send_edit->SetFocus();
	}*/
}
