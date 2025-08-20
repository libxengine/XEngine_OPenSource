#include "pch.h"
#include "Verification_OAuth.h"
/********************************************************************
//    Created:     2025/08/19  17:14:19
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_OAuth\Verification_OAuth.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_OAuth
//    File Base:   Verification_OAuth
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     OPen Authentication
//    History:
*********************************************************************/
CVerification_OAuth::CVerification_OAuth()
{
}
CVerification_OAuth::~CVerification_OAuth()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Verification_OAuth_Parse
函数功能：解析OAUTH验证请求信息
 参数.一：pSt_OAuthInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出解析到的信息
 参数.二：lpszUrl
  In/Out：Out
  类型：常量字符指针
  可空：Y
  意思：输入请求的URL参数
 参数.三：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入负载数据类型
返回值
  类型：逻辑型
  意思：是否成功
备注：OAuth验证可以是在URL地址或者负载数据.
*********************************************************************/
bool CVerification_OAuth::Verification_OAuth_Parse(VERIFICATION_OAUTHINFO* pSt_OAuthInfo, LPCXSTR lpszUrl /* = NULL */, LPCXSTR lpszMSGBuffer /* = NULL */)
{
	Verification_IsErrorOccur = false;

	if (NULL == pSt_OAuthInfo)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_OAUTH_PARAMENT;
		return false;
	}
	XCHAR tszHTTPBuffer[XPATH_MAX] = {};
	if (NULL != lpszMSGBuffer)
	{
		_xstrcpy(tszHTTPBuffer, lpszMSGBuffer, XPATH_MAX);
	}
	if (NULL != lpszUrl)
	{
		_xstrcpy(tszHTTPBuffer, lpszUrl + 1, XPATH_MAX);
	}
	LPCXSTR lpszGrandTStr = _X("grant_type");
	LPCXSTR lpszCodeStr = _X("code");
	LPCXSTR lpszClientIDStr = _X("client_id");
	LPCXSTR lpszSecretStr = _X("client_secret");
	LPCXSTR lpszUserStr = _X("username");
	LPCXSTR lpszPassStr = _X("password");
	LPCXSTR lpszRefreshStr = _X("refresh_token");

	XCHAR* ptszTokTmp = NULL;
	XCHAR* ptszTokStr = _tcsxtok_s(tszHTTPBuffer, _X("&"), &ptszTokTmp);
	while (NULL != ptszTokStr)
	{
		XCHAR tszStrKey[XPATH_MIN] = {};
		XCHAR tszStrVlu[XPATH_MIN] = {};
		BaseLib_String_GetKeyValue(ptszTokStr, _X("="), tszStrKey, tszStrVlu);

		if (0 == _tcsxnicmp(lpszGrandTStr, tszStrKey, _tcsxlen(lpszGrandTStr)))
		{
			_tcsxcpy(pSt_OAuthInfo->tszGrandType, tszStrVlu);
		}
		else if (0 == _tcsxnicmp(lpszCodeStr, tszStrKey, _tcsxlen(lpszCodeStr)))
		{
			_tcsxcpy(pSt_OAuthInfo->tszClientCode, tszStrVlu);
		}
		else if (0 == _tcsxnicmp(lpszClientIDStr, tszStrKey, _tcsxlen(lpszClientIDStr)))
		{
			_tcsxcpy(pSt_OAuthInfo->tszClientID, tszStrVlu);
		}
		else if (0 == _tcsxnicmp(lpszSecretStr, tszStrKey, _tcsxlen(lpszSecretStr)))
		{
			_tcsxcpy(pSt_OAuthInfo->tszClientSecert, tszStrVlu);
		}
		else if (0 == _tcsxnicmp(lpszUserStr, tszStrKey, _tcsxlen(lpszUserStr)))
		{
			_tcsxcpy(pSt_OAuthInfo->tszClientUser, tszStrVlu);
		}
		else if (0 == _tcsxnicmp(lpszPassStr, tszStrKey, _tcsxlen(lpszPassStr)))
		{
			_tcsxcpy(pSt_OAuthInfo->tszClientPass, tszStrVlu);
		}
		else if (0 == _tcsxnicmp(lpszRefreshStr, tszStrKey, _tcsxlen(lpszRefreshStr)))
		{
			_tcsxcpy(pSt_OAuthInfo->tszClientRefresh, tszStrVlu);
		}
		ptszTokStr = _tcsxtok_s(NULL, _X("&"), &ptszTokTmp);
	}
	return true;
}
/********************************************************************
函数名称：Verification_OAuth_PacketToken
函数功能：打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包的大小
 参数.三：lpszAccessToken
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要打包的访问token
 参数.四：lpszRefreshToken
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入刷新的TOKEN
 参数.五：lpszTokenType
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入TOKEN类型，默认为Bearer
 参数.六：nExpiredTime
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入token过期时间，默认为3600秒
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_OAuth::Verification_OAuth_PacketToken(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszAccessToken, LPCXSTR lpszRefreshToken /* = NULL */, LPCXSTR lpszTokenType /* = _X("Bearer") */, int nExpiredTime /* = 3600 */)
{
	Verification_IsErrorOccur = false;

	if (NULL == ptszMSGBuffer)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_OAUTH_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;

	st_JsonRoot["access_token"] = lpszAccessToken;
	st_JsonRoot["token_type"] = lpszTokenType;
	if (NULL != lpszRefreshToken)
	{
		st_JsonRoot["refresh_token"] = lpszRefreshToken;
	}
	st_JsonRoot["expires_in"] = nExpiredTime;

	*pInt_MSGLen = st_JsonRoot.toStyledString().length();
	_tcsxcpy(ptszMSGBuffer, st_JsonRoot.toStyledString().c_str());
	return true;
}
/********************************************************************
函数名称：Verification_OAuth_PacketError
函数功能：错误打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包的大小
 参数.三：lpszOAuthError
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入-OAUTH2错误类型,必须参考头定义的错误信息
 参数.四：lpszErrorDescription
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：错误描述信息
 参数.五：lpszErrorUri
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入错误URI地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_OAuth::Verification_OAuth_PacketError(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszOAuthError, LPCXSTR lpszErrorDescription /* = NULL */, LPCXSTR lpszErrorUri /* = NULL */)
{
	Verification_IsErrorOccur = false;

	if (NULL == ptszMSGBuffer)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_OAUTH_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;

	st_JsonRoot["error"] = lpszOAuthError;
	if (NULL != lpszErrorDescription)
	{
		st_JsonRoot["error_description"] = lpszErrorDescription;
	}
	if (NULL != lpszErrorUri)
	{
		st_JsonRoot["error_uri"] = lpszErrorUri;
	}

	*pInt_MSGLen = st_JsonRoot.toStyledString().length();
	_tcsxcpy(ptszMSGBuffer, st_JsonRoot.toStyledString().c_str());
	return true;
}