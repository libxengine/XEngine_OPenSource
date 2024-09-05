// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#endif //PCH_H
#include <string>
#include <list>
#include <thread>
#include <memory>
#include <atlconv.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
using namespace std;
/********************************************************************
//    Created:     2024/09/02  14:45:39
//    File Name:   D:\XEngine_OPenSource\XEngine_Application\XEngine_APPSocketTest\pch.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Application\XEngine_APPSocketTest
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     公用头
//    History:
*********************************************************************/
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_Core")
#pragma comment(lib,"XEngine_Client/XClient_Socket")