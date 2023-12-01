﻿// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#ifdef _MSC_BUILD
// 添加要在此处预编译的标头
#include "framework.h"
#include <tchar.h>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/write_resume_data.hpp>
#endif
#endif //PCH_H
#include <iostream>
#include <thread>
#include <fstream>
#include <list>
#include <shared_mutex>
#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/bdecode.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/announce_entry.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/create_torrent.hpp>
#include <libtorrent/file_storage.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/write_resume_data.hpp>
using namespace std;
#include "../../XEngine_CommHdr.h"
#include "../../XEngine_Types.h"
#include "../../XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "BTorrent_Define.h"
#include "BTorrent_Error.h"
//////////////////////////////////////////////////////////////////////////
//                       用户不用关心的数据
//////////////////////////////////////////////////////////////////////////
extern XBOOL BTDload_IsErrorOccur;
extern XLONG BTDload_dwErrorCode;


#ifdef _MSC_BUILD
#ifdef _DEBUG
#pragma comment(lib,"../../Debug/XEngine_BaseLib.lib")
#else
#ifdef _WIN64
#pragma comment(lib,"../../x64/Release/XEngine_BaseLib.lib")
#else
#pragma comment(lib,"../../Release/XEngine_BaseLib.lib")
#endif
#endif
#pragma comment(lib,"torrent-rasterbar")
#else
#ifdef __linux__
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
#endif