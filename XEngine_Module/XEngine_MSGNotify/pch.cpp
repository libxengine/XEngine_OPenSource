#include "pch.h"
#include "MSGNotify_EMail/MSGNotify_EMail.h"
#include "MSGNotify_SMS/MSGNotify_SMS.h"
/********************************************************************
//    Created:     2025/05/20  14:27:11
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\pch.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     消息通知导出实现
//    History:
*********************************************************************/
bool MSGNotify_IsErrorOccur = false;
XLONG MSGNotify_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CMSGNotify_EMail m_MSGEMail;
CMSGNotify_SMS m_MSGSms;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG MSGNotify_GetLastError(int* pInt_ErrorCode)
{
	if (NULL != pInt_ErrorCode)
	{
		*pInt_ErrorCode = errno;
	}
	return MSGNotify_dwErrorCode;
}
/************************************************************************/
/*                        电子邮件通知                                  */
/************************************************************************/
extern "C" bool MSGNotify_EMail_Send(LPCXSTR lpszServiceAddr, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszSendAddr, LPCXSTR lpszSubJect, LPCXSTR lpszPayLoad)
{
	return m_MSGEMail.MSGNotify_EMail_Send(lpszServiceAddr, lpszUser, lpszPass, lpszSendAddr, lpszSubJect, lpszPayLoad);
}
/************************************************************************/
/*                        短信服务通知                                  */
/************************************************************************/
extern "C" bool MSGNotify_SMS_SendTencent(LPCXSTR lpszAPIUrl, LPCXSTR lpszSecretID, LPCXSTR lpszSecretKey, LPCXSTR lpszRegionStr, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, XCHAR*** ppptszPhoneNumber, int nListCount)
{
	return m_MSGSms.MSGNotify_SMS_SendTencent(lpszAPIUrl, lpszSecretID, lpszSecretKey, lpszRegionStr, lpszAPPID, lpszSignName, lpszTemplateID, ppptszPhoneNumber, nListCount);
}
extern "C" bool MSGNotify_SMS_SendAliyun(LPCXSTR lpszAPIUrl, LPCXSTR lpszSecretID, LPCXSTR lpszSecretKey, LPCXSTR lpszRegionStr, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, XCHAR*** ppptszPhoneNumber, int nListCount)
{
	return m_MSGSms.MSGNotify_SMS_SendAliyun(lpszAPIUrl, lpszSecretID, lpszSecretKey, lpszRegionStr, lpszAPPID, lpszSignName, lpszTemplateID, ppptszPhoneNumber, nListCount);
}