#include "pch.h"
#include "Session_Token/Session_Token.h"
#include "Session_Dynamic/Session_Dynamic.h"
/********************************************************************
//    Created:     2025/05/21  14:13:21
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token\pch.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Session_IsErrorOccur = false;
XLONG Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CSession_Token m_SessionToken;
CSession_Dynamic m_SessionDynamic;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Session_GetLastError(int* pInt_ErrorCode)
{
	if (NULL != pInt_ErrorCode)
	{
		*pInt_ErrorCode = errno;
	}
	return Session_dwErrorCode;
}
/************************************************************************/
/*                         TOKEN会话导出函数                            */
/************************************************************************/
extern "C" bool Session_Token_Init(int nTimeout, CALLBACK_XENGIEN_MODULE_TOKEN_EVENTS fpCall_TokenEvent, XPVOID lParam)
{
	return m_SessionToken.Session_Token_Init(nTimeout, fpCall_TokenEvent, lParam);
}
extern "C" bool Session_Token_Destroy()
{
	return m_SessionToken.Session_Token_Destroy();
}
extern "C" bool Session_Token_Insert(XNETHANDLE xhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo, int nTimeout)
{
	return m_SessionToken.Session_Token_Insert(xhToken, pSt_UserInfo, nTimeout);
}
extern "C" bool Session_Token_Delete(XNETHANDLE xhToken)
{
	return m_SessionToken.Session_Token_Delete(xhToken);
}
extern "C" bool Session_Token_UPDate(XNETHANDLE xhToken)
{
	return m_SessionToken.Session_Token_UPDate(xhToken);
}
extern "C" bool Session_Token_Get(XNETHANDLE xhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
	return m_SessionToken.Session_Token_Get(xhToken, pSt_UserInfo);
}
extern "C" bool Session_Token_GetTimeInfo(XNETHANDLE xhToken, XENGINE_LIBTIME* pSt_LoginTime, XENGINE_LIBTIME* pSt_UPTime)
{
	return m_SessionToken.Session_Token_GetTimeInfo(xhToken, pSt_LoginTime, pSt_UPTime);
}
extern "C" bool Session_Token_GetTimeout(XNETHANDLE xhToken, int* pInt_Timeout)
{
	return m_SessionToken.Session_Token_GetTimeout(xhToken, pInt_Timeout);
}
extern "C" bool Session_Token_GetUser(LPCXSTR lpszUser, LPCXSTR lpszPass, XNETHANDLE* pxhToken)
{
	return m_SessionToken.Session_Token_GetUser(lpszUser, lpszPass, pxhToken);
}
extern "C" bool Session_Token_RenewalTime(XNETHANDLE xhToken, int* pInt_RenewalTime)
{
	return m_SessionToken.Session_Token_RenewalTime(xhToken, pInt_RenewalTime);
}
/************************************************************************/
/*                    动态码导出定义                                    */
/************************************************************************/
extern "C" bool Session_Dynamic_Init(int nSecond)
{
	return m_SessionDynamic.Session_Dynamic_Init(nSecond);
}
extern "C" bool Session_Dynamic_Destory()
{
	return m_SessionDynamic.Session_Dynamic_Destory();
}
extern "C" bool Session_Dynamic_Create(XNETHANDLE* pxhToken, XSHOT* pInt_DynamicCode)
{
	return m_SessionDynamic.Session_Dynamic_Create(pxhToken, pInt_DynamicCode);
}
extern "C" bool Session_Dynamic_Delete(XNETHANDLE xhToken)
{
	return m_SessionDynamic.Session_Dynamic_Delete(xhToken);
}
extern "C" bool Session_Dynamic_Get(XNETHANDLE xhToken, int* pInt_DynamicCode)
{
	return m_SessionDynamic.Session_Dynamic_Get(xhToken, pInt_DynamicCode);
}