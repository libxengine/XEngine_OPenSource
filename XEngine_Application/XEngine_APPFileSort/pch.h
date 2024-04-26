// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"
#include <string>
#include <list>
#include <thread>
#include <memory>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
using namespace std;
#endif //PCH_H
/********************************************************************
//    Created:     2024/04/26  17:18:44
//    File Name:   D:\XEngine_OPenSource\XEngine_Application\XEngine_APPFileSort\pch.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Application\XEngine_APPFileSort
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公用头
//    History:
*********************************************************************/
typedef struct
{
	TCHAR tszFileSrc[MAX_PATH];
	TCHAR tszFileDst[MAX_PATH];
}FILESORT_INFO;

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi")