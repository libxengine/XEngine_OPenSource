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
#include <errno.h>
#include <memory>
#include <SDL2/SDL.h>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include "AVPlayer_Define.h"
#include "AVPlayer_Error.h"
/********************************************************************
//	Created:	2018/10/8   14:51
//	Filename: 	E:\XEngine_Windows\XEngine_SourceCode\XEngine_AvCoder\XEngine_AVPlayer\AVPlayer_CommHdr.h
//	File Path:	E:\XEngine_Windows\XEngine_SourceCode\XEngine_AvCoder\XEngine_AVPlayer
//	File Base:	AVPlayer_CommHdr
//	File Ext:	h
//  Project:    XEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	公用头文件
//	History:
*********************************************************************/
extern bool AVPlayer_IsErrorOccur;
extern XLONG AVPlayer_dwErrorCode;


#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#else
#ifdef __linux__
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
#endif
