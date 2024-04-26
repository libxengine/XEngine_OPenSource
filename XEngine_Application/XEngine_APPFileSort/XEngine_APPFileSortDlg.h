
// XEngine_APPFileSortDlg.h: 头文件
//

#pragma once


// CXEngineAPPFileSortDlg 对话框
class CXEngineAPPFileSortDlg : public CDialogEx
{
// 构造
public:
	CXEngineAPPFileSortDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XENGINE_APPFILESORT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	bool bThread = false;
	bool bThreadList = false;
	bool bThreadName = false;
	std::unique_ptr<std::thread> pSTDThread;
	std::unique_ptr<std::thread> pSTDThreadList;
	std::unique_ptr<std::thread> pSTDThreadName;
	static void XEngine_FileSort_Thread(LPVOID lParam);
	static void XEngine_FileSort_ThreadList(LPVOID lParam);
	static void XEngine_FileSort_ThreadName(LPVOID lParam);
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CEdit m_EditSelectDir;
	CListCtrl m_ListFile;
	CEdit m_EditNumber;
	CStatic m_StaticTips;
	afx_msg void OnDestroy();
};
