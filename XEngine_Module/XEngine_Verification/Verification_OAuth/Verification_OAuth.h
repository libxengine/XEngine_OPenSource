#pragma once
/********************************************************************
//    Created:     2025/08/19  17:13:21
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_OAuth\Verification_OAuth.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_OAuth
//    File Base:   Verification_OAuth
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     OPen Authentication
//    History:
*********************************************************************/

class CVerification_OAuth
{
public:
	CVerification_OAuth();
	~CVerification_OAuth();
public:
	bool Verification_OAuth_Parse(VERIFICATION_OAUTHINFO* pSt_OAuthInfo, LPCXSTR lpszUrl = NULL, LPCXSTR lpszMSGBuffer = NULL);
	bool Verification_OAuth_PacketToken(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszAccessToken, LPCXSTR lpszRefreshToken = NULL, LPCXSTR lpszTokenType = _X("Bearer"), int nExpiredTime = 3600);
	bool Verification_OAuth_PacketError(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszOAuthError, LPCXSTR lpszErrorDescription = NULL, LPCXSTR lpszErrorUri = NULL);
public:
};