﻿
// XEngine_FileSortDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "XEngine_FileSort.h"
#include "XEngine_FileSortDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXEngineFileSortDlg 对话框



CXEngineFileSortDlg::CXEngineFileSortDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XENGINE_FILESORT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXEngineFileSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditSelectDir);
	DDX_Control(pDX, IDC_LIST1, m_ListFile);
	DDX_Control(pDX, IDC_EDIT2, m_EditNumber);
	DDX_Control(pDX, IDC_STATIC_TIPS, m_StaticTips);
}

BEGIN_MESSAGE_MAP(CXEngineFileSortDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CXEngineFileSortDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CXEngineFileSortDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CXEngineFileSortDlg 消息处理程序

BOOL CXEngineFileSortDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ListFile.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50);
	m_ListFile.InsertColumn(1, _T("原始文件"), LVCFMT_LEFT, 300);
	m_ListFile.InsertColumn(2, _T("目标文件"), LVCFMT_LEFT, 300);
	m_ListFile.InsertColumn(3, _T("替换结果"), LVCFMT_LEFT, 100);
	m_EditNumber.SetWindowText(_T("1"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXEngineFileSortDlg::OnPaint()
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
HCURSOR CXEngineFileSortDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool compareNumericString(const std::string& strSource, const std::string& strDest)
{
	// 解析字符串中的数字部分
	XCHAR tszSrcName[MAX_PATH] = {};
	XCHAR tszDstName[MAX_PATH] = {};
	BaseLib_OperatorString_GetFileAndPath(strSource.c_str(), NULL, tszSrcName);
	BaseLib_OperatorString_GetFileAndPath(strDest.c_str(), NULL, tszDstName);

	XCHAR tszSrcKey[MAX_PATH] = {};
	XCHAR tszSrcVlu[MAX_PATH] = {};
	XCHAR tszDstKey[MAX_PATH] = {};
	XCHAR tszDstVlu[MAX_PATH] = {};

	BaseLib_OperatorString_GetKeyValue(tszSrcName, _X("."), tszSrcKey, tszSrcVlu);
	BaseLib_OperatorString_GetKeyValue(tszDstName, _X("."), tszDstKey, tszDstVlu);
	int nValueSrc = atoi(tszSrcKey);
	int nVlaueDst = atoi(tszDstKey);
	// 如果数字不相等，则按数字排序
	if (nValueSrc != nVlaueDst)
	{
		return nValueSrc < nVlaueDst;
	}
	// 如果数字相等，则按字符串部分排序
	return strSource < strDest;
}

void CXEngineFileSortDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO st_BrowseInfo = { 0 };
	TCHAR tszDIRBuffer[MAX_PATH] = {};
	st_BrowseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

	LPITEMIDLIST pSt_ItemList = SHBrowseForFolder(&st_BrowseInfo);
	
	if (NULL == pSt_ItemList)
	{
		return;
	}
	if (!SHGetPathFromIDList(pSt_ItemList, tszDIRBuffer))
	{
		return;
	}
	m_ListFile.DeleteAllItems();
	m_EditSelectDir.SetWindowText(tszDIRBuffer);
	CoTaskMemFree(pSt_ItemList);

	int nListCount = 0;
	XCHAR** ppszListDir = NULL; 

	_tcscat(tszDIRBuffer, _T("\\*"));
	USES_CONVERSION;
	SystemApi_File_EnumFile(W2A(tszDIRBuffer), &ppszListDir, &nListCount, false, 1);
	list<string> stl_ListFile;

	for (int i = 0; i < nListCount; i++)
	{
		stl_ListFile.push_back(ppszListDir[i]);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListDir, nListCount);

	stl_ListFile.sort(compareNumericString);

	int j = 0;
	CString m_StrNumber;
	m_EditNumber.GetWindowText(m_StrNumber);

	j = _ttoi(m_StrNumber.GetBuffer());
	auto stl_ListIterator = stl_ListFile.begin();
	for (int i = 0; stl_ListIterator != stl_ListFile.end(); stl_ListIterator++, i++, j++)
	{
		TCHAR tszIndexStr[MAX_PATH] = {};
		CHAR tszFileName[MAX_PATH] = {};
		CHAR tszFilePath[MAX_PATH] = {};
		CHAR tszFileExt[MAX_PATH] = {};

		BaseLib_OperatorString_GetFileAndPath(stl_ListIterator->c_str(), tszFilePath, NULL, NULL, tszFileExt);

		_stprintf(tszIndexStr, _T("%d"), i);
		sprintf(tszFileName, "%s%d.%s", tszFilePath, j, tszFileExt);

		m_ListFile.InsertItem(i, _T(""));
		m_ListFile.SetItemText(i, 0, tszIndexStr);
		m_ListFile.SetItemText(i, 1, A2W(stl_ListIterator->c_str()));
		m_ListFile.SetItemText(i, 2, A2W(tszFileName));
	}
}

void CXEngineFileSortDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	
	list<FILESORT_INFO> m_ListBackFile;
	for (int i = 0; i < m_ListFile.GetItemCount(); i++)
	{
		FILESORT_INFO st_FileInfo = {};
		CString m_StrSource = m_ListFile.GetItemText(i, 1);
		CString m_StrDest = m_ListFile.GetItemText(i, 2);
		//他们两个文件名称是否一样
		if (0 == _tcsnicmp(m_StrSource.GetBuffer(), m_StrDest.GetBuffer(), m_StrSource.GetLength()))
		{
			//相同文件不处理
			m_ListFile.SetItemText(i, 3, _T("相同"));
		}
		else
		{
			//不相同,开始改名
			if (0 == _taccess(m_StrDest.GetBuffer(), 0))
			{
				m_ListFile.SetItemText(i, 3, _T("文件存在"));
			}
			else
			{
				if (0 == _trename(m_StrSource.GetBuffer(), m_StrDest.GetBuffer()))
				{
					m_ListFile.SetItemText(i, 3, _T("成功"));
				}
				else
				{
					m_ListFile.SetItemText(i, 3, _T("失败"));
				}
			}
		}
	}
}