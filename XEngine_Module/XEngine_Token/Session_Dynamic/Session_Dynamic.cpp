#include "pch.h"
#include "Session_Dynamic.h"
/********************************************************************
//    Created:     2025/05/21  14:15:55
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token\Session_Dynamic\Session_Dynamic.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token\Session_Dynamic
//    File Base:   Session_Dynamic
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     动态码会话管理
//    History:
*********************************************************************/
CSession_Dynamic::CSession_Dynamic()
{
}
CSession_Dynamic::~CSession_Dynamic()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_Dynamic_Init
函数功能：初始化动态码
 参数.一：nSecond
  In/Out：In
  类型：整数型
  可空：N
  意思：动态码超时时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Dynamic::Session_Dynamic_Init(int nSecond)
{
	Session_IsErrorOccur = true;

	m_bRun = true;
	m_nSescond = nSecond;
	pSTDThread = std::make_unique<std::thread>(Session_Dynamic_Thread, this);
	if (NULL == pSTDThread)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_DYNAMIC_THREAD;
		return false;
	}

	return true;
}
/********************************************************************
函数名称：Session_Dynamic_Destory
函数功能：销毁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Dynamic::Session_Dynamic_Destory()
{
	Session_IsErrorOccur = false;

	if (NULL != pSTDThread)
	{
		m_bRun = false;
		pSTDThread->join();
	}
	st_Locker.lock();
	stl_MapDynamicCode.clear();
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：Session_Dynamic_Create
函数功能：创建一个动态码
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出动态码绑定的句柄
 参数.二：pInt_DynamicCode
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出动态码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Dynamic::Session_Dynamic_Create(XNETHANDLE* pxhToken, XSHOT* pInt_DynamicCode)
{
	Session_IsErrorOccur = false;

	if ((NULL == pxhToken) || (NULL == pInt_DynamicCode))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_DYNAMIC_PARAMENT;
		return false;
	}
	DYANMICSESSION_INFOCLIENT st_DynamicCode = {};

	st_DynamicCode.nTimeStart = time(NULL);
	BaseLib_Handle_Create(&st_DynamicCode.xhToken);
	BaseLib_Handle_Create((XNETHANDLE*)&st_DynamicCode.nDynamicCode, 10001, 65535);

	*pxhToken = st_DynamicCode.xhToken;
	*pInt_DynamicCode = st_DynamicCode.nDynamicCode;
	st_Locker.lock();
	stl_MapDynamicCode.insert(std::make_pair(*pxhToken, st_DynamicCode));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：Session_Dynamic_Delete
函数功能：主动删除一个动态码
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的动态码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Dynamic::Session_Dynamic_Delete(XNETHANDLE xhToken)
{
	Session_IsErrorOccur = false;

	st_Locker.lock();
	auto stl_MapIterator = stl_MapDynamicCode.find(xhToken);
	if (stl_MapIterator != stl_MapDynamicCode.end())
	{
		stl_MapDynamicCode.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：Session_Dynamic_Get
函数功能：获取句柄和动态码绑定信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入操作的句柄
 参数.二：pInt_DynamicCode
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出绑定的动态码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Dynamic::Session_Dynamic_Get(XNETHANDLE xhToken, int* pInt_DynamicCode)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapDynamicCode.find(xhToken);
	if (stl_MapIterator == stl_MapDynamicCode.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_XENGINE_MODULE_SESSION_DYNAMIC_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pInt_DynamicCode = stl_MapIterator->second.nDynamicCode;
	st_Locker.unlock_shared();
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CALLBACK CSession_Dynamic::Session_Dynamic_Thread(XPVOID lParam)
{
	CSession_Dynamic* pClass_This = (CSession_Dynamic*)lParam;
	//用于处理超时的token
	while (pClass_This->m_bRun)
	{
		pClass_This->st_Locker.lock();
		for (auto stl_MapIterator = pClass_This->stl_MapDynamicCode.begin(); stl_MapIterator != pClass_This->stl_MapDynamicCode.end(); )
		{
			time_t nTimeEnd = time(NULL);
			if ((nTimeEnd - stl_MapIterator->second.nTimeStart) > pClass_This->m_nSescond)
			{
				stl_MapIterator = pClass_This->stl_MapDynamicCode.erase(stl_MapIterator);
			}
			else
			{
				stl_MapIterator++;
			}
		}
		pClass_This->st_Locker.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}