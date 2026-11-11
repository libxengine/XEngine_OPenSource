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
/************************************************************************/
/*                        短信服务通知                                  */
/************************************************************************/
/********************************************************************
函数名称：MSGNotify_SMS_SendTencent
函数功能：腾讯API发送短信消息
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszSecretID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的秘钥ID
 参数.三：lpszSecretKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的秘钥KEY
 参数.四：lpszRegionStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的地区 (如: ap-guangzhou)
 参数.五：lpszAPPID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的APP ID
 参数.六：lpszSignName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：签名内容
 参数.七：lpszTemplateID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的模版ID
 参数.八：ppptszPhoneNumber
  In/Out：In
  类型：三级指针
  可空：N
  意思：发送给谁 +8613333
 参数.九：nListCount
  In/Out：In
  类型：整数型
  可空：Y
  意思：发送的数量
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool MSGNotify_SMS_SendTencent(LPCXSTR lpszAPIUrl, LPCXSTR lpszSecretID, LPCXSTR lpszSecretKey, LPCXSTR lpszRegionStr, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, XCHAR*** ppptszPhoneNumber, int nListCount);
/********************************************************************
函数名称：MSGNotify_SMS_SendTencent
函数功能：阿里云API发送短信消息
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszSecretID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的秘钥ID
 参数.三：lpszSecretKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的秘钥KEY
 参数.四：lpszRegionStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的地区
 参数.五：lpszAPPID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的APP ID
 参数.六：lpszSignName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：签名内容
 参数.七：lpszTemplateID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的模版ID
 参数.八：ppptszPhoneNumber
  In/Out：In
  类型：三级指针
  可空：N
  意思：发送给谁
 参数.九：nListCount
  In/Out：In
  类型：整数型
  可空：Y
  意思：发送的数量
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool MSGNotify_SMS_SendAliyun(LPCXSTR lpszAPIUrl, LPCXSTR lpszSecretID, LPCXSTR lpszSecretKey, LPCXSTR lpszRegionStr, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, XCHAR*** ppptszPhoneNumber, int nListCount);