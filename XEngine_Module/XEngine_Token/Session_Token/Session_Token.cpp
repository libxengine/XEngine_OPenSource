#include "pch.h"
#include "Session_Token.h"
/********************************************************************
//    Created:     2025/05/21  14:12:39
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token\Session_Token\Session_Token.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token\Session_Token
//    File Base:   Session_Token
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     TOKEN会话管理器
//    History:
*********************************************************************/
CSession_Token::CSession_Token()
{
}
CSession_Token::~CSession_Token()
{
}
//////////////////////////////////////////////////////////////////////////
//                     公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_Token_Init
函数功能：初始化会话
 参数.一：nTimeout
  In/Out：In
  类型：整数型
  可空：N
  意思：超时时间,单位秒,0不超时,永远存在
 参数.二：bRemove
  In/Out：In
  类型：逻辑型
  可空：N
  意思：是否允许自动删除
 参数.三：fpCall_TokenEvent
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：TOKEN登录的客户端连接超时回调
 参数.四：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否初始化成功
备注：
*********************************************************************/
bool CSession_Token::Session_Token_Init(int nTimeout, bool bRemove, CALLBACK_XENGIEN_MODULE_TOKEN_EVENTS fpCall_TokenEvent, XPVOID lParam /* = NULL */)
{
    Session_IsErrorOccur = false;

    if (NULL == fpCall_TokenEvent)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_PARAMENT;
        return false;
    }
    m_nTimeout = nTimeout;
    m_lParam = lParam;
    m_bRemove = bRemove;
    lpCall_TokenEvents = fpCall_TokenEvent;

    bIsRun = true;
    //创建线程
    pSTDThread = std::make_unique<std::thread>(Session_Token_Thread, this);
    if (!pSTDThread->joinable())
    {
        bIsRun = false;
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_THREAD;
        return false;
    }
    return true;
}
/********************************************************************
函数名称：Session_Token_Destroy
函数功能：销毁服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
bool CSession_Token::Session_Token_Destroy()
{
    Session_IsErrorOccur = false;

    bIsRun = false;
    //结束线程
    if (NULL != pSTDThread)
    {
        pSTDThread->join();
    }
    //释放客户端资源
    st_Locker.lock();
    stl_MapTokenStr.clear();
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：Session_Token_Create
函数功能：创建一个TOKEN到会话管理器
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：创建成功的TOKEN
 参数.二：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：Y
  意思：用户信息表
 参数.三：nTimeout
  In/Out：In
  类型：整数型
  可空：Y
  意思：单独指定超时时间,-1 不启用,0 不超时,> 0 超时秒
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：
*********************************************************************/
bool CSession_Token::Session_Token_Create(XNETHANDLE *pxhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo /* = NULL */, int nTimeout /* = -1 */)
{
    Session_IsErrorOccur = false;

    TOKENSESSION_INFOCLIENT st_TokenClient = {};

    BaseLib_Handle_Create(pxhToken);
    st_TokenClient.nTimeout = nTimeout;
    BaseLib_Time_GetSysTime(&st_TokenClient.st_LibTimer);
    BaseLib_Time_GetSysTime(&st_TokenClient.st_OutTimer);

	_xstprintf(st_TokenClient.tszTokenStr, _X("%lld"), *pxhToken);
    if (NULL != pSt_UserInfo)
    {
        st_TokenClient.st_UserInfo = *pSt_UserInfo;
    }
    st_Locker.lock();
    stl_MapTokenStr.insert(std::make_pair(st_TokenClient.tszTokenStr, st_TokenClient));
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：Session_Authorize_Insert
函数功能：插入一个TOKEN到会话管理器
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
 参数.二：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：Y
  意思：用户信息表
 参数.三：nTimeout
  In/Out：In
  类型：整数型
  可空：Y
  意思：单独指定超时时间,-1 不启用,0 不超时,> 0 超时秒
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：
*********************************************************************/
bool CSession_Token::Session_Token_Insert(XNETHANDLE xhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo /* = NULL */, int nTimeout /* = -1 */)
{
    Session_IsErrorOccur = false;

    TOKENSESSION_INFOCLIENT st_TokenClient = {};
    _xstprintf(st_TokenClient.tszTokenStr, _X("%lld"), xhToken);

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapTokenStr.find(st_TokenClient.tszTokenStr);
	if (stl_MapIterator != stl_MapTokenStr.end())
	{
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_EXIST;
        st_Locker.unlock_shared();
		return false;
	}
	st_Locker.unlock_shared();
    
    st_TokenClient.nTimeout = nTimeout;
    BaseLib_Time_GetSysTime(&st_TokenClient.st_LibTimer);
    BaseLib_Time_GetSysTime(&st_TokenClient.st_OutTimer);

	if (NULL != pSt_UserInfo)
    {
        st_TokenClient.st_UserInfo = *pSt_UserInfo;
    }
    st_Locker.lock();
    stl_MapTokenStr.insert(std::make_pair(st_TokenClient.tszTokenStr, st_TokenClient));
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：Session_Token_Delete
函数功能：移除一个客户端
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
返回值
  类型：逻辑型
  意思：是否移除成功
备注：
*********************************************************************/
bool CSession_Token::Session_Token_Delete(XNETHANDLE xhToken)
{
    Session_IsErrorOccur = false;

	XCHAR tszTokenStr[XPATH_MID] = {};
	_xstprintf(tszTokenStr, _X("%lld"), xhToken);

    st_Locker.lock();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(tszTokenStr);
    if (stl_MapIterator != stl_MapTokenStr.end())
    {
        //移除元素
        stl_MapTokenStr.erase(stl_MapIterator);
    }
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：Session_Token_UPDate
函数功能：更新一个客户端的TOKEN时间
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：可以用于续期
*********************************************************************/
bool CSession_Token::Session_Token_UPDate(XNETHANDLE xhToken)
{
    Session_IsErrorOccur = false;

	XCHAR tszTokenStr[XPATH_MID] = {};
	_xstprintf(tszTokenStr, _X("%lld"), xhToken);

    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(tszTokenStr);
    if (stl_MapIterator == stl_MapTokenStr.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    stl_MapIterator->second.nRenewalTime++;
    BaseLib_Time_GetSysTime(&stl_MapIterator->second.st_OutTimer);
    st_Locker.unlock_shared();
    return true;
}
/********************************************************************
函数名称：Session_Token_Get
函数功能：获取客户端信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
 参数.二：pSt_UserTable
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：输出TOKEN对应的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Token::Session_Token_Get(XNETHANDLE xhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo /* = NULL */)
{
    Session_IsErrorOccur = false;

	XCHAR tszTokenStr[XPATH_MID] = {};
	_xstprintf(tszTokenStr, _X("%lld"), xhToken);

    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(tszTokenStr);
    if (stl_MapIterator == stl_MapTokenStr.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    if (NULL != pSt_UserInfo)
    {
		*pSt_UserInfo = stl_MapIterator->second.st_UserInfo;
    }
    st_Locker.unlock_shared();
    return true;
}
/********************************************************************
函数名称：Session_Token_GetTimeInfo
函数功能：获取客户端时间信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
 参数.二：pSt_LoginTime
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：输出客户端登录时间
 参数.二：pSt_UPTime
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：输出客户端更新时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Token::Session_Token_GetTimeInfo(XNETHANDLE xhToken, XENGINE_LIBTIME* pSt_LoginTime /* = NULL */, XENGINE_LIBTIME* pSt_UPTime /* = NULL */)
{
	Session_IsErrorOccur = false;

	XCHAR tszTokenStr[XPATH_MID] = {};
	_xstprintf(tszTokenStr, _X("%lld"), xhToken);

	st_Locker.lock_shared();
	std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(tszTokenStr);
	if (stl_MapIterator == stl_MapTokenStr.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	if (NULL != pSt_LoginTime)
	{
		*pSt_LoginTime = stl_MapIterator->second.st_LibTimer;
	}
    if (NULL != pSt_UPTime)
    {
        *pSt_UPTime = stl_MapIterator->second.st_OutTimer;
    }
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_Token_GetTimeout
函数功能：获取客户端剩余时间
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
 参数.二：pInt_TimeLogin
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出在线时间
 参数.三：pInt_Timeout
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出剩余超时时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Token::Session_Token_GetTimeout(XNETHANDLE xhToken, __int64x* pInt_TimeLogin, __int64x* pInt_Timeout)
{
	Session_IsErrorOccur = false;

	XCHAR tszTokenStr[XPATH_MID] = {};
	_xstprintf(tszTokenStr, _X("%lld"), xhToken);

	st_Locker.lock_shared();
	std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(tszTokenStr);
	if (stl_MapIterator == stl_MapTokenStr.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
    XENGINE_LIBTIME st_LibTimer = {};
	BaseLib_Time_GetSysTime(&st_LibTimer);                  //获取现在的系统时间
	//用户登录了多少秒
    if (NULL != pInt_TimeLogin)
    {
        BaseLib_TimeSpan_GetForStu(&stl_MapIterator->second.st_OutTimer, &st_LibTimer, pInt_TimeLogin, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
    }
    //用户超时时间
	time_t nTimeEnd = 0;
	time_t nTimeStart = 0;
    BaseLib_Time_StuTimeToTTime(&stl_MapIterator->second.st_OutTimer, &nTimeStart);
    if (NULL != pInt_Timeout)
    {
		if (-1 == stl_MapIterator->second.nTimeout)
		{
			//全局时间
			if (m_nTimeout > 0)
			{
				BaseLib_Time_StuTimeToTTime(&st_LibTimer, &nTimeEnd);
				nTimeEnd += m_nTimeout;
			}
			else
			{
                *pInt_Timeout = 0; //不超时
			}
		}
		else if (0 == stl_MapIterator->second.nTimeout)
		{
            *pInt_Timeout = 0; //不超时
		}
		else
		{
			BaseLib_Time_StuTimeToTTime(&st_LibTimer, &nTimeEnd);
			nTimeEnd += stl_MapIterator->second.nTimeout;
		}
        BaseLib_TimeSpan_GetForTime(nTimeStart, nTimeEnd, pInt_Timeout, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
    }
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_Token_GetTimeRenewal
函数功能：获取续期次数
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的TOKEN
 参数.二：pInt_RenewalTime
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出续期的次数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Token::Session_Token_GetTimeRenewal(XNETHANDLE xhToken, int* pInt_RenewalTime)
{
    Session_IsErrorOccur = false;

	XCHAR tszTokenStr[XPATH_MID] = {};
	_xstprintf(tszTokenStr, _X("%lld"), xhToken);

    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(tszTokenStr);
    if (stl_MapIterator == stl_MapTokenStr.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    *pInt_RenewalTime = stl_MapIterator->second.nRenewalTime;
    st_Locker.unlock_shared();
    return true;
}
/********************************************************************
函数名称：Session_Token_GetUser
函数功能：获取用户的TOKEN
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：密码
 参数.三：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：导出获取到的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Token::Session_Token_GetUser(LPCXSTR lpszUser, LPCXSTR lpszPass, XNETHANDLE* pxhToken)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszUser) || (NULL == lpszPass))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_PARAMENT;
        return false;
    }

    bool bFound = false;
    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.begin();
    for (; stl_MapIterator != stl_MapTokenStr.end(); stl_MapIterator++)
    {
        //用户名
        if (0 == _tcsxncmp(lpszUser, stl_MapIterator->second.st_UserInfo.tszUserName, _tcsxlen(lpszUser)))
        {
            //密码,验证密码防治冲突
            if (0 == _tcsxncmp(lpszPass, stl_MapIterator->second.st_UserInfo.tszUserPass, _tcsxlen(lpszPass)))
            {
                *pxhToken = _ttxoi(stl_MapIterator->second.tszTokenStr);
                bFound = true;
                break;
            }
        }
    }
    st_Locker.unlock_shared();
    if (!bFound)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        return false;
    }
    return true;
}
/********************************************************************
函数名称：Session_Token_GetList
函数功能：获取所有管理的TOKEN
 参数.一：pppxhToken
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出获取到的TOKEN列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取到的列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Token::Session_Token_GetList(XNETHANDLE*** pppxhToken, int* pInt_ListCount)
{
	Session_IsErrorOccur = false;

	if ((NULL == pppxhToken) || (NULL == pInt_ListCount))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_PARAMENT;
		return false;
	}
    *pInt_ListCount = stl_MapTokenStr.size();
    BaseLib_Memory_Malloc((XPPPMEM)pppxhToken, *pInt_ListCount, sizeof(XNETHANDLE));

	st_Locker.lock_shared();
	std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.begin();
    for (int i = 0; stl_MapIterator != stl_MapTokenStr.end(); stl_MapIterator++, i++)
	{
        *(*pppxhToken)[i] = _ttxoi(stl_MapIterator->second.tszTokenStr);
	}
	st_Locker.unlock_shared();
	
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool CSession_Token::Session_Token_CreateStr(XCHAR* ptszToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo /* = NULL */, int nTimeout /* = -1 */)
{
    Session_IsErrorOccur = false;

    TOKENSESSION_INFOCLIENT st_TokenClient = {};

    BaseLib_Handle_CreateStr(ptszToken);

    st_TokenClient.nTimeout = nTimeout;
    BaseLib_Time_GetSysTime(&st_TokenClient.st_LibTimer);
    BaseLib_Time_GetSysTime(&st_TokenClient.st_OutTimer);
	_tcsxcpy(st_TokenClient.tszTokenStr, ptszToken);

    if (NULL != pSt_UserInfo)
    {
        st_TokenClient.st_UserInfo = *pSt_UserInfo;
    }
    st_Locker.lock();
    stl_MapTokenStr.insert(std::make_pair(ptszToken, st_TokenClient));
    st_Locker.unlock();
    return true;
}
bool CSession_Token::Session_Token_InsertStr(LPCXSTR lpszToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo /* = NULL */, int nTimeout /* = -1 */)
{
    Session_IsErrorOccur = false;

    TOKENSESSION_INFOCLIENT st_TokenClient = {};

    st_Locker.lock_shared();
    auto stl_MapIterator = stl_MapTokenStr.find(lpszToken);
    if (stl_MapIterator != stl_MapTokenStr.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_EXIST;
        st_Locker.unlock_shared();
        return false;
    }
    st_Locker.unlock_shared();

    st_TokenClient.nTimeout = nTimeout;
    BaseLib_Time_GetSysTime(&st_TokenClient.st_LibTimer);
    BaseLib_Time_GetSysTime(&st_TokenClient.st_OutTimer);
	_tcsxcpy(st_TokenClient.tszTokenStr, lpszToken);
    if (NULL != pSt_UserInfo)
    {
        st_TokenClient.st_UserInfo = *pSt_UserInfo;
    }
    st_Locker.lock();
    stl_MapTokenStr.insert(std::make_pair(lpszToken, st_TokenClient));
    st_Locker.unlock();
    return true;
}
bool CSession_Token::Session_Token_DeleteStr(LPCXSTR lpszToken)
{
    Session_IsErrorOccur = false;

    st_Locker.lock();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(lpszToken);
    if (stl_MapIterator != stl_MapTokenStr.end())
    {
        //移除元素
        stl_MapTokenStr.erase(stl_MapIterator);
    }
    st_Locker.unlock();
    return true;
}
bool CSession_Token::Session_Token_UPDateStr(LPCXSTR lpszToken, LPCXSTR lpszRefreshToken /* = NULL */, LPCXSTR lpszNewToken /* = NULL */)
{
    Session_IsErrorOccur = false;

    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(lpszToken);
    if (stl_MapIterator == stl_MapTokenStr.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    stl_MapIterator->second.nRenewalTime++;
    BaseLib_Time_GetSysTime(&stl_MapIterator->second.st_OutTimer);
    //判断类型
    if (1 == stl_MapIterator->second.nTokenType)
    {
        //判断刷新token是否一致
        if (0 != _tcsxnicmp(stl_MapIterator->second.st_TokenInfo.tszTokenRefresh, lpszRefreshToken, _tcsxlen(lpszRefreshToken)))
        {
			Session_IsErrorOccur = true;
			Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_TOKEN;
			st_Locker.unlock_shared();
			return false;
        }
        st_Locker.unlock_shared();
        //OAuth2类型需要重置
        st_Locker.lock();
        TOKENSESSION_INFOCLIENT st_ClientInfo = std::move(stl_MapIterator->second);  // 取出旧的值
        stl_MapTokenStr.erase(stl_MapIterator);                               // 删除旧 key
        stl_MapTokenStr.emplace(lpszNewToken, std::move(st_ClientInfo));                 // 插入新 key
        st_Locker.unlock();
    }
    else
    {
        st_Locker.unlock_shared();
    }
    return true;
}
bool CSession_Token::Session_Token_GetStr(LPCXSTR lpszToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo /* = NULL */)
{
    Session_IsErrorOccur = false;

    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(lpszToken);
    if (stl_MapIterator == stl_MapTokenStr.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    if (NULL != pSt_UserInfo)
    {
        *pSt_UserInfo = stl_MapIterator->second.st_UserInfo;
    }
    st_Locker.unlock_shared();
    return true;
}
bool CSession_Token::Session_Token_GetTimeInfoStr(LPCXSTR lpszToken, XENGINE_LIBTIME* pSt_LoginTime /* = NULL */, XENGINE_LIBTIME* pSt_UPTime /* = NULL */)
{
    Session_IsErrorOccur = false;

    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(lpszToken);
    if (stl_MapIterator == stl_MapTokenStr.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    if (NULL != pSt_LoginTime)
    {
        *pSt_LoginTime = stl_MapIterator->second.st_LibTimer;
    }
    if (NULL != pSt_UPTime)
    {
        *pSt_UPTime = stl_MapIterator->second.st_OutTimer;
    }
    st_Locker.unlock_shared();
    return true;
}
bool CSession_Token::Session_Token_GetTimeoutStr(LPCXSTR lpszToken, __int64x* pInt_TimeLogin /* = NULL */, __int64x* pInt_Timeout /* = NULL */)
{
    Session_IsErrorOccur = false;

    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(lpszToken);
    if (stl_MapIterator == stl_MapTokenStr.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    XENGINE_LIBTIME st_LibTimer = {};
    BaseLib_Time_GetSysTime(&st_LibTimer);                  //获取现在的系统时间
    //用户登录了多少秒
    if (NULL != pInt_TimeLogin)
    {
        BaseLib_TimeSpan_GetForStu(&stl_MapIterator->second.st_OutTimer, &st_LibTimer, pInt_TimeLogin, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
    }
    //用户超时时间
    time_t nTimeEnd = 0;
    time_t nTimeStart = 0;
    BaseLib_Time_StuTimeToTTime(&stl_MapIterator->second.st_OutTimer, &nTimeStart);
    if (NULL != pInt_Timeout)
    {
        if (-1 == stl_MapIterator->second.nTimeout)
        {
            //全局时间
            if (m_nTimeout > 0)
            {
                BaseLib_Time_StuTimeToTTime(&st_LibTimer, &nTimeEnd);
                nTimeEnd += m_nTimeout;
            }
            else
            {
                *pInt_Timeout = 0; //不超时
            }
        }
        else if (0 == stl_MapIterator->second.nTimeout)
        {
            *pInt_Timeout = 0; //不超时
        }
        else
        {
            BaseLib_Time_StuTimeToTTime(&st_LibTimer, &nTimeEnd);
            nTimeEnd += stl_MapIterator->second.nTimeout;
        }
        BaseLib_TimeSpan_GetForTime(nTimeStart, nTimeEnd, pInt_Timeout, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
    }
    st_Locker.unlock_shared();
    return true;
}
bool CSession_Token::Session_Token_GetTimeRenewalStr(LPCXSTR lpszToken, int* pInt_RenewalTime)
{
    Session_IsErrorOccur = false;

    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.find(lpszToken);
    if (stl_MapIterator == stl_MapTokenStr.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    *pInt_RenewalTime = stl_MapIterator->second.nRenewalTime;
    st_Locker.unlock_shared();
    return true;
}
bool CSession_Token::Session_Token_GetUserStr(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR* ptszToken)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszUser) || (NULL == lpszPass))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_PARAMENT;
        return false;
    }
    bool bFound = false;
    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.begin();
    for (; stl_MapIterator != stl_MapTokenStr.end(); stl_MapIterator++)
    {
        //用户名
        if (0 == _tcsxncmp(lpszUser, stl_MapIterator->second.st_UserInfo.tszUserName, _tcsxlen(lpszUser)))
        {
            //密码,验证密码防治冲突
            if (0 == _tcsxncmp(lpszPass, stl_MapIterator->second.st_UserInfo.tszUserPass, _tcsxlen(lpszPass)))
            {
                _tcsxcpy(ptszToken, stl_MapIterator->first.c_str());
                bFound = true;
                break;
            }
        }
    }
    st_Locker.unlock_shared();
    if (!bFound)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
        return false;
    }
    return true;
}
bool CSession_Token::Session_Token_GetListStr(XCHAR*** ppptszToken, int* pInt_ListCount)
{
    Session_IsErrorOccur = false;

    if ((NULL == ppptszToken) || (NULL == pInt_ListCount))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_PARAMENT;
        return false;
    }
    *pInt_ListCount = stl_MapTokenStr.size();
    BaseLib_Memory_Malloc((XPPPMEM)ppptszToken, *pInt_ListCount, sizeof(XPATH_MID));

    st_Locker.lock_shared();
    std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = stl_MapTokenStr.begin();
    for (int i = 0; stl_MapIterator != stl_MapTokenStr.end(); stl_MapIterator++, i++)
    {
        _tcsxcpy((*ppptszToken)[i], stl_MapIterator->first.c_str());
    }
    st_Locker.unlock_shared();

    return true;
}
bool CSession_Token::Session_Token_OAuthSetInfo(LPCXSTR lpszToken, VERIFICATION_OAUTHINFO* pSt_OAuthInfo)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszToken) || (NULL == pSt_OAuthInfo))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapTokenStr.find(lpszToken);
    if (stl_MapIterator == stl_MapTokenStr.end())
    {
        st_Locker.unlock_shared();
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
		return false;
    }
    stl_MapIterator->second.nTokenType = 1;
    stl_MapIterator->second.st_OAuthInfo = *pSt_OAuthInfo;
	st_Locker.unlock_shared();
	return true;
}
bool CSession_Token::Session_Token_OAuthSetToken(LPCXSTR lpszToken, VERIFICATION_TOKENINFO* pSt_TokenInfo)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszToken) || (NULL == pSt_TokenInfo))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapTokenStr.find(lpszToken);
	if (stl_MapIterator == stl_MapTokenStr.end())
	{
		st_Locker.unlock_shared();
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND;
		return false;
	}
	stl_MapIterator->second.st_TokenInfo = *pSt_TokenInfo;
    //影响过期时间
	stl_MapIterator->second.nTimeout = pSt_TokenInfo->nExpiredTime; //更新超时时间
	st_Locker.unlock_shared();
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                     线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CSession_Token::Session_Token_Thread(XPVOID lParam)
{
    CSession_Token* pClass_This = (CSession_Token*)lParam;
    std::list<TOKENSESSION_INFOCLIENT> stl_ListNotify;
    XENGINE_LIBTIME st_LibTimer;

    while (pClass_This->bIsRun)
    {
        //开始轮训用户
        pClass_This->st_Locker.lock_shared();
		std::unordered_map<xstring, TOKENSESSION_INFOCLIENT>::iterator stl_MapIterator = pClass_This->stl_MapTokenStr.begin();
		for (; stl_MapIterator != pClass_This->stl_MapTokenStr.end(); stl_MapIterator++)
		{
			BaseLib_Time_GetSysTime(&st_LibTimer);                  //获取现在的系统时间
			__int64x nOnlineSpan = 0;                               //在线时间
			//用户登录了多少秒
			BaseLib_TimeSpan_GetForStu(&stl_MapIterator->second.st_OutTimer, &st_LibTimer, &nOnlineSpan, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
			if (stl_MapIterator->second.nTimeout >= 0)
			{
				if ((stl_MapIterator->second.nTimeout > 0) && (nOnlineSpan > stl_MapIterator->second.nTimeout))
				{
					stl_ListNotify.push_back(stl_MapIterator->second);
				}
			}
			else
			{
				if ((nOnlineSpan > pClass_This->m_nTimeout) && (pClass_This->m_nTimeout > 0))
				{
					stl_ListNotify.push_back(stl_MapIterator->second);
				}
			}
		}
        pClass_This->st_Locker.unlock_shared();
		//判断是否有需要关闭的客户端
		if (!stl_ListNotify.empty())
		{
			std::list<TOKENSESSION_INFOCLIENT>::iterator stl_ListIterator = stl_ListNotify.begin();
			for (; stl_ListIterator != stl_ListNotify.end(); stl_ListIterator++)
			{
                if (pClass_This->m_bRemove)
                {
                    ::Session_Token_DeleteStr(stl_ListIterator->tszTokenStr);
                }
				pClass_This->lpCall_TokenEvents(stl_ListIterator->tszTokenStr, stl_ListIterator->nTimeout, stl_ListIterator->nRenewalTime, &stl_ListIterator->st_LibTimer, &stl_ListIterator->st_UserInfo, pClass_This->m_lParam);
			}
			stl_ListNotify.clear();        //清理元素
		}

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
