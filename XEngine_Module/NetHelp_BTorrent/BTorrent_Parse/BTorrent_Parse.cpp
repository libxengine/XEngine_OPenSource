#include "pch.h"
#include "BTorrent_Parse.h"
/********************************************************************
//    Created:     2023/02/02  16:26:41
//    File Name:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Parse\BTorrent_Parse.cpp
//    File Path:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Parse
//    File Base:   BTorrent_Parse
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     种子解析工具
//    History:
*********************************************************************/
CBTorrent_Parse::CBTorrent_Parse()
{
}
CBTorrent_Parse::~CBTorrent_Parse()
{
}
//////////////////////////////////////////////////////////////////////////
//                        公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：BTorrent_Parse_Init
函数功能：初始化一个解析器
 参数.一：pxhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：导出一个下载句柄
 参数.二：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：种子地址,可以是本地种子文件路径或者磁力链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CBTorrent_Parse::BTorrent_Parse_Init(XNETHANDLE* pxhToken, LPCXSTR lpszAddr)
{
    BTDload_IsErrorOccur = FALSE;

    if ((NULL == pxhToken) || (NULL == lpszAddr))
    {
        BTDload_IsErrorOccur = TRUE;
        BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_INIT_PARAMENT;
        return FALSE;
    }
	//申请空间
	BTORRENT_PARSEINFO* pSt_BTParse = new BTORRENT_PARSEINFO;
    if (NULL == pSt_BTParse)
    {
        BTDload_IsErrorOccur = TRUE;
        BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_INIT_MALLOC;
        return FALSE;
    }
    memset(pSt_BTParse,'\0',sizeof(BTORRENT_PARSEINFO));

	LPCXSTR lpszFindStr = _T("magnet:?xt=urn:");
	if (NULL == _tcsstr(lpszAddr, lpszFindStr))
	{
		std::fstream m_FStream;
		m_FStream.exceptions(std::ifstream::failbit);
		m_FStream.open(lpszAddr, std::ios_base::in | std::ios_base::binary);
		if (!m_FStream)
		{
			BTDload_IsErrorOccur = TRUE;
			BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_INIT_MALLOC;
			delete pSt_BTParse;
			pSt_BTParse = NULL;
			return FALSE;
		}
		m_FStream.seekg(0, std::ios_base::end);
		size_t nSize = size_t(m_FStream.tellg());
		m_FStream.seekg(0, std::ios_base::beg);
		std::vector<char> stl_ListBuffer(nSize);
		m_FStream.read(stl_ListBuffer.data(), nSize);

		int nPos = -1;
		lt::error_code m_BTError;
		lt::load_torrent_limits st_BTLimits;
		pSt_BTParse->m_BTBNode = lt::bdecode(stl_ListBuffer, m_BTError, &nPos, st_BTLimits.max_decode_depth, st_BTLimits.max_decode_tokens);
		pSt_BTParse->m_BTInfo = std::make_shared<lt::torrent_info>(std::move(pSt_BTParse->m_BTBNode), st_BTLimits);
	}
	else
	{
		//如果是磁力
	}
	
	BaseLib_OperatorHandle_Create(pxhToken);
    st_Locker.lock();
    stl_MapBTParse.insert(make_pair(*pxhToken, pSt_BTParse));
    st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：BTorrent_Parse_GetNode
函数功能：获取节点信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pppSt_Parse
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出解析到的节点列表,此参数需要BaseLib_OperatorMemory_Free释放内存
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CBTorrent_Parse::BTorrent_Parse_GetNode(XNETHANDLE xhToken, BTORRENT_PARSEMAP*** pppSt_Parse, int* pInt_ListCount)
{
    BTDload_IsErrorOccur = FALSE;

	if ((NULL == pppSt_Parse) || (NULL == pInt_ListCount))
	{
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETNODE_PARAMENT;
		return FALSE;
	}
    st_Locker.lock_shared();
    unordered_map<XNETHANDLE, BTORRENT_PARSEINFO*>::const_iterator stl_MapIterator = stl_MapBTParse.find(xhToken);
    if (stl_MapIterator == stl_MapBTParse.end())
    {
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETNODE_NOTFOUND;
        st_Locker.unlock_shared();
        return FALSE;
    }
    *pInt_ListCount = stl_MapIterator->second->m_BTInfo->nodes().size();
    BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_Parse, stl_MapIterator->second->m_BTInfo->nodes().size(), sizeof(BTORRENT_PARSEMAP));
    std::vector<std::pair<std::string, int>>::const_iterator stl_ListIterator = stl_MapIterator->second->m_BTInfo->nodes().begin();
    for (int i = 0; stl_ListIterator != stl_MapIterator->second->m_BTInfo->nodes().end(); stl_ListIterator++, i++)
    {
        (*pppSt_Parse)[i]->nValue = stl_ListIterator->second;
        _tcscpy((*pppSt_Parse)[i]->tszValue, stl_ListIterator->first.c_str());
    }
    st_Locker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：BTorrent_Parse_GetTracker
函数功能：获取TRACKER服务器列表
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pppSt_Parse
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出解析到的列表,此参数需要BaseLib_OperatorMemory_Free释放内存
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CBTorrent_Parse::BTorrent_Parse_GetTracker(XNETHANDLE xhToken, BTORRENT_PARSEMAP*** pppSt_Parse, int* pInt_ListCount)
{
    BTDload_IsErrorOccur = FALSE;

	if ((NULL == pppSt_Parse) || (NULL == pInt_ListCount))
	{
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETTRACKER_PARAMENT;
		return FALSE;
	}
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_PARSEINFO*>::const_iterator stl_MapIterator = stl_MapBTParse.find(xhToken);
	if (stl_MapIterator == stl_MapBTParse.end())
	{
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETTRACKER_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	int i = 0;
	*pInt_ListCount = stl_MapIterator->second->m_BTInfo->trackers().size();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_Parse, stl_MapIterator->second->m_BTInfo->trackers().size(), sizeof(BTORRENT_PARSEMAP));
    
    for (auto const& stl_ListIterator : stl_MapIterator->second->m_BTInfo->trackers())
    {
		(*pppSt_Parse)[i]->nValue = stl_ListIterator.tier;
		_tcscpy((*pppSt_Parse)[i]->tszValue, stl_ListIterator.url.c_str());
		i++;
    }
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：BTorrent_Parse_GetSeeds
函数功能：获取种子列表
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pppSt_Parse
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出解析到的列表,此参数需要BaseLib_OperatorMemory_Free释放内存
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CBTorrent_Parse::BTorrent_Parse_GetSeeds(XNETHANDLE xhToken, BTORRENT_PARSEMAP*** pppSt_Parse, int* pInt_ListCount)
{
	BTDload_IsErrorOccur = FALSE;

	if ((NULL == pppSt_Parse) || (NULL == pInt_ListCount))
	{
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETSEED_PARAMENT;
		return FALSE;
	}
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_PARSEINFO*>::const_iterator stl_MapIterator = stl_MapBTParse.find(xhToken);
	if (stl_MapIterator == stl_MapBTParse.end())
	{
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETSEED_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	int i = 0;
	*pInt_ListCount = stl_MapIterator->second->m_BTInfo->web_seeds().size();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_Parse, stl_MapIterator->second->m_BTInfo->web_seeds().size(), sizeof(BTORRENT_PARSEMAP));
	
	for (auto const& stl_ListIterator : stl_MapIterator->second->m_BTInfo->web_seeds())
	{
		(*pppSt_Parse)[i]->nValue = stl_ListIterator.type;
		_tcscpy((*pppSt_Parse)[i]->tszValue, stl_ListIterator.url.c_str());
		i++;
	}
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：BTorrent_Parse_GetPiece
函数功能：获取块信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pInt_PieceLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出块大小
 参数.三：pInt_PieceCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出块个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CBTorrent_Parse::BTorrent_Parse_GetPiece(XNETHANDLE xhToken, int* pInt_PieceLen, int* pInt_PieceCount)
{
    BTDload_IsErrorOccur = FALSE;

	if ((NULL == pInt_PieceLen) || (NULL == pInt_PieceCount))
	{
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETSEED_PARAMENT;
		return FALSE;
	}
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_PARSEINFO*>::const_iterator stl_MapIterator = stl_MapBTParse.find(xhToken);
	if (stl_MapIterator == stl_MapBTParse.end())
	{
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETSEED_PARAMENT;
		st_Locker.unlock_shared();
		return FALSE;
	}
    *pInt_PieceLen = stl_MapIterator->second->m_BTInfo->piece_length();
    *pInt_PieceCount = stl_MapIterator->second->m_BTInfo->num_pieces();
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：BTorrent_Parse_GetInfo
函数功能：获取块信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：ptszHash
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出HASH值
 参数.三：ptszCreator
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出创建者
 参数.四：ptszComment
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出种子的注释信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CBTorrent_Parse::BTorrent_Parse_GetInfo(XNETHANDLE xhToken, XCHAR* ptszHash /* = NULL */, XCHAR* ptszCreator /* = NULL */, XCHAR* ptszComment /* = NULL */)
{
	BTDload_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_PARSEINFO*>::const_iterator stl_MapIterator = stl_MapBTParse.find(xhToken);
	if (stl_MapIterator != stl_MapBTParse.end())
	{
		if (NULL != ptszHash)
		{
			_tcscpy(ptszHash, stl_MapIterator->second->m_BTInfo->info_hash().to_string().c_str());
		}
		if (NULL != ptszCreator)
		{
			_tcscpy(ptszCreator, stl_MapIterator->second->m_BTInfo->creator().c_str());
		}
		if (NULL != ptszComment)
		{
			_tcscpy(ptszComment, stl_MapIterator->second->m_BTInfo->comment().c_str());
		}
	}
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：BTorrent_Parse_GetMagnet
函数功能：获取种子文件的磁力链接
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：ptszMagnet
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出种子文件的磁力链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CBTorrent_Parse::BTorrent_Parse_GetMagnet(XNETHANDLE xhToken, XCHAR* ptszMagnet)
{
	BTDload_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_PARSEINFO*>::const_iterator stl_MapIterator = stl_MapBTParse.find(xhToken);
	if (stl_MapIterator != stl_MapBTParse.end())
	{
		_tcscpy(ptszMagnet, make_magnet_uri(*stl_MapIterator->second->m_BTInfo).c_str());
	}
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：BTorrent_Parse_GetFile
函数功能：获取文件信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：ptszFilePath
  In/Out：Out
  类型：字符指针
  可空：N
  意思：获得保存文件路径
 参数.三：pppSt_FileList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出文件列表
 参数.四：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：获取列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CBTorrent_Parse::BTorrent_Parse_GetFile(XNETHANDLE xhToken, XCHAR* ptszFilePath, BTORRENT_FILEINFO*** pppSt_FileList, int* pInt_ListCount)
{
	BTDload_IsErrorOccur = FALSE;

	if ((NULL == ptszFilePath) || (NULL == pppSt_FileList) || (NULL == pInt_ListCount))
	{
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETFILE_PARAMENT;
		return FALSE;
	}
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_PARSEINFO*>::const_iterator stl_MapIterator = stl_MapBTParse.find(xhToken);
	if (stl_MapIterator == stl_MapBTParse.end())
	{
		BTDload_IsErrorOccur = TRUE;
		BTDload_dwErrorCode = ERROR_XENGINE_DOWNLOAD_BTORRENT_PARSE_GETFILE_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	int nUTFLen = stl_MapIterator->second->m_BTInfo->name().length();
	BaseLib_OperatorCharset_UTFToAnsi(stl_MapIterator->second->m_BTInfo->name().c_str(), ptszFilePath, &nUTFLen);

	*pInt_ListCount = stl_MapIterator->second->m_BTInfo->num_files();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_FileList, stl_MapIterator->second->m_BTInfo->num_files(), sizeof(BTORRENT_FILEINFO));
	
	lt::file_storage const& m_FileStorage = stl_MapIterator->second->m_BTInfo->files();
	int i = 0;
	for (auto const stl_ListIterator : m_FileStorage.file_range())
	{
		auto const nPieceStart = m_FileStorage.map_file(stl_ListIterator, 0, 0).piece;
		auto const nPieceEnd = m_FileStorage.map_file(stl_ListIterator, max(int64_t(m_FileStorage.file_size(stl_ListIterator)) - 1, int64_t(0)), 0).piece;
		auto const nFileFlags = m_FileStorage.file_flags(stl_ListIterator);
		//判断是否有HASH值
		if (!m_FileStorage.hash(stl_ListIterator).is_all_zeros())
		{
			_tcscpy((*pppSt_FileList)[i]->tszFileHash, m_FileStorage.hash(stl_ListIterator).to_string().c_str());
		}
#ifdef _MSC_BUILD
		nUTFLen = m_FileStorage.file_path(stl_ListIterator).length();
		BaseLib_OperatorCharset_UTFToAnsi(m_FileStorage.file_path(stl_ListIterator).c_str(), (*pppSt_FileList)[i]->tszFilePath, &nUTFLen);
		nUTFLen = m_FileStorage.file_name(stl_ListIterator).size();
		BaseLib_OperatorCharset_UTFToAnsi(m_FileStorage.file_name(stl_ListIterator).data(), (*pppSt_FileList)[i]->tszFileName, &nUTFLen);
#else
		_tcscpy((*pppSt_FileList)[i]->tszFilePath, m_FileStorage.file_path(stl_ListIterator).c_str());
		_tcscpy((*pppSt_FileList)[i]->tszFileName, m_FileStorage.file_name(stl_ListIterator).data());
#endif
		(*pppSt_FileList)[i]->nFileOffset = m_FileStorage.file_offset(stl_ListIterator);
		(*pppSt_FileList)[i]->nFileSize = m_FileStorage.file_size(stl_ListIterator);
		(*pppSt_FileList)[i]->nFileTime = m_FileStorage.mtime(stl_ListIterator);
		(*pppSt_FileList)[i]->nFileStart = static_cast<int>(nPieceStart);
		(*pppSt_FileList)[i]->nFileEnd = static_cast<int>(nPieceEnd);

		if (nFileFlags & lt::file_storage::flag_pad_file)
		{
			(*pppSt_FileList)[i]->wFileAttr = (*pppSt_FileList)[i]->wFileAttr | (1 << 3);
		}
		if (nFileFlags & lt::file_storage::flag_executable)
		{
			(*pppSt_FileList)[i]->wFileAttr = (*pppSt_FileList)[i]->wFileAttr | (1 << 2);
		}
		if (nFileFlags & lt::file_storage::flag_hidden)
		{
			(*pppSt_FileList)[i]->wFileAttr = (*pppSt_FileList)[i]->wFileAttr | (1 << 1);
		}
		if (nFileFlags & lt::file_storage::flag_symlink)
		{
			(*pppSt_FileList)[i]->wFileAttr = (*pppSt_FileList)[i]->wFileAttr | (1 << 0);
		}
		if (nFileFlags & lt::file_storage::flag_symlink)
		{
			_tcscpy((*pppSt_FileList)[i]->tszFileLink, m_FileStorage.symlink(stl_ListIterator).c_str());
		}
		i++;
	}
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：BTorrent_Parse_Destory
函数功能：销毁种子解析器
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CBTorrent_Parse::BTorrent_Parse_Destory(XNETHANDLE xhToken)
{
	BTDload_IsErrorOccur = FALSE;

	st_Locker.lock();
	unordered_map<XNETHANDLE, BTORRENT_PARSEINFO*>::iterator stl_MapIterator = stl_MapBTParse.find(xhToken);
	if (stl_MapIterator != stl_MapBTParse.end())
	{
		delete stl_MapIterator->second;
		stl_MapIterator->second = NULL;
		stl_MapBTParse.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                               保护函数
//////////////////////////////////////////////////////////////////////////