
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
SOCKET sockfd;
int fromlen = sizeof(struct sockaddr);
SOCKADDR_IN serv_addr;

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
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnSend, &CClientDlg::OnBnClickedBtnsend)
END_MESSAGE_MAP()

UINT recv_thd(LPVOID p)
{
	//int res;
	//sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	//serv_addr.sin_family = AF_INET;
	//serv_addr.sin_port = htons(8888);
	//serv_addr.sin_addr.s_addr = inet_addr("192.168.13.1");
	//char buffer[1024];

	//CClientDlg *dlg = (CClientDlg*)AfxGetApp()->GetMainWnd();
	//if (bind(sockfd, (struct sockaddr*) &sockfd, sizeof(SOCKADDR_IN)))
	//{
	//	dlg->update(_T("绑定错误"));
	//}
	//while (1)
	//{
	//	if ((res = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&serv_addr, &fromlen)) < 0)
	//	{
	//		dlg->update(_T("接受数据失败"));
	//		break;
	//	}
	//	else
	//	{
	//		buffer[res] = '\0';
	//		dlg->update(_T("server:") + CString(buffer));
	//	}
	//	//closesocket(sock);
	//}
	//return 0;
	SOCKADDR_IN local_addr;
	int iaddrSize = sizeof(SOCKADDR_IN);

	int res;
	int len = sizeof(struct sockaddr);
	char buf[1024];
	char buffer[1024];
	CClientDlg * dlg = (CClientDlg   *)AfxGetApp()->GetMainWnd();
	char ch_ip[20];
	//CString2Char(IP, ch_ip);//注意！这里调用了字符格式转换函数，此函数功能：CString类型转换为Char类型，实现代码在后面添加
	//local_addr.sin_addr.s_addr = htonl(INADDR_ANY);//获取任意IP地址
	local_addr.sin_addr.s_addr = inet_addr("192.168.13.1");
	local_addr.sin_family = AF_INET;//设置为IP通讯
	local_addr.sin_port = htons(8888);//服务器端口号
	SOCKET listen_sock;
	listen_sock = socket(AF_INET, SOCK_DGRAM, 0);//创建套接字

	if (bind(listen_sock, (struct sockaddr*) &local_addr, iaddrSize))//绑定套接字
	{
		dlg->update(_T("绑定错误"));
	}
	////////////接收数据
	while (1)
	{
		if ((res = recvfrom(listen_sock, buf, 1024, 0, (struct sockaddr *)&local_addr, &len)) == -1)
		{
			dlg->update(_T("接受数据失败"));
			break;
		}
		else
		{
			buf[res] = '\0';
			dlg->update(_T("client:") + CString(buf));
		}
	}
	return 0;
}

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
	AfxBeginThread(recv_thd, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CClientDlg::update(CString s)
{
	m_listwords.AddString(s);
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
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8888);
	serv_addr.sin_addr.s_addr = inet_addr("192.168.13.1");
	CString s;
	char msg[1024];
	send_edit->GetWindowTextW(s);
	CString2Char(s, msg);
	if (s == "")
	{
	MessageBox(_T("请输入信息"));
	}
	else if (sendto(sockfd, msg, 1024, 0,(struct sockaddr*)&serv_addr,fromlen) == SOCKET_ERROR)
	{
		update(_T("发送失败"));
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
