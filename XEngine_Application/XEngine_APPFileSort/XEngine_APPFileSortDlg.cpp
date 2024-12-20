
// XEngine_APPFileSortDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "XEngine_APPFileSort.h"
#include "XEngine_APPFileSortDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXEngineAPPFileSortDlg 对话框



CXEngineAPPFileSortDlg::CXEngineAPPFileSortDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XENGINE_APPFILESORT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXEngineAPPFileSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditSelectDir);
	DDX_Control(pDX, IDC_LIST1, m_ListFile);
	DDX_Control(pDX, IDC_EDIT2, m_EditNumber);
	DDX_Control(pDX, IDC_STATIC_TIPS, m_StaticTips);
}

BEGIN_MESSAGE_MAP(CXEngineAPPFileSortDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CXEngineAPPFileSortDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CXEngineAPPFileSortDlg::OnBnClickedButton2)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CXEngineAPPFileSortDlg 消息处理程序

BOOL CXEngineAPPFileSortDlg::OnInitDialog()
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
	m_ListFile.InsertColumn(3, _T("替换结果"), LVCFMT_LEFT, 80);
	m_EditNumber.SetWindowText(_T("1"));

	bThread = true;
	pSTDThread = std::make_unique<std::thread>(XEngine_FileSort_Thread, this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXEngineAPPFileSortDlg::OnPaint()
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
HCURSOR CXEngineAPPFileSortDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool compareNumericString(const std::string& strSource, const std::string& strDest)
{
	// 解析字符串中的数字部分
	XCHAR tszSrcName[MAX_PATH] = {};
	XCHAR tszDstName[MAX_PATH] = {};
	BaseLib_String_GetFileAndPath(strSource.c_str(), NULL, tszSrcName);
	BaseLib_String_GetFileAndPath(strDest.c_str(), NULL, tszDstName);

	XCHAR tszSrcKey[MAX_PATH] = {};
	XCHAR tszSrcVlu[MAX_PATH] = {};
	XCHAR tszDstKey[MAX_PATH] = {};
	XCHAR tszDstVlu[MAX_PATH] = {};

	BaseLib_String_GetKeyValue(tszSrcName, _X("."), tszSrcKey, tszSrcVlu);
	BaseLib_String_GetKeyValue(tszDstName, _X("."), tszDstKey, tszDstVlu);
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

void CXEngineAPPFileSortDlg::XEngine_FileSort_Thread(LPVOID lParam)
{
	CXEngineAPPFileSortDlg* pClass_This = (CXEngineAPPFileSortDlg*)lParam;

	while (pClass_This->bThread)
	{
		if (!pClass_This->bThreadList && (NULL != pClass_This->pSTDThreadList))
		{
			pClass_This->pSTDThreadList->join();
			pClass_This->pSTDThreadList = NULL;
		}
		if (!pClass_This->bThreadName && (NULL != pClass_This->pSTDThreadName))
		{
			pClass_This->pSTDThreadName->join();
			pClass_This->pSTDThreadName = NULL;
		}
		Sleep(1);
	}
}
void CXEngineAPPFileSortDlg::XEngine_FileSort_ThreadList(LPVOID lParam)
{
	CXEngineAPPFileSortDlg* pClass_This = (CXEngineAPPFileSortDlg*)lParam;

	int nHideCount = 0;
	int nListCount = 0;
	CString m_StrPath;
	XCHAR** ppszListDir = NULL;

	pClass_This->m_EditSelectDir.GetWindowText(m_StrPath);
	if ('\\' == m_StrPath.GetBuffer()[m_StrPath.GetLength() - 1])
	{
		m_StrPath.Append(_T("*"));
	}
	else
	{
		m_StrPath.Append(_T("\\*"));
	}
	CString m_StrLog;
	pClass_This->m_StaticTips.SetWindowText(_T("提示:开始查找所有文件"));

	USES_CONVERSION;
	SystemApi_File_EnumFile(W2A(m_StrPath.GetBuffer()), &ppszListDir, &nListCount, false, 1);
	list<string> stl_ListFile;

	m_StrLog.Format(_T("提示:正在对文件进行排序,一共有%d 个文件"), nListCount);
	pClass_This->m_StaticTips.SetWindowText(m_StrLog);

	for (int i = 0; i < nListCount; i++)
	{
		SYSTEMAPI_FILE_ATTR st_FileAttr = {};
		SystemApi_File_GetFileAttr(ppszListDir[i], &st_FileAttr);
		if (st_FileAttr.bHidden)
		{
			nHideCount++;
		}
		else
		{
			stl_ListFile.push_back(ppszListDir[i]);
		}
	}
	BaseLib_Memory_Free((XPPPMEM)&ppszListDir, nListCount);

	stl_ListFile.sort(compareNumericString);

	int j = 0;
	CString m_StrNumber;
	pClass_This->m_EditNumber.GetWindowText(m_StrNumber);

	j = _ttoi(m_StrNumber.GetBuffer());
	auto stl_ListIterator = stl_ListFile.begin();
	for (int i = 0; stl_ListIterator != stl_ListFile.end(); stl_ListIterator++, i++, j++)
	{
		TCHAR tszIndexStr[MAX_PATH] = {};
		CHAR tszFileName[MAX_PATH] = {};
		CHAR tszFilePath[MAX_PATH] = {};
		CHAR tszFileExt[MAX_PATH] = {};

		BaseLib_String_GetFileAndPath(stl_ListIterator->c_str(), tszFilePath, NULL, NULL, tszFileExt);

		_stprintf(tszIndexStr, _T("%d"), i);
		sprintf(tszFileName, "%s%d.%s", tszFilePath, j, tszFileExt);

		pClass_This->m_ListFile.InsertItem(i, _T(""));
		pClass_This->m_ListFile.SetItemText(i, 0, tszIndexStr);
#ifdef _UNICODE
		int nGBKLen = 0;
		wchar_t tszUNCSrc[MAX_PATH] = {};
		wchar_t tszUNCDst[MAX_PATH] = {};
		nGBKLen = stl_ListIterator->length();
		BaseLib_Charset_AnsiToUnicode(stl_ListIterator->c_str(), tszUNCSrc, &nGBKLen);

		nGBKLen = strlen(tszFileName);
		BaseLib_Charset_AnsiToUnicode(tszFileName, tszUNCDst, &nGBKLen);
		pClass_This->m_ListFile.SetItemText(i, 1, tszUNCSrc);
		pClass_This->m_ListFile.SetItemText(i, 2, tszUNCDst);
#else
		pClass_This->m_ListFile.SetItemText(i, 1, stl_ListIterator->c_str());
		pClass_This->m_ListFile.SetItemText(i, 2, tszFileName);
#endif
		m_StrLog.Format(_T("提示:正在插入文件到列表,总个数:%d,当前:%d"), stl_ListFile.size(), i);
		pClass_This->m_StaticTips.SetWindowText(m_StrLog);
	}
	m_StrLog.Format(_T("提示:插入文件到列表成功,总个数:%d,忽略的文件个数:%d"), stl_ListFile.size(), nHideCount);
	pClass_This->m_StaticTips.SetWindowText(m_StrLog);
	pClass_This->bThreadList = false;
}
void CXEngineAPPFileSortDlg::XEngine_FileSort_ThreadName(LPVOID lParam)
{
	CXEngineAPPFileSortDlg* pClass_This = (CXEngineAPPFileSortDlg*)lParam;

	CString m_StrLastSrc = pClass_This->m_ListFile.GetItemText(pClass_This->m_ListFile.GetItemCount() - 1, 1);
	CString m_StrLastDst = pClass_This->m_ListFile.GetItemText(pClass_This->m_ListFile.GetItemCount() - 1, 2);

	XCHAR tszFileSrc[MAX_PATH] = {};
	XCHAR tszFileDst[MAX_PATH] = {};
	USES_CONVERSION;
	BaseLib_String_GetFileAndPath(W2A(m_StrLastSrc.GetBuffer()), NULL, tszFileSrc);
	BaseLib_String_GetFileAndPath(W2A(m_StrLastDst.GetBuffer()), NULL, tszFileDst);

	XCHAR tszNameSrc[MAX_PATH] = {};
	XCHAR tszNameDst[MAX_PATH] = {};
	BaseLib_String_GetKeyValue(tszFileSrc, ".", tszNameSrc);
	BaseLib_String_GetKeyValue(tszFileDst, ".", tszNameDst);

	if (atoi(tszNameSrc) > atoi(tszNameDst))
	{
		for (int i = 0; i < pClass_This->m_ListFile.GetItemCount(); i++)
		{
			CString m_StrSource = pClass_This->m_ListFile.GetItemText(i, 1);
			CString m_StrDest = pClass_This->m_ListFile.GetItemText(i, 2);
			//他们两个文件名称是否一样
			if (0 == _tcsnicmp(m_StrSource.GetBuffer(), m_StrDest.GetBuffer(), m_StrSource.GetLength()))
			{
				//相同文件不处理
				pClass_This->m_ListFile.SetItemText(i, 3, _T("相同"));
			}
			else
			{
				//不相同,开始改名
				if (0 == _taccess(m_StrDest.GetBuffer(), 0))
				{
					pClass_This->m_ListFile.SetItemText(i, 3, _T("文件存在"));
				}
				else
				{
					if (0 == _trename(m_StrSource.GetBuffer(), m_StrDest.GetBuffer()))
					{
						pClass_This->m_ListFile.SetItemText(i, 3, _T("成功"));
					}
					else
					{
						pClass_This->m_ListFile.SetItemText(i, 3, _T("失败"));
					}
				}
			}
			CString m_StrLog;
			m_StrLog.Format(_T("提示:正在进行重命名文件,总个数:%d,当前:%d"), pClass_This->m_ListFile.GetItemCount(), i);
			pClass_This->m_StaticTips.SetWindowText(m_StrLog);
		}
	}
	else
	{
		for (int i = pClass_This->m_ListFile.GetItemCount() - 1; i >= 0; i--)
		{
			CString m_StrSource = pClass_This->m_ListFile.GetItemText(i, 1);
			CString m_StrDest = pClass_This->m_ListFile.GetItemText(i, 2);
			//他们两个文件名称是否一样
			if (0 == _tcsnicmp(m_StrSource.GetBuffer(), m_StrDest.GetBuffer(), m_StrSource.GetLength()))
			{
				//相同文件不处理
				pClass_This->m_ListFile.SetItemText(i, 3, _T("相同"));
			}
			else
			{
				//不相同,开始改名
				if (0 == _taccess(m_StrDest.GetBuffer(), 0))
				{
					pClass_This->m_ListFile.SetItemText(i, 3, _T("文件存在"));
				}
				else
				{
					if (0 == _trename(m_StrSource.GetBuffer(), m_StrDest.GetBuffer()))
					{
						pClass_This->m_ListFile.SetItemText(i, 3, _T("成功"));
					}
					else
					{
						pClass_This->m_ListFile.SetItemText(i, 3, _T("失败"));
					}
				}
			}
			CString m_StrLog;
			m_StrLog.Format(_T("提示:正在进行重命名文件,总个数:%d,当前:%d"), pClass_This->m_ListFile.GetItemCount(), i);
			pClass_This->m_StaticTips.SetWindowText(m_StrLog);
		}
	}
	CString m_StrLog;
	m_StrLog.Format(_T("提示:重命名文件成功,总个数:%d"), pClass_This->m_ListFile.GetItemCount());
	pClass_This->m_StaticTips.SetWindowText(m_StrLog);
	pClass_This->bThreadName = false;
}
void CXEngineAPPFileSortDlg::OnBnClickedButton1()
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

	bThreadList = true;
	m_StaticTips.SetWindowText(_T("开始搜索文件"));
	pSTDThreadList = std::make_unique<std::thread>(XEngine_FileSort_ThreadList, this);
}


void CXEngineAPPFileSortDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	bThreadName = true;
	m_StaticTips.SetWindowText(_T("开始重命名文件"));
	pSTDThreadName = std::make_unique<std::thread>(XEngine_FileSort_ThreadName, this);
}


void CXEngineAPPFileSortDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	bThread = false;
	pSTDThread->join();
}
