#pragma once
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
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG MSGNotify_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                        电子邮件通知                                  */
/************************************************************************/
/********************************************************************
函数名称：MSGNotify_EMail_Send
函数功能：发送一条邮件通知
 参数.一：lpszServiceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.三：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.四：lpszSendAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送给谁
 参数.五：lpszSubJect
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送的标题
 参数.六：lpszPayLoad
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool MSGNotify_EMail_Send(LPCXSTR lpszServiceAddr, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszSendAddr, LPCXSTR lpszSubJect, LPCXSTR lpszPayLoad);