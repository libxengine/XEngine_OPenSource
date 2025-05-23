﻿#pragma once
/********************************************************************
//    Created:     2025/05/20  14:26:10
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\MSGNotify_Define.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify
//    File Base:   MSGNotify_Define
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     消息通知导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                         导出的回调函数
//////////////////////////////////////////////////////////////////////////
typedef void(CALLBACK* CALLBACK_XENGIEN_MODULE_TOKEN_EVENTS)(XNETHANDLE xhToken, XPVOID lParam);
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Session_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                         TOKEN会话导出函数                            */
/************************************************************************/
/********************************************************************
函数名称：Session_Token_Init
函数功能：初始化会话
 参数.一：nTimeout
  In/Out：In
  类型：整数型
  可空：N
  意思：超时时间,单位秒
 参数.二：fpCall_TokenEvent
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：TOKEN登录的客户端连接超时回调
 参数.三：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否初始化成功
备注：
*********************************************************************/
extern "C" bool Session_Token_Init(int nTimeout, CALLBACK_XENGIEN_MODULE_TOKEN_EVENTS fpCall_TokenEvent, XPVOID lParam = NULL);
/********************************************************************
函数名称：Session_Token_Destroy
函数功能：销毁服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
extern "C" bool Session_Token_Destroy();
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
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：
*********************************************************************/
extern "C" bool Session_Token_Insert(XNETHANDLE xhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo = NULL);
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
extern "C" bool Session_Token_Delete(XNETHANDLE xhToken);
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
备注：
*********************************************************************/
extern "C" bool Session_Token_UPDate(XNETHANDLE xhToken);
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
extern "C" bool Session_Token_Get(XNETHANDLE xhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo = NULL);
/********************************************************************
函数名称：Session_Token_GetUser
函数功能：获取用户是否存在
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
extern "C" bool Session_Token_GetUser(LPCXSTR lpszUser, LPCXSTR lpszPass, XNETHANDLE* pxhToken);
/********************************************************************
函数名称：Session_Token_RenewalTime
函数功能：续期时间
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
extern "C" bool Session_Token_RenewalTime(XNETHANDLE xhToken, int* pInt_RenewalTime);
/************************************************************************/
/*                    动态码导出定义                                    */
/************************************************************************/
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
extern "C" bool Session_Dynamic_Init(int nSecond);
/********************************************************************
函数名称：Session_Dynamic_Destory
函数功能：销毁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Dynamic_Destory();
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
extern "C" bool Session_Dynamic_Create(XNETHANDLE* pxhToken, XSHOT* pInt_DynamicCode);
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
extern "C" bool Session_Dynamic_Delete(XNETHANDLE xhToken);
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
extern "C" bool Session_Dynamic_Get(XNETHANDLE xhToken, int* pInt_DynamicCode);