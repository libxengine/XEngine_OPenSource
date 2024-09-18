
// XEngine_APPSocketTestDlg.h: 头文件
//

#pragma once


// CXEngineAPPSocketTestDlg 对话框
class CXEngineAPPSocketTestDlg : public CDialogEx
{
// 构造
public:
	CXEngineAPPSocketTestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XENGINE_APPSOCKETTEST_DIALOG };
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
	static bool CALLBACK NetCore_TCPSelect_CBLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
	static void CALLBACK NetCore_TCPSelect_CBRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen, XPVOID lParam);
	static void CALLBACK NetCore_TCPSelect_CBLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
	static void CALLBACK XClient_TCPSelect_CBEvent(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_XCLIENT_SOCKET_EVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nMsgLen, XPVOID lParam);
protected:
	bool SocketTest_LogPrint(int nType, XNETHANDLE xhClient, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer = NULL, int nMSGLen = 0);
private:
	XSHOT nClientType = 0;  //0:未决,1:TCP 服务器,2:TCP 客户端,3:UDP 服务器,4:UDP 客户端
	XHANDLE xhToken = NULL;
	XHANDLE xhUDPToken = NULL;
	XSOCKET xhUDPSocket = 0;
	XNETHANDLE xhClient = 0;
public:
	CEdit m_EditIPPort;
	CEdit m_EditRecv;
	CEdit m_EditSend;
	CEdit m_EditIPAddr;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CButton m_BtnStartService;
	CButton m_BtnStartConnect;
	afx_msg void OnBnClickedButton4();
	CButton m_BtnStop;
	afx_msg void OnDestroy();
	CEdit m_EditIPClient;
	afx_msg void OnBnClickedButton2();
	CButton m_RadioProtocolTCP;
	CButton m_RadioProtocolUDP;
};
