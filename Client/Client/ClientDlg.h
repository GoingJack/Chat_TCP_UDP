
// ClientDlg.h: 头文件
//

#pragma once
#include <iostream>
#include <fstream>
using namespace std;
// CClientDlg 对话框

typedef struct
{
	char FileName[256];//文件的名称（只有第一个数据包才有）;
	int index;//第几个数据包。
	int totallength;//总共几个数据包。
	int DataLength;//DataPacket数据包的长度
	char DataPacket[10240];//每个数据包的内容。
	char MD5[129];//文件的MD5值。
}SendFilePacket;
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void update(CString s);
private:
	CEdit *send_edit;
	
public:
	CListBox m_listwords;
	CIPAddressCtrl m_ip;
	afx_msg void OnBnClickedBtnconnect();
	afx_msg void OnBnClickedBtnsend();
	CButton btnconn;
	CEdit ip_edit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMFCEditBrowseCtrl m_SelectFile;
	CButton m_ButtonSendFile;
	afx_msg void OnBnClickedButtonsendfiles();
	//传输文件
	void CClientDlg::DealAndSendFile(const char *filepath);
};
typedef struct {
	char pp[1025];
}test;

typedef struct
{
	CString FileName;//文件名字。
	CString FileMD5;//文件的MD5值。
	int index;//当前数据包的序号。
	int totalindex;//所有的数据包个数。
	



}SendFile;