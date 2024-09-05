
// XEngine_APPSocketTestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "XEngine_APPSocketTest.h"
#include "XEngine_APPSocketTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXEngineAPPSocketTestDlg 对话框



CXEngineAPPSocketTestDlg::CXEngineAPPSocketTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XENGINE_APPSOCKETTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXEngineAPPSocketTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditIPPort);
	DDX_Control(pDX, IDC_EDIT3, m_EditRecv);
	DDX_Control(pDX, IDC_EDIT4, m_EditSend);
	DDX_Control(pDX, IDC_EDIT2, m_EditIPAddr);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnStartService);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnStartConnect);
	DDX_Control(pDX, IDC_BUTTON4, m_BtnStop);
	DDX_Control(pDX, IDC_EDIT5, m_EditIPClient);
}

BEGIN_MESSAGE_MAP(CXEngineAPPSocketTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CXEngineAPPSocketTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CXEngineAPPSocketTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CXEngineAPPSocketTestDlg::OnBnClickedButton4)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CXEngineAPPSocketTestDlg 消息处理程序

BOOL CXEngineAPPSocketTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	m_EditIPPort.SetWindowText(_T("5000"));

	m_BtnStartService.EnableWindow(true);
	m_BtnStartConnect.EnableWindow(true);
	m_BtnStop.EnableWindow(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXEngineAPPSocketTestDlg::OnPaint()
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
HCURSOR CXEngineAPPSocketTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CALLBACK CXEngineAPPSocketTestDlg::NetCore_TCPSelect_CBLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	CXEngineAPPSocketTestDlg* pClass_This = (CXEngineAPPSocketTestDlg*)lParam;

	pClass_This->SocketTest_LogPrint(0, lpszClientAddr);
	return true;
}
void CALLBACK CXEngineAPPSocketTestDlg::NetCore_TCPSelect_CBRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	CXEngineAPPSocketTestDlg* pClass_This = (CXEngineAPPSocketTestDlg*)lParam;

	pClass_This->SocketTest_LogPrint(1, lpszClientAddr, lpszMsgBuffer, nMsgLen);
}
void CALLBACK CXEngineAPPSocketTestDlg::NetCore_TCPSelect_CBLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	CXEngineAPPSocketTestDlg* pClass_This = (CXEngineAPPSocketTestDlg*)lParam;

	pClass_This->SocketTest_LogPrint(2, lpszClientAddr);
}

bool CXEngineAPPSocketTestDlg::SocketTest_LogPrint(int nType, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMSGLen)
{
	CString m_StrLog;
	TCHAR tszMSGBuffer[2048] = {}; 
	USES_CONVERSION;
	if (0 == nType)
	{
		_stprintf(tszMSGBuffer, _T("事件=用户:%s 登录\r\n"), A2W(lpszClientAddr));
		m_EditIPClient.SetWindowText(A2W(lpszClientAddr));
	}
	else if (1 == nType)
	{
		_stprintf(tszMSGBuffer, _T("事件=用户:%s 接受数据,大小:%d:%s\r\n"), A2W(lpszClientAddr), nMSGLen, A2W(lpszMsgBuffer));
	}
	else
	{
		_stprintf(tszMSGBuffer, _T("事件=用户:%s 离开\r\n"), A2W(lpszClientAddr));
	}
	
	m_EditRecv.GetWindowText(m_StrLog);

	if (m_StrLog.GetLength() > 10240)
	{
		m_StrLog.ReleaseBuffer();
	}
	m_StrLog.Append(tszMSGBuffer);
	m_EditRecv.SetWindowText(m_StrLog);
	m_EditRecv.SetSel(m_StrLog.GetLength(), m_StrLog.GetLength());
	m_EditRecv.SetFocus();
	return true;
}

void CXEngineAPPSocketTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;

	m_EditIPPort.GetWindowText(m_StrIPPort);
	if (!NetCore_TCPSelect_Start(_ttoi(m_StrIPPort.GetBuffer())))
	{
		AfxMessageBox(_T("启动服务端失败"));
		return;
	}
	NetCore_TCPSelect_RegisterCallBack(NetCore_TCPSelect_CBLogin, NetCore_TCPSelect_CBRecv, NetCore_TCPSelect_CBLeave, this, this, this);
	AfxMessageBox(_T("启动服务端成功"));

	m_BtnStartService.EnableWindow(false);
	m_BtnStartConnect.EnableWindow(false);
	m_BtnStop.EnableWindow(true);
}


void CXEngineAPPSocketTestDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrSendMsg;
	CString m_StrSendAddr;
	m_EditSend.GetWindowText(m_StrSendMsg);

	if (m_StrSendMsg.IsEmpty())
	{
		AfxMessageBox(_T("需要输入发送的数据"));
		return;
	}
	m_EditIPClient.GetWindowText(m_StrSendAddr);

	USES_CONVERSION;
	if (!NetCore_TCPSelect_Send(W2A(m_StrSendAddr.GetBuffer()), W2A(m_StrSendMsg.GetBuffer()), m_StrSendMsg.GetLength()))
	{
		AfxMessageBox(_T("发送数据失败"));
		return;
	}
	m_EditSend.SetWindowText(_T(""));
}


void CXEngineAPPSocketTestDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BtnStartService.EnableWindow(true);
	m_BtnStartConnect.EnableWindow(true);
	m_BtnStop.EnableWindow(false);
	NetCore_TCPSelect_Stop();
}


void CXEngineAPPSocketTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	WSACleanup();
}
