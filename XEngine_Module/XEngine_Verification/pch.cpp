#include "pch.h"
#include "Verification_HTTP/Verification_HTTP.h"
#include "Verification_OAuth/Verification_OAuth.h"
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
CVerification_OAuth m_OAuthVerification;
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
extern "C" bool Verification_HTTP_GetType(XCHAR** pptszListHdr, int nHdrCount, int* pInt_Type)
{
	return m_HTTPVerification.Verification_HTTP_GetType(pptszListHdr, nHdrCount, pInt_Type);
}
extern "C" bool Verification_HTTP_Basic(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR** pptszListHdr, int nHdrCount)
{
	return m_HTTPVerification.Verification_HTTP_Basic(lpszUser, lpszPass, pptszListHdr, nHdrCount);
}
extern "C" bool Verification_HTTP_Digest(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszMethod, XCHAR** pptszListHdr, int nHdrCount, LPCXSTR lpszNonceStr, LPCXSTR lpszOpaqueStr)
{
	return m_HTTPVerification.Verification_HTTP_Digest(lpszUser, lpszPass, lpszMethod, pptszListHdr, nHdrCount, lpszNonceStr, lpszOpaqueStr);
}
/************************************************************************/
/*                         OAUTH验证导出函数                            */
/************************************************************************/
extern "C" bool Verification_OAuth_Parse(VERIFICATION_OAUTHINFO* pSt_OAuthInfo, LPCXSTR lpszUrl, LPCXSTR lpszMSGBuffer)
{
	return m_OAuthVerification.Verification_OAuth_Parse(pSt_OAuthInfo, lpszUrl, lpszMSGBuffer);
}
extern "C" bool Verification_OAuth_PacketToken(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszAccessToken, LPCXSTR lpszRefreshToken, LPCXSTR lpszTokenType, int nExpiredTime)
{
	return m_OAuthVerification.Verification_OAuth_PacketToken(ptszMSGBuffer, pInt_MSGLen, lpszAccessToken, lpszRefreshToken, lpszTokenType, nExpiredTime);
}
extern "C" bool Verification_OAuth_PacketError(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszOAuthError, LPCXSTR lpszErrorDescription, LPCXSTR lpszErrorUri)
{
	return m_OAuthVerification.Verification_OAuth_PacketError(ptszMSGBuffer, pInt_MSGLen, lpszOAuthError, lpszErrorDescription, lpszErrorUri);
}