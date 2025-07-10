#pragma once
/********************************************************************
//    Created:     2025/05/29  14:02:44
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Define.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi
//    File Base:   AIApi_Define
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     AI接口导出函数
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的回调函数
//////////////////////////////////////////////////////////////////////////
typedef void(XCALLBACK* CALLBACK_XENGINE_MODULE_AIAPI_CHAT)(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMsgBuffer, int nMsgLen, bool bThink, XPVOID lParam);
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG AIApi_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                        AI对话模型导出函数                            */
/************************************************************************/
/********************************************************************
函数名称：AIApi_Chat_Create
函数功能：AI对话模型初始化
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出初始化成功的句柄
 参数.二：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入模型API地址
 参数.三：lpszAPIKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入模型的KEY
 参数.四：fpCall_Chat
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：对话输出信息
 参数.五：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
 参数.六：bHistory
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用历史会话功能
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AIApi_Chat_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam = NULL, bool bHistory = true);
/********************************************************************
函数名称：AIApi_Chat_Destory
函数功能：销毁一个对话模型
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的模型句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AIApi_Chat_Excute(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream = false);
/********************************************************************
函数名称：AIApi_Chat_SetRole
函数功能：设置系统角色
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszModelName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入系统角色信息,比如:你是一个C/C++语言的开发大师
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AIApi_Chat_SetRole(XNETHANDLE xhToken, LPCXSTR lpszRoleName);
/********************************************************************
函数名称：AIApi_Chat_Clear
函数功能：清理对话历史记录
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
extern "C" bool AIApi_Chat_Clear(XNETHANDLE xhToken);
/********************************************************************
函数名称：AIApi_Chat_Excute
函数功能：执行对话
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszModelName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入模型名称
 参数.三：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要对话的数据
 参数.四：nMSGLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入对话数据大小
 参数.五：bStream
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否使用流式数据回复
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AIApi_Chat_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode = NULL, bool bWaitExist = true);
/********************************************************************
函数名称：AIApi_Chat_Destory
函数功能：销毁一个对话模型
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的模型句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AIApi_Chat_Destory(XNETHANDLE xhToken);