#pragma once
/********************************************************************
//    Created:     2025/08/19  10:55:10
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_HTTP\Verification_HTTP.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_HTTP
//    File Base:   Verification_HTTP
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     HTTP验证处理类
//    History:
*********************************************************************/

class CVerification_HTTP
{
public:
	CVerification_HTTP();
	~CVerification_HTTP();
public:
	bool Verification_HTTP_BasicClientPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszUser, LPCXSTR lpszPass);
	bool Verification_HTTP_DigestClientPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszRequestMethod, LPCXSTR lpszRequestUri, LPCXSTR lpszNonceStr, LPCXSTR lpszOpaqueStr, bool bQOPBody = false);
public:
	bool Verification_HTTP_BasicServerPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen);
	bool Verification_HTTP_DigestServerPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, XCHAR* ptszNonceStr, XCHAR* ptszOpaqueStr, bool bQOPBody = false);
public:
	bool Verification_HTTP_GetType(XCHAR** pptszListHdr, int nHdrCount, int* pInt_Type);
	bool Verification_HTTP_Basic(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR** pptszListHdr, int nHdrCount);
	bool Verification_HTTP_Digest(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszMethod, XCHAR** pptszListHdr, int nHdrCount, LPCXSTR lpszNonceStr = NULL, LPCXSTR lpszOpaqueStr = NULL);
protected:
	bool Verification_HTTP_DigestVer(XCHAR* ptszResponseStr, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszMethod, LPCXSTR lpszUrl, LPCXSTR lpszNonce, LPCXSTR lpszCNonce, LPCXSTR lpszNC = _X("00000001"), bool bQOPBody = false, LPCXSTR lpszMSGBody = NULL);
	bool Verification_HTTP_BasicEncoder(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR* ptszMsgBuffer, bool bADD = true);
	bool Verification_HTTP_BasicDecoder(LPCXSTR lpszMsgBuffer, XCHAR* ptszUser, XCHAR* ptszPass);
	bool Verification_HTTP_ConvertToLower(XCHAR* ptszMD5Str);
private:
	LPCXSTR lpszRealm = _X("XEngine_Verification");
};