
// ClientDlg.h: 头文件
//

#pragma once


// CClientDlg 对话框
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
};
