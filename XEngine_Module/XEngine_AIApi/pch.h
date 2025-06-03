// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H
#ifdef _MSC_BUILD
// 添加要在此处预编译的标头
#include "framework.h"
#endif
#endif //PCH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json/json.h>
#include <thread>
#include <memory>
#include <list>
#include <shared_mutex>
#include <unordered_map>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_BaseLib/BaseSafe_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseSafe_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include "AIApi_Define.h"
#include "AIApi_Error.h"
/********************************************************************
//    Created:     2025/05/29  13:58:10
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\pch.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     公用函数
//    History:
*********************************************************************/
extern bool AIApi_IsErrorOccur;
extern XLONG AIApi_dwErrorCode;

#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseSafe")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"x64/Debug/jsoncpp")
#elif _M_ARM64
#pragma comment(lib,"ARM64/Debug/jsoncpp")
#elif _M_IX86
#pragma comment(lib,"Debug/jsoncpp")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"x64/Release/jsoncpp")
#elif _M_ARM64
#pragma comment(lib,"ARM64/Release/jsoncpp")
#elif _M_IX86
#pragma comment(lib,"Release/jsoncpp")
#endif
#endif
#endif