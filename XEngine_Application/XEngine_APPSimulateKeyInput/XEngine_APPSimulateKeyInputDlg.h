
// XEngine_APPSimulateKeyInputDlg.h: 头文件
//

#pragma once


// CXEngineAPPSimulateKeyInputDlg 对话框
class CXEngineAPPSimulateKeyInputDlg : public CDialogEx
{
// 构造
public:
	CXEngineAPPSimulateKeyInputDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XENGINE_APPSIMULATEKEYINPUT_DIALOG };
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
	bool m_bRun = false;
	unique_ptr<thread> pSTD_ThreadCheck1 = NULL;
	unique_ptr<thread> pSTD_ThreadCheck2 = NULL;
	unique_ptr<thread> pSTD_ThreadCheck3 = NULL;
	unique_ptr<thread> pSTD_ThreadCheck4 = NULL;
	static int CALLBACK Keyboard_ThreadCheck1(LPVOID lParam);
	static int CALLBACK Keyboard_ThreadCheck2(LPVOID lParam);
	static int CALLBACK Keyboard_ThreadCheck3(LPVOID lParam);
	static int CALLBACK Keyboard_ThreadCheck4(LPVOID lParam);
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CEdit m_EditTime1;
	CEdit m_EditTime2;
	CEdit m_EditTime3;
	CEdit m_EditTime4;
	CEdit m_EditCheck1;
	CEdit m_EditCheck2;
	CEdit m_EditCheck3;
	CEdit m_EditCheck4;
	CButton m_Check1;
	CButton m_Check2;
	CButton m_Check3;
	CButton m_Check4;
	CButton m_BtnStop;
	CButton m_BtnStart;
	CStatic m_StaticTips;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
};
