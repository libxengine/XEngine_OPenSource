// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H
#ifdef _MSC_BUILD
// 添加要在此处预编译的标头
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include <tchar.h>
#endif
#endif //PCH_H
#include <string>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include "PluginVideo_Define.h"
#include "PluginVideo_Error.h"
/********************************************************************
//    Created:     2026/06/01  00:00:00
//    File Name:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook\pch.h
//    File Path:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine_AVCodecApp(音视频编解码插件示例)
//    Author:      qyt
//    Purpose:     公用头文件
//    History:
*********************************************************************/
extern bool VideoHook_IsErrorOccur;
extern XLONG VideoHook_dwErrorCode;


#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#endif