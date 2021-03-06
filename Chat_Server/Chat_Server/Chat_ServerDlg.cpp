
// Chat_ServerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Chat_Server.h"
#include "Chat_ServerDlg.h"
//#include "afxdialogex.h"
//#include <afxwin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CString IP;
UINT server_thd(LPVOID p);
SOCKET listen_sock;
SOCKET sock;

SOCKET listen_sock_file;
SOCKET socksd;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CChatServerDlg 对话框



CChatServerDlg::CChatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listwords);
	DDX_Control(pDX, IDC_PROGRESS1, m_process1);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnSend, &CChatServerDlg::OnBnClickedBtnsend)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChatServerDlg 消息处理程序
CString GetTime()
{
	CTime time = CTime::GetCurrentTime();
	CString currenttime;
	currenttime = time.Format(_T("(%Y-%m-%d %H:%M:%S)"));
	return currenttime;

}
UINT server_thd(LPVOID p)
{
	SOCKADDR_IN local_addr;
	SOCKADDR_IN client_addr;
	int iaddrSize = sizeof(SOCKADDR_IN);
	int res;
	char msg[1024] = { 0 };
	CChatServerDlg * dlg = (CChatServerDlg  *)AfxGetApp()->GetMainWnd();
	char ch_ip[20];

	CStringA tmp;
	tmp = IP;
	sprintf(ch_ip, "%s", tmp.GetBuffer());

	//local_addr.sin_addr.s_addr = htonl(INADDR_ANY);//获取任意IP地址
	local_addr.sin_addr.s_addr = inet_addr(ch_ip);
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(8888);
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//创建套接字
	{
		dlg->update(_T("创建监听失败"));
		return -1;
	}
	if (bind(listen_sock, (struct sockaddr*) &local_addr, sizeof(SOCKADDR_IN)))//绑定套接字
	{
		dlg->update(_T("绑定错误"));
		return -1;
	}
Begin:

	listen(listen_sock, 20);
	if ((sock = accept(listen_sock, (struct sockaddr *)&client_addr, &iaddrSize)) == INVALID_SOCKET)//接收套接字
	{
		dlg->update(_T("accept 失败"));
		closesocket(sock);
		closesocket(listen_sock);
		goto Begin;//再次监听等待新的客户端连接
	}
	else
	{
		CString port;
		port.Format(_T("%d"), int(ntohs(client_addr.sin_port)));
		dlg->update(_T("已连接客户端：") + CString(inet_ntoa(client_addr.sin_addr)) + "  端口：" + port);
	}
	////////////接收数据
	while (1)
	{
		if ((res = recv(sock, msg, 1024, 0)) == -1)
		{
			dlg->update(_T("失去客户端的连接"));
			closesocket(sock);
			closesocket(listen_sock);
			goto Begin;//再次监听等待新的客户端连接
		}
		else
		{
			msg[res] = '\0';
			dlg->update(_T("client:") + CString(msg));
		}
	}
	return 0;
}

