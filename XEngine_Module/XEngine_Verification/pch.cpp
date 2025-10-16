#include "pch.h"
#include "Verification_HTTP/Verification_HTTP.h"
#include "Verification_OAuth/Verification_OAuth.h"
#include "Verification_XAuth/Verification_XAuthKey.h"
#include "Verification_XAuth/Verification_XAuthNet.h"
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
CVerification_XAuthKey m_XAuthKeyVerification;
CVerification_XAuthNet m_XAuthNetVerification;
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
/************************************************************************/
/*                         XAUTH验证导出函数                            */
/************************************************************************/
extern "C" bool Verification_XAuthKey_FileRead(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszKeyFile, LPCXSTR lpszKeyPass)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_FileRead(pSt_XAuthInfo, lpszKeyFile, lpszKeyPass);
}
extern "C" bool Verification_XAuthKey_FileWrite(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszKeyFile, LPCXSTR lpszKeyPass)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_FileWrite(pSt_XAuthInfo, lpszKeyFile, lpszKeyPass);
}
extern "C" bool Verification_XAuthKey_KeyParse(VERIFICATION_XAUTHKEY* pSt_XAuthInfo)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_KeyParse(pSt_XAuthInfo);
}
extern "C" bool Verification_XAuthKey_KeyInit(VERIFICATION_XAUTHKEY* pSt_XAuthInfo)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_KeyInit(pSt_XAuthInfo);
}
extern "C" bool Verification_XAuthKey_KeySerial(XCHAR* ptszSerialStr, int nCount, int nType)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_KeySerial(ptszSerialStr, nCount, nType);
}
extern "C" bool Verification_XAuthKey_UserRegister(VERIFICATION_XAUTHKEY* pSt_AuthLocal, LPCXSTR lpszSerialStr)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_UserRegister(pSt_AuthLocal, lpszSerialStr);
}
extern "C" bool Verification_XAuthKey_WriteTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, int nCount)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_WriteTime(pSt_AuthLocal, nCount);
}
extern "C" bool Verification_XAuthKey_ReadTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, XCHAR*** ppptszTimeList, int* pInt_ListCount)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_ReadTime(pSt_AuthLocal, ppptszTimeList, pInt_ListCount);
}
extern "C" bool Verification_XAuthKey_WriteKey(LPCXSTR lpszFileKey, VERIFICATION_XAUTHKEY* pSt_AuthLocal)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_WriteKey(lpszFileKey, pSt_AuthLocal);
}
extern "C" bool Verification_XAuthKey_ReadKey(LPCXSTR lpszFileKey, VERIFICATION_XAUTHKEY* pSt_AuthLocal)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_ReadKey(lpszFileKey, pSt_AuthLocal);
}
extern "C" bool Verification_XAuthKey_WriteMemory(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, VERIFICATION_XAUTHKEY* pSt_AuthLocal)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_WriteMemory(ptszMsgBuffer, pInt_MsgLen, pSt_AuthLocal);
}
extern "C" bool Verification_XAuthKey_ReadMemory(LPCXSTR lpszMsgBuffer, int nMsgLen, VERIFICATION_XAUTHKEY* pSt_AuthLocal)
{
	return m_XAuthKeyVerification.Verification_XAuthKey_ReadMemory(lpszMsgBuffer, nMsgLen, pSt_AuthLocal);
}
extern "C" bool Verification_XAuthNet_TryRequest(LPCXSTR lpszURLAddr, LPCXSTR lpszPass)
{
	return m_XAuthNetVerification.Verification_XAuthNet_TryRequest(lpszURLAddr, lpszPass);
}
extern "C" bool Verification_XAuthNet_GetDCode(LPCXSTR lpszURLAddr, int* pInt_DYCode, XNETHANDLE* pxhToken, int* pInt_Timeout, LPCXSTR lpszPass)
{
	return m_XAuthNetVerification.Verification_XAuthNet_GetDCode(lpszURLAddr, pInt_DYCode, pxhToken, pInt_Timeout, lpszPass);
}
extern "C" bool Verification_XAuthNet_Connect(LPCXSTR lpszClientAddr, int nPort, LPCXSTR lpszPass)
{
	return m_XAuthNetVerification.Verification_XAuthNet_Connect(lpszClientAddr, nPort, lpszPass);
}
extern "C" bool Verification_XAuthNet_Close()
{
	return m_XAuthNetVerification.Verification_XAuthNet_Close();
}
extern "C" bool Verification_XAuthNet_GetAuth()
{
	return m_XAuthNetVerification.Verification_XAuthNet_GetAuth();
}
extern "C" bool Verification_XAuthNet_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszHWCode, XSHOT nDYCode, XNETHANDLE xhToken, XLONG dwCryption)
{
	return m_XAuthNetVerification.Verification_XAuthNet_Login(lpszUser, lpszPass, lpszHWCode, nDYCode, xhToken, dwCryption);
}
extern "C" bool Verification_XAuthNet_Logout(LPCXSTR lpszUser, LPCXSTR lpszPass, XLONG dwCryption)
{
	return m_XAuthNetVerification.Verification_XAuthNet_Logout(lpszUser, lpszPass, dwCryption);
}