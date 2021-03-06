
// testDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CtestDlg 对话框



CtestDlg::CtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress1);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CtestDlg 消息处理程序

BOOL CtestDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtestDlg::OnPaint()
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
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#include <fstream>
const int BUFF_SIZE = 10240 + 256 + 5;//数据+文件名字+当前读取到的实际字节长度。

void CtestDlg::CopyAnyFile()
{
	CStdioFile readfilestream;
	CStdioFile writefilestream;
	int flag;

	flag = readfilestream.Open("D://1.zip",  CFile::modeRead|CFile::typeBinary);
	if (flag == 0)
	{
		MessageBox("读文件失败！");
		return;
	}
	//获取文件总长度：
	unsigned long long int filelength = readfilestream.GetLength();

	char *FilePacket = new char[filelength + 1];
	readfilestream.Read(FilePacket, filelength);
	readfilestream.Close();


	flag = writefilestream.Open("D://1//1.zip",  CFile::modeWrite|CFile::modeCreate | CFile::typeBinary);
	if (flag == 0)
	{
		MessageBox("创建文件失败！");
		return;
	}
	char *pFilePacket = FilePacket;
	while ((filelength / 1024) > 0)
	{
		writefilestream.Write(pFilePacket, 1024);
		pFilePacket += 1024;
		filelength -= 1024;
	}
	

	writefilestream.Write(FilePacket, filelength);

	writefilestream.Close();
	delete[] FilePacket;

	MessageBox("Copy Over");

}
void CtestDlg::My_ReadFile(const char *filepath)
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
		My_WriteFile(TotalSendPacket);
	}
	delete[] TotalSendPacket;
	MessageBox("Send Over！");
}
int CtestDlg::My_WriteFile(const char *sct)
{
	assert(sct != NULL);
	//分割字符串求得数据包内容，文件名字，以及读取到得实际字节数目。
	char szBuf[10240];//数据包值
	char FileName[256];//文件名字
	memcpy(szBuf, sct, 10240);

	memcpy(FileName, sct + 10240, 256);
	int realreadByes;
	memcpy(&realreadByes, sct + 10240 + 256, 4);
	


	ofstream out_file_stream;
	
	out_file_stream.open(FileName, std::ios::binary|std::ios::app);
	if (!out_file_stream)
	{
		MessageBox("写入文件打开失败");
		return -1;
	}
	out_file_stream.write(szBuf, realreadByes);
	out_file_stream.close();
	return 0;
}
void CtestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//CopyAnyFile();
	//My_ReadFile("D:\\1.docx");
	m_progress1.SetRange(0, 100);
	m_progress1.SetStep(1);
	m_progress1.SetPos(0);
	for (int i = 0; i < 100; i++)
	{
		m_progress1.SetPos(i);
		Sleep(100);
		//m_progress1.StepIt();
	}
	MessageBox("传输完成！");
}