UINT server_filethd(LPVOID p)
{
	SOCKADDR_IN local_addr;
	SOCKADDR_IN client_addr;
	int iaddrSize = sizeof(SOCKADDR_IN);
	int res;
	char msg[10240];
	CChatServerDlg * dlg = (CChatServerDlg  *)AfxGetApp()->GetMainWnd();
	char ch_ip[20];

	CStringA tmp;
	tmp = IP;
	sprintf(ch_ip, "%s", tmp.GetBuffer());

	//local_addr.sin_addr.s_addr = htonl(INADDR_ANY);//获取任意IP地址
	local_addr.sin_addr.s_addr = inet_addr(ch_ip);
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(7777);
Begin:
	if ((listen_sock_file = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//创建套接字
	{
		dlg->update(_T("创建监听失败(file port)"));
	}
	if (bind(listen_sock_file, (struct sockaddr*) &local_addr, sizeof(SOCKADDR_IN)))//绑定套接字
	{
		dlg->update(_T("绑定错误(file port)"));
	}

	listen(listen_sock_file, 20);
	if ((socksd = accept(listen_sock_file, (struct sockaddr *)&client_addr, &iaddrSize)) == INVALID_SOCKET)//接收套接字
	{
		dlg->update(_T("accept 失败(file port)"));
		closesocket(sock);
		closesocket(listen_sock_file);
		goto Begin;//再次监听等待新的客户端连接
	}
	else
	{
		CString port;
		port.Format(_T("%d"), int(ntohs(client_addr.sin_port)));
		dlg->update(_T("已连接客户端(file port)：") + CString(inet_ntoa(client_addr.sin_addr)) + "  端口：" + port);
	}
	////////////接收数据
	while (1)
	{
		if ((res = recv(socksd, msg, 10240+256+4+4, 0)) == -1)
		{
			dlg->update(_T("失去客户端的连接(file port)"));
			closesocket(sock);
			closesocket(listen_sock_file);
			goto Begin;//再次监听等待新的客户端连接
		}
		else
		{
			dlg->RecvFileData(msg);

			//dlg->m_process1.SetPos(0);
		}
	}
	return 0;
}
static long long  totalBytes = 0;
int CChatServerDlg::RecvFileData(const char *sct)
{
	assert(sct != NULL);
	//分割字符串求得数据包内容，文件名字，以及读取到得实际字节数目。
	char szBuf[10240];//数据包值
	char FileName[256];//文件名字
	memcpy(szBuf, sct, 10240);

	memcpy(FileName, sct + 10240, 256);
	int realreadByes = 0;
	memcpy(&realreadByes, sct + 10240 + 256, 4);

	int FiletotalBytes = 0;
	memcpy(&FiletotalBytes, sct + 10240 + 256 + 4, 4);


	ofstream out_file_stream;

	out_file_stream.open(FileName, std::ios::binary | std::ios::app);
	if (!out_file_stream)
	{
		MessageBox("写入文件打开失败");
		return -1;
	}
	out_file_stream.write(szBuf, realreadByes);
	out_file_stream.close();

	totalBytes += realreadByes;

	//更新进度条
	/*CChatServerDlg * dlg = (CChatServerDlg  *)AfxGetApp()->GetMainWnd();
	int iposition = totalBytes / FiletotalBytes;
	dlg->m_process1.SetPos(iposition);*/

	if (totalBytes == FiletotalBytes) { MessageBox("文件接受完毕！"); FiletotalBytes = 0; }
	return 0;
}


BOOL CChatServerDlg::OnInitDialog()
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
	send_edit = (CEdit*)GetDlgItem(IDC_EDIT1);
	send_edit->SetFocus();
	char name[128];
	HOSTENT* pHost;
	gethostname(name, 128);//获得主机名字
	pHost = gethostbyname(name);//获得主机结构
	IP = inet_ntoa(*(in_addr*)pHost->h_addr);
	update(_T("本地服务器IP地址:") + IP);
	AfxBeginThread(server_thd,NULL);
	AfxBeginThread(server_filethd, NULL);

	m_process1.SetRange(0, 100);
	m_process1.SetStep(1);
	m_process1.SetPos(0);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatServerDlg::update(CString s)
{
	m_listwords.AddString(s+GetTime());
}
void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatServerDlg::OnPaint()
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
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatServerDlg::OnBnClickedBtnsend()
{
	// TODO: 在此添加控件通知处理程序代码
	//SOCKET sock;
	CString s;
	char  msg[1024];
	send_edit->GetWindowText(s);

	CStringA tmp;
	tmp = s;
	sprintf(msg, "%s", tmp.GetBuffer());
	
	if (s == "")
	{
		MessageBox(_T("请输入信息"));
	}
	else if (send(sock, msg, strlen(msg), 0) == SOCKET_ERROR)
	{
		update(_T("发送失败"));
	}
	else
	{
		s = msg;
		//update(s);//消息上屏，清空输入，并重获焦点
		//show_edit->ReplaceSel(_T("server:") + s);//消息上屏，清空输入，并重获焦点
		update(_T("server:") + s);
		send_edit->SetWindowText(_T(""));
		m_listwords.SetFocus();
	}
}


BOOL CChatServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)

	{

		if (pMsg->wParam == VK_RETURN)//当按下键为回车键时

		{

			OnBnClickedBtnsend();//调用按钮函数

			return TRUE;

		}

	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
