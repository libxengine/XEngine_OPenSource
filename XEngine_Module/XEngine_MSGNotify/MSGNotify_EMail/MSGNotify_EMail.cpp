#include "pch.h"
#include "MSGNotify_EMail.h"
/********************************************************************
//    Created:     2025/05/20  14:18:37
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\MSGNotify_EMail\MSGNotify_EMail.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\MSGNotify_EMail
//    File Base:   MSGNotify_EMail
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     电子邮件通知
//    History:
*********************************************************************/
CMSGNotify_EMail::CMSGNotify_EMail()
{

}
CMSGNotify_EMail::~CMSGNotify_EMail()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
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
bool CMSGNotify_EMail::MSGNotify_EMail_Send(LPCXSTR lpszServiceAddr, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszSendAddr, LPCXSTR lpszSubJect, LPCXSTR lpszPayLoad)
{
	MSGNotify_IsErrorOccur = false;

    if ((NULL == lpszServiceAddr) || (NULL == lpszUser) || (NULL == lpszPass))
    {
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PARAMENT;
        return false;
    }
    XCLIENT_APIEMAIL st_EMailClient = {};

    _tcsxcpy(st_EMailClient.tszServiceAddr, lpszServiceAddr);
    _tcsxcpy(st_EMailClient.tszUserName, lpszUser);
    _tcsxcpy(st_EMailClient.tszPassWord, lpszPass);
    _tcsxcpy(st_EMailClient.tszFromAddr, lpszUser);

    XHANDLE xhToken = APIClient_EMail_Init(&st_EMailClient);
    if (NULL == xhToken)
    {
        MSGNotify_IsErrorOccur = true;
        MSGNotify_dwErrorCode = APIClient_GetLastError();
        return false;
    }
    if (!APIClient_EMail_Send(xhToken, lpszSendAddr, lpszSubJect, lpszPayLoad))
    {
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = APIClient_GetLastError();
		return false;
    }
    APIClient_EMail_Close(xhToken);
    return true;
}