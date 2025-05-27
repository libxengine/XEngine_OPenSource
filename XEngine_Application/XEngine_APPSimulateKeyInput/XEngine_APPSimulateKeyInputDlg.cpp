
// XEngine_APPSimulateKeyInputDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "XEngine_APPSimulateKeyInput.h"
#include "XEngine_APPSimulateKeyInputDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXEngineAPPSimulateKeyInputDlg 对话框



CXEngineAPPSimulateKeyInputDlg::CXEngineAPPSimulateKeyInputDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XENGINE_APPSIMULATEKEYINPUT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXEngineAPPSimulateKeyInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_EditTime1);
	DDX_Control(pDX, IDC_EDIT4, m_EditTime2);
	DDX_Control(pDX, IDC_EDIT6, m_EditTime3);
	DDX_Control(pDX, IDC_EDIT8, m_EditTime4);
	DDX_Control(pDX, IDC_EDIT1, m_EditCheck1);
	DDX_Control(pDX, IDC_EDIT3, m_EditCheck2);
	DDX_Control(pDX, IDC_EDIT5, m_EditCheck3);
	DDX_Control(pDX, IDC_EDIT7, m_EditCheck4);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_CHECK3, m_Check3);
	DDX_Control(pDX, IDC_CHECK4, m_Check4);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnStop);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnStart);
	DDX_Control(pDX, IDC_STATIC_TIPS, m_StaticTips);
}

