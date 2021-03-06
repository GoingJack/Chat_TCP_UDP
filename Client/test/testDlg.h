
// testDlg.h: 头文件
//
#include <iostream>
using namespace std;
#pragma once

typedef struct
{
	char FileName[256];//文件的名称（只有第一个数据包才有）;
	int index;//第几个数据包。
	int totallength;//总共几个数据包。
	int DataLength;//DataPacket数据包的长度
	char DataPacket[10240];//每个数据包的内容。
	char MD5[129];//文件的MD5值。
}SendFilePacket;
// CtestDlg 对话框
class CtestDlg : public CDialogEx
{
// 构造
public:
	CtestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	CProgressCtrl m_progress1;
	void CopyAnyFile();
	//
	void My_ReadFile(const char *name);
	int My_WriteFile(const char  *sct);

};
