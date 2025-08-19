#include "pch.h"
#include "Verification_HTTP/Verification_HTTP.h"
/********************************************************************
//    Created:     2025/08/19  10:52:23
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\pch.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Verification_IsErrorOccur = false;
XLONG Verification_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CVerification_HTTP m_HTTPVerification;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Verification_GetLastError(int* pInt_ErrorCode)
{
	if (NULL != pInt_ErrorCode)
	{
		*pInt_ErrorCode = errno;
	}
	return Verification_dwErrorCode;
}
/************************************************************************/
/*                         HTTP验证导出函数                             */
/************************************************************************/
extern "C" bool Verification_HTTP_BasicClientPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszUser, LPCXSTR lpszPass)
{
	return m_HTTPVerification.Verification_HTTP_BasicClientPacket(ptszMSGBuffer, pInt_MSGLen, lpszUser, lpszPass);
}
extern "C" bool Verification_HTTP_DigestClientPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszRequestMethod, LPCXSTR lpszRequestUri, LPCXSTR lpszNonceStr, LPCXSTR lpszOpaqueStr, bool bQOPBody)
{
	return m_HTTPVerification.Verification_HTTP_DigestClientPacket(ptszMSGBuffer, pInt_MSGLen, lpszUser, lpszPass, lpszRequestMethod, lpszRequestUri, lpszNonceStr, lpszOpaqueStr, bQOPBody);
}
extern "C" bool Verification_HTTP_BasicServerPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	return m_HTTPVerification.Verification_HTTP_BasicServerPacket(ptszMSGBuffer, pInt_MSGLen);
}
extern "C" bool Verification_HTTP_DigestServerPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, XCHAR* ptszNonceStr, XCHAR* ptszOpaqueStr, bool bQOPBody)
{
	return m_HTTPVerification.Verification_HTTP_DigestServerPacket(ptszMSGBuffer, pInt_MSGLen, ptszNonceStr, ptszOpaqueStr, bQOPBody);
}