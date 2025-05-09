﻿#pragma once
/********************************************************************
//    Created:     2024/04/28  17:21:18
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_InfoReport\InfoReport_Define.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_InfoReport
//    File Base:   InfoReport_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     信息报告导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG InfoReport_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                        APIMachine报告                                */
/************************************************************************/
/********************************************************************
函数名称：InfoReport_APIMachine_Send
函数功能：发送一条信息报告给API服务器
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszServiceName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入上报的服务名称
 参数.三：pInt_TimeNumber
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出报告次数
返回值
  类型：逻辑型
  意思：是否成功
备注：lpszAPIUrl = _X("http://127.0.0.1:5501/api?function=machine&params1=0");
*********************************************************************/
extern "C" bool InfoReport_APIMachine_Send(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName, __int64x* pInt_TimeNumber = NULL);
/********************************************************************
函数名称：InfoReport_APIMachine_GetTime
函数功能：获取报告次数
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszServiceName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入服务名称
 参数.三：pInt_TimeNumber
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出报告次数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool InfoReport_APIMachine_GetTime(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName, __int64x* pInt_TimeNumber);
/********************************************************************
函数名称：InfoReport_APIMachine_Delete
函数功能：发送一条删除请求给API服务器
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszServiceName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入上报的服务名称
返回值
  类型：逻辑型
  意思：是否成功
备注：lpszAPIUrl = _X("http://127.0.0.1:5501/api?function=machine");
*********************************************************************/
extern "C" bool InfoReport_APIMachine_Delete(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName);