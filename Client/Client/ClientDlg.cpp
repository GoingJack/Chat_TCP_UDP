
// ClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const int BUFF_SIZE = 10240 + 256 + 4 + 4;//文件内容 + 文件名称 + 当前数据包长度 + 总长度。
CString GetTime();
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
void CString2Char(CString str, char ch[]);
SOCKET sock;
SOCKET Filesock;
UINT server_thd(LPVOID p);

void CClientDlg::DealAndSendFile(const char *filepath)
{
	if (NULL == filepath)
	{
		MessageBox("参数文件名字为空");
		return;
	}

	ifstream input_file_stream;
	input_file_stream.open(filepath, std::ios::binary);// 以输入流打开文件
	if (!input_file_stream)
	{
		MessageBox("打开文件失败！");
		return;
	}
	input_file_stream.seekg(0, ios::end);//将文件指针移动至末尾
	int d_file_length = input_file_stream.tellg();// 获取文件长度
	input_file_stream.seekg(0, ios::beg);//将文件指针移到至开始

	char *TotalSendPacket = new char[BUFF_SIZE];
	char szBuf[10240];
	char FileName[256] = { 0 };

	//从文件路径中提取文件名字
	int i = 0;
	char filename[256] = { 0 };
	const char *ptoslant = filepath;
	const char *ptmptoslant = ptoslant;
	while ((ptmptoslant = strstr(ptoslant, "\\")) != NULL)ptoslant = ptmptoslant + 1;
	while (filename[i++] = *ptoslant++);
	remove(filename);//为了测试方便，每次删除文件如果存在
	while (!input_file_stream.eof())
	{
		input_file_stream.read(szBuf, sizeof(char) * 10240);
		int realreadBytes = input_file_stream.gcount();//获取本次真正读到的字节
		if (input_file_stream.bad())
		{
			MessageBox("读取文件异常");
			break;
		}
		//将信息拷贝到总字符串处
		memcpy(TotalSendPacket, szBuf, 10240);
		memcpy(TotalSendPacket + 10240, filename, 256);
		memcpy(TotalSendPacket + 10240 + 256, &realreadBytes, sizeof(int));
		memcpy(TotalSendPacket + 10240 + 256 + 4, &d_file_length, sizeof(int));
		if (send(Filesock, TotalSendPacket, BUFF_SIZE, 0) == SOCKET_ERROR)
		{
			MessageBox("发送失败!");
		}
	}

	delete[] TotalSendPacket;
	MessageBox("Send Over！");
}
UINT sendFile(LPVOID p)
{
	char *filepath = (char *)p;
	

	int res;
	char msg[1024];
	//CString s;
	CClientDlg * dlg = (CClientDlg  *)AfxGetApp()->GetMainWnd();
	dlg->DealAndSendFile(filepath);
	////////////接收数据
	//while (1)
	//{
	//	if ((res = recv(Filesock, msg, 1024, 0)) == -1)//接收服务器的数据
	//	{
	//		dlg->update(_T("(send file)失去连接") + GetTime());
	//		break;
	//	}
	//	else
	//	{
	//		msg[res] = '\0';
	//		dlg->update(_T("(send file)server:") + CString(msg) + _T("(") + GetTime() + _T(")"));
	//	}
	//}
	return 0;
}


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
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_SelectFile);
	DDX_Control(pDX, IDC_ButtonSendFiles, m_ButtonSendFile);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnConnect, &CClientDlg::OnBnClickedBtnconnect)
	ON_BN_CLICKED(IDC_BtnSend, &CClientDlg::OnBnClickedBtnsend)
	ON_BN_CLICKED(IDC_ButtonSendFiles, &CClientDlg::OnBnClickedButtonsendfiles)
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
	m_SelectFile.EnableWindow(FALSE);
	m_ButtonSendFile.EnableWindow(FALSE);
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
			dlg->update(_T("失去连接")+GetTime());
			dlg->btnconn.EnableWindow(TRUE);//按钮变灰
			break;
		}
		else
		{
			msg[res] = '\0';
			dlg->update(_T("server:") + CString(msg)+_T("(")+GetTime()+_T(")"));
		}
	}
	closesocket(sock);
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
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
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
	ip_edit.SetWindowText(str);
	char cp[50];
	//CString2Char(str, cp);
	CStringA tmp;
	tmp = str;
	sprintf(cp, "%s", tmp.GetBuffer());

	server_addr.sin_addr.s_addr = inet_addr(cp);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8888);
	bind(sock, (SOCKADDR*)&server_addr, sizeof(SOCKADDR));
	if (connect(sock, (struct sockaddr *) &server_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		update(_T("连接失败")+GetTime());
	}
	else
	{
		//show_edit->SetWindowText(_T(""));
		update(_T("连接成功")+GetTime());
		btnconn.EnableWindow(FALSE);//按钮变灰
		m_SelectFile.EnableWindow(TRUE);
		m_ButtonSendFile.EnableWindow(TRUE);
		AfxBeginThread(recv_thd, NULL);
	}
}
void CClientDlg::OnBnClickedButtonsendfiles()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePath;
	long long int m_length = 0;
	GetDlgItemText(IDC_MFCEDITBROWSE1, FilePath);

	//MessageBox(FilePath);

	if (FilePath.IsEmpty())
	{
		MessageBox(_T("请选择要传输的文件！"));
		return;
	}

	CStringA tmp;
	tmp = FilePath;
	char *pp = (char *)malloc(sizeof(char) * 1024);
	memset(pp, 0, sizeof(pp));
	pp = tmp.GetBuffer();

	char *filepath = new char[256];
	assert(filepath != NULL);
	strcpy(filepath, pp);






	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	if ((Filesock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		update(_T("(send file)create socket error !!!"));
	}
	BYTE nArrIP[4];
	m_ip.GetAddress(nArrIP[0], nArrIP[1], nArrIP[2], nArrIP[3]);
	CString str;
	str.Format(_T("%d.%d.%d.%d"), nArrIP[0], nArrIP[1], nArrIP[2], nArrIP[3]);
	ip_edit.SetWindowText(str);
	char cp[50];
	//CString2Char(str, cp);
	tmp = str;
	sprintf(cp, "%s", tmp.GetBuffer());

	server_addr.sin_addr.s_addr = inet_addr(cp);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(7777);
	bind(Filesock, (SOCKADDR*)&server_addr, sizeof(SOCKADDR));
	if (connect(Filesock, (struct sockaddr *) &server_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		update(_T("连接失败(send file)") + GetTime());
	}
	else
	{
		//show_edit->SetWindowText(_T(""));
		update(_T("连接成功(send file)") + GetTime());
		AfxBeginThread(sendFile, (LPVOID)filepath);
	}
}

//void CString2Char(CString str, char ch[])
//{
//	int i;
//	char *tmpch;
//	int wLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);//得到Char的长度
//	tmpch = new char[wLen + 1];                                             //分配变量的地址大小
//	WideCharToMultiByte(CP_ACP, 0, str, -1, tmpch, wLen, NULL, NULL);       //将CString转换成char*
//
//
//	for (i = 0; tmpch[i] != '\0'; i++) ch[i] = tmpch[i];
//	ch[i] = '\0';
//}
CString GetTime()
{
	CTime time = CTime::GetCurrentTime();
	CString currentTime;
	currentTime = time.Format(_T("(%Y-%m-%d %H:%M:%S)"));
	return currentTime;
}


void CClientDlg::OnBnClickedBtnsend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	char msg[1024];
	send_edit->GetWindowText(s);
	/*CString2Char(s, msg);*/
	CStringA tmp;
	tmp = s;
	sprintf(msg, "%s", tmp.GetBuffer());

	if (send(sock, msg, strlen(msg), 0) == SOCKET_ERROR)
	{
		update(_T("发送失败")+GetTime());
	}
	else if (s == "")
	{
		MessageBox(_T("请输入信息"));
	}
	else
	{
		//CString currentTime= GetTime();
		s = msg;
		update(_T("client:") + s+GetTime());//消息上屏，清空输入，并重获焦点
		send_edit->SetWindowText(_T(""));
		send_edit->SetFocus();
	}
}


BOOL CClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)

	{

		if (pMsg->wParam == VK_RETURN)//当按下键为回车键时

		{

			OnBnClickedBtnsend();//调用按钮函数

			return TRUE;

		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



