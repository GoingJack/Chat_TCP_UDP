
// Chat_ServerDlg.h: 头文件
//

#pragma once


// CChatServerDlg 对话框
class CChatServerDlg : public CDialogEx
{
// 构造
public:
	CChatServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_SERVER_DIALOG };
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
	CListBox m_listwords;
	//这里是我所添加的
	void update(CString s);
	

	 
private:
	CEdit* send_edit;
	
public:
	afx_msg void OnBnClickedBtnsend();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton BtnSend;
	virtual void OnOK();
};