BEGIN_MESSAGE_MAP(CXEngineAPPSimulateKeyInputDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CXEngineAPPSimulateKeyInputDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CXEngineAPPSimulateKeyInputDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &CXEngineAPPSimulateKeyInputDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CXEngineAPPSimulateKeyInputDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CXEngineAPPSimulateKeyInputDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CXEngineAPPSimulateKeyInputDlg::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CXEngineAPPSimulateKeyInputDlg 消息处理程序

BOOL CXEngineAPPSimulateKeyInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXEngineAPPSimulateKeyInputDlg::OnPaint()
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
HCURSOR CXEngineAPPSimulateKeyInputDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CALLBACK CXEngineAPPSimulateKeyInputDlg::Keyboard_ThreadCheck1(LPVOID lParam)
{
	CXEngineAPPSimulateKeyInputDlg* pClass_This = (CXEngineAPPSimulateKeyInputDlg*)lParam;
	CString m_StrTips;
	CString m_StrTime;
	__int64 nTimeCount = 0;

	srand((UINT)time(NULL));
	while (pClass_This->m_bRun)
	{
		int nRandom = 0;
		TCHAR tszWindowName[MAX_PATH];
		memset(tszWindowName, '\0', sizeof(tszWindowName));

		pClass_This->m_EditTime1.GetWindowText(m_StrTime);
		if (_ttoi(m_StrTime.GetBuffer()) > 0)
		{
			nRandom = _ttoi(m_StrTime.GetBuffer());
		}
		else
		{
			nRandom = rand() % 10 + 1;
		}
		//获取当前激活窗口的标题
		::GetWindowText(::GetForegroundWindow(), tszWindowName, sizeof(tszWindowName));
		LPCTSTR lpszWindowTitle = _T("魔兽世界");
		// 比较窗口标题
		if (0 == _tcsnicmp(lpszWindowTitle, tszWindowName, _tcslen(lpszWindowTitle)) || 0 == _tcsnicmp(lpszWindowTitle, tszWindowName + 1, _tcslen(lpszWindowTitle)))
		{
			m_StrTips.Format(_T("提示:当前为魔兽世界窗口,按键次数:%d,按键等待时间:%d"), nTimeCount++, nRandom);
			pClass_This->m_StaticTips.SetWindowText(m_StrTips);

			CString m_StrMsg;
			INPUT st_Inputs[2];
			ZeroMemory(st_Inputs, sizeof(INPUT) * 2);

			pClass_This->m_EditCheck1.GetWindowText(m_StrMsg);

			st_Inputs[0].type = INPUT_KEYBOARD;
			st_Inputs[0].ki.wVk = _ttoi(m_StrMsg.GetBuffer());

			st_Inputs[1].type = INPUT_KEYBOARD;
			st_Inputs[1].ki.wVk = _ttoi(m_StrMsg.GetBuffer());;
			st_Inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

			UINT nRet = SendInput(ARRAYSIZE(st_Inputs), st_Inputs, sizeof(INPUT));
			if (nRet != ARRAYSIZE(st_Inputs))
			{
				CString m_StrError;
				m_StrError.Format(_T("发送按键失败,错误:%d,%d"), nRet, GetLastError());
				AfxMessageBox(m_StrError);
			}
		}
		else
		{
			m_StrTips.Format(_T("提示:当前不是魔兽世界窗口,按键次数:%d,按键等待时间:%d"), nTimeCount, nRandom);
			pClass_This->m_StaticTips.SetWindowText(m_StrTips);
		}
		std::this_thread::sleep_for(std::chrono::seconds(nRandom));
	}
	return 0;
}
int CALLBACK CXEngineAPPSimulateKeyInputDlg::Keyboard_ThreadCheck2(LPVOID lParam)
{
	CXEngineAPPSimulateKeyInputDlg* pClass_This = (CXEngineAPPSimulateKeyInputDlg*)lParam;
	CString m_StrTips;
	CString m_StrTime;
	__int64 nTimeCount = 0;

	srand((UINT)time(NULL));
	while (pClass_This->m_bRun)
	{
		int nRandom = 0;
		TCHAR tszWindowName[MAX_PATH];
		memset(tszWindowName, '\0', sizeof(tszWindowName));

		pClass_This->m_EditTime2.GetWindowText(m_StrTime);
		if (_ttoi(m_StrTime.GetBuffer()) > 0)
		{
			nRandom = _ttoi(m_StrTime.GetBuffer());
		}
		else
		{
			nRandom = rand() % 10 + 1;
		}
		//获取当前激活窗口的标题
		::GetWindowText(::GetForegroundWindow(), tszWindowName, sizeof(tszWindowName));
		LPCTSTR lpszWindowTitle = _T("魔兽世界");
		// 比较窗口标题
		if (0 == _tcsnicmp(lpszWindowTitle, tszWindowName, _tcslen(lpszWindowTitle)) || 0 == _tcsnicmp(lpszWindowTitle, tszWindowName + 1, _tcslen(lpszWindowTitle)))
		{
			m_StrTips.Format(_T("提示:当前为魔兽世界窗口,按键次数:%d,按键等待时间:%d"), nTimeCount++, nRandom);
			pClass_This->m_StaticTips.SetWindowText(m_StrTips);

			CString m_StrMsg;
			INPUT st_Inputs[2];
			ZeroMemory(st_Inputs, sizeof(INPUT) * 2);

			pClass_This->m_EditCheck2.GetWindowText(m_StrMsg);

			st_Inputs[0].type = INPUT_KEYBOARD;
			st_Inputs[0].ki.wVk = _ttoi(m_StrMsg.GetBuffer());

			st_Inputs[1].type = INPUT_KEYBOARD;
			st_Inputs[1].ki.wVk = _ttoi(m_StrMsg.GetBuffer());;
			st_Inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

			UINT nRet = SendInput(ARRAYSIZE(st_Inputs), st_Inputs, sizeof(INPUT));
			if (nRet != ARRAYSIZE(st_Inputs))
			{
				CString m_StrError;
				m_StrError.Format(_T("发送按键失败,错误:%d,%d"), nRet, GetLastError());
				AfxMessageBox(m_StrError);
			}
		}
		else
		{
			m_StrTips.Format(_T("提示:当前不是魔兽世界窗口,按键次数:%d,按键等待时间:%d"), nTimeCount, nRandom);
			pClass_This->m_StaticTips.SetWindowText(m_StrTips);
		}
		std::this_thread::sleep_for(std::chrono::seconds(nRandom));
	}
	return 0;
}
int CALLBACK CXEngineAPPSimulateKeyInputDlg::Keyboard_ThreadCheck3(LPVOID lParam)
{
	CXEngineAPPSimulateKeyInputDlg* pClass_This = (CXEngineAPPSimulateKeyInputDlg*)lParam;
	CString m_StrTips;
	CString m_StrTime;
	__int64 nTimeCount = 0;

	srand((UINT)time(NULL));
	while (pClass_This->m_bRun)
	{
		int nRandom = 0;
		TCHAR tszWindowName[MAX_PATH];
		memset(tszWindowName, '\0', sizeof(tszWindowName));

		pClass_This->m_EditTime3.GetWindowText(m_StrTime);
		if (_ttoi(m_StrTime.GetBuffer()) > 0)
		{
			nRandom = _ttoi(m_StrTime.GetBuffer());
		}
		else
		{
			nRandom = rand() % 10 + 1;
		}
		//获取当前激活窗口的标题
		::GetWindowText(::GetForegroundWindow(), tszWindowName, sizeof(tszWindowName));
		LPCTSTR lpszWindowTitle = _T("魔兽世界");
		// 比较窗口标题
		if (0 == _tcsnicmp(lpszWindowTitle, tszWindowName, _tcslen(lpszWindowTitle)) || 0 == _tcsnicmp(lpszWindowTitle, tszWindowName + 1, _tcslen(lpszWindowTitle)))
		{
			m_StrTips.Format(_T("提示:当前为魔兽世界窗口,按键次数:%d,按键等待时间:%d"), nTimeCount++, nRandom);
			pClass_This->m_StaticTips.SetWindowText(m_StrTips);

			CString m_StrMsg;
			INPUT st_Inputs[2];
			ZeroMemory(st_Inputs, sizeof(INPUT) * 2);

			pClass_This->m_EditCheck3.GetWindowText(m_StrMsg);

			st_Inputs[0].type = INPUT_KEYBOARD;
			st_Inputs[0].ki.wVk = _ttoi(m_StrMsg.GetBuffer());

			st_Inputs[1].type = INPUT_KEYBOARD;
			st_Inputs[1].ki.wVk = _ttoi(m_StrMsg.GetBuffer());;
			st_Inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

			UINT nRet = SendInput(ARRAYSIZE(st_Inputs), st_Inputs, sizeof(INPUT));
			if (nRet != ARRAYSIZE(st_Inputs))
			{
				CString m_StrError;
				m_StrError.Format(_T("发送按键失败,错误:%d,%d"), nRet, GetLastError());
				AfxMessageBox(m_StrError);
			}
		}
		else
		{
			m_StrTips.Format(_T("提示:当前不是魔兽世界窗口,按键次数:%d,按键等待时间:%d"), nTimeCount, nRandom);
			pClass_This->m_StaticTips.SetWindowText(m_StrTips);
		}
		std::this_thread::sleep_for(std::chrono::seconds(nRandom));
	}
	return 0;
}
int CALLBACK CXEngineAPPSimulateKeyInputDlg::Keyboard_ThreadCheck4(LPVOID lParam)
{
	CXEngineAPPSimulateKeyInputDlg* pClass_This = (CXEngineAPPSimulateKeyInputDlg*)lParam;
	CString m_StrTips;
	CString m_StrTime;
	__int64 nTimeCount = 0;

	srand((UINT)time(NULL));
	while (pClass_This->m_bRun)
	{
		int nRandom = 0;
		TCHAR tszWindowName[MAX_PATH];
		memset(tszWindowName, '\0', sizeof(tszWindowName));

		pClass_This->m_EditTime4.GetWindowText(m_StrTime);
		if (_ttoi(m_StrTime.GetBuffer()) > 0)
		{
			nRandom = _ttoi(m_StrTime.GetBuffer());
		}
		else
		{
			nRandom = rand() % 10 + 1;
		}
		//获取当前激活窗口的标题
		::GetWindowText(::GetForegroundWindow(), tszWindowName, sizeof(tszWindowName));
		LPCTSTR lpszWindowTitle = _T("魔兽世界");
		// 比较窗口标题
		if (0 == _tcsnicmp(lpszWindowTitle, tszWindowName, _tcslen(lpszWindowTitle)) || 0 == _tcsnicmp(lpszWindowTitle, tszWindowName + 1, _tcslen(lpszWindowTitle)))
		{
			m_StrTips.Format(_T("提示:当前为魔兽世界窗口,按键次数:%d,按键等待时间:%d"), nTimeCount++, nRandom);
			pClass_This->m_StaticTips.SetWindowText(m_StrTips);

			CString m_StrMsg;
			INPUT st_Inputs[2];
			ZeroMemory(st_Inputs, sizeof(INPUT) * 2);

			pClass_This->m_EditCheck4.GetWindowText(m_StrMsg);

			st_Inputs[0].type = INPUT_KEYBOARD;
			st_Inputs[0].ki.wVk = _ttoi(m_StrMsg.GetBuffer());

			st_Inputs[1].type = INPUT_KEYBOARD;
			st_Inputs[1].ki.wVk = _ttoi(m_StrMsg.GetBuffer());;
			st_Inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

			UINT nRet = SendInput(ARRAYSIZE(st_Inputs), st_Inputs, sizeof(INPUT));
			if (nRet != ARRAYSIZE(st_Inputs))
			{
				CString m_StrError;
				m_StrError.Format(_T("发送按键失败,错误:%d,%d"), nRet, GetLastError());
				AfxMessageBox(m_StrError);
			}
		}
		else
		{
			m_StrTips.Format(_T("提示:当前不是魔兽世界窗口,按键次数:%d,按键等待时间:%d"), nTimeCount, nRandom);
			pClass_This->m_StaticTips.SetWindowText(m_StrTips);
		}
		std::this_thread::sleep_for(std::chrono::seconds(nRandom));
	}
	return 0;
}

void CXEngineAPPSimulateKeyInputDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == m_Check1.GetCheck())
	{
		m_bRun = true;
		pSTD_ThreadCheck1 = make_unique<thread>(Keyboard_ThreadCheck1, this);

		if (NULL == pSTD_ThreadCheck1)
		{
			AfxMessageBox(_T("启动失败"));
		}
	}
	if (BST_CHECKED == m_Check2.GetCheck())
	{
		m_bRun = true;
		pSTD_ThreadCheck2 = make_unique<thread>(Keyboard_ThreadCheck2, this);

		if (NULL == pSTD_ThreadCheck2)
		{
			AfxMessageBox(_T("启动失败"));
		}
	}
	if (BST_CHECKED == m_Check3.GetCheck())
	{
		m_bRun = true;
		pSTD_ThreadCheck3 = make_unique<thread>(Keyboard_ThreadCheck3, this);

		if (NULL == pSTD_ThreadCheck3)
		{
			AfxMessageBox(_T("启动失败"));
		}
	}
	if (BST_CHECKED == m_Check4.GetCheck())
	{
		m_bRun = true;
		pSTD_ThreadCheck4 = make_unique<thread>(Keyboard_ThreadCheck4, this);

		if (NULL == pSTD_ThreadCheck4)
		{
			AfxMessageBox(_T("启动失败"));
		}
	}
	m_BtnStart.EnableWindow(FALSE);
	m_BtnStop.EnableWindow(TRUE);
}


void CXEngineAPPSimulateKeyInputDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRun = false;

	if (NULL != pSTD_ThreadCheck1)
	{
		if (pSTD_ThreadCheck1->joinable())
		{
			pSTD_ThreadCheck1->join();
		}
	}
	if (NULL != pSTD_ThreadCheck2)
	{
		if (pSTD_ThreadCheck2->joinable())
		{
			pSTD_ThreadCheck2->join();
		}
	}
	if (NULL != pSTD_ThreadCheck3)
	{
		if (pSTD_ThreadCheck3->joinable())
		{
			pSTD_ThreadCheck3->join();
		}
	}
	if (NULL != pSTD_ThreadCheck4)
	{
		if (pSTD_ThreadCheck4->joinable())
		{
			pSTD_ThreadCheck4->join();
		}
	}
	m_BtnStart.EnableWindow(TRUE);
	m_BtnStop.EnableWindow(FALSE);
}


void CXEngineAPPSimulateKeyInputDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == m_Check1.GetCheck())
	{
		m_EditCheck1.EnableWindow(TRUE);
		m_EditTime1.EnableWindow(TRUE);
	}
	else
	{
		m_EditCheck1.EnableWindow(FALSE);
		m_EditTime1.EnableWindow(FALSE);
	}
}


void CXEngineAPPSimulateKeyInputDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == m_Check2.GetCheck())
	{
		m_EditCheck2.EnableWindow(TRUE);
		m_EditTime2.EnableWindow(TRUE);
	}
	else
	{
		m_EditCheck2.EnableWindow(FALSE);
		m_EditTime2.EnableWindow(FALSE);
	}
}


void CXEngineAPPSimulateKeyInputDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == m_Check3.GetCheck())
	{
		m_EditCheck3.EnableWindow(TRUE);
		m_EditTime3.EnableWindow(TRUE);
	}
	else
	{
		m_EditCheck3.EnableWindow(FALSE);
		m_EditTime3.EnableWindow(FALSE);
	}
}


void CXEngineAPPSimulateKeyInputDlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == m_Check4.GetCheck())
	{
		m_EditCheck4.EnableWindow(TRUE);
		m_EditTime4.EnableWindow(TRUE);
	}
	else
	{
		m_EditCheck4.EnableWindow(FALSE);
		m_EditTime4.EnableWindow(FALSE);
	}
}
