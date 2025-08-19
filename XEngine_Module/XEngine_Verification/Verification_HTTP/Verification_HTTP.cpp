#include "pch.h"
#include "Verification_HTTP.h"
/********************************************************************
//    Created:     2025/08/19  10:55:14
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_HTTP\Verification_HTTP.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_HTTP
//    File Base:   Verification_HTTP
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     HTTP验证处理类
//    History:
*********************************************************************/
CVerification_HTTP::CVerification_HTTP()
{
}
CVerification_HTTP::~CVerification_HTTP()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Verification_HTTP_BasicClientPacket
函数功能：客户端基本验证打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包后的HTTP验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
 参数.三：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.四：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_BasicClientPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszUser, LPCXSTR lpszPass)
{
	Verification_IsErrorOccur = false;

	if ((NULL == ptszMSGBuffer) || (NULL == pInt_MSGLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT;
		return false;
	}
	Verification_HTTP_BasicEncoder(lpszUser, lpszPass, ptszMSGBuffer, true);

	*pInt_MSGLen = (int)_tcsxlen(ptszMSGBuffer);
	return true;
}
/********************************************************************
函数名称：Verification_HTTP_DigestClientPacket
函数功能：客户端信息摘要打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包后的HTTP验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
 参数.三：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.四：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.五：lpszRequestMethod
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的方法
 参数.六：lpszRequestUri
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的URL地址
 参数.七：lpszNonceStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务端返回的随机数
 参数.八：lpszOpaqueStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务端返回的不透明值
 参数.九：bQOPBody
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：请求的QOP保护级别,默认为false
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_DigestClientPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszRequestMethod, LPCXSTR lpszRequestUri, LPCXSTR lpszNonceStr, LPCXSTR lpszOpaqueStr, bool bQOPBody /* = false */)
{
	Verification_IsErrorOccur = false;

	if ((NULL == ptszMSGBuffer) || (NULL == pInt_MSGLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT;
		return false;
	}
	XCHAR tszCNonceStr[XPATH_MIN] = {};
	XCHAR tszResponseStr[XPATH_MAX] = {};
	BaseLib_Handle_CreateStr(tszCNonceStr, 32);
	Verification_HTTP_DigestVer(tszResponseStr, lpszUser, lpszPass, lpszRequestMethod, lpszRequestUri, lpszNonceStr, tszCNonceStr);

	int nRet = 0;
	XCHAR tszMSGBuffer[1024] = {};
	if (bQOPBody)
	{

	}
	else
	{
		nRet = _xstprintf(tszMSGBuffer, _X("Authorization: Digest username=\"%s\","
			"realm=\"%s\","
			"nonce=\"%s\","
			"uri=\"%s\","
			"qop=\"auth\","
			"nc=00000001,"
			"cnonce=\"%s\","
			"response=\"%s\","
			"opaque=\"%s\"\r\n"), lpszUser, lpszRealm, lpszNonceStr, lpszRequestUri, tszCNonceStr, tszResponseStr, lpszOpaqueStr);
	}

	return true;
}
/********************************************************************
函数名称：Verification_HTTP_BasicServerPacket
函数功能：服务端请求基本验证打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包后的HTTP验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_BasicServerPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	Verification_IsErrorOccur = false;

	if ((NULL == ptszMSGBuffer) || (NULL == pInt_MSGLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT;
		return false;
	}
	*pInt_MSGLen = _xstprintf(ptszMSGBuffer, "WWW-Authenticate: Basic realm=\"%s\"\r\n", lpszRealm);
	return true;
}
/********************************************************************
函数名称：Verification_HTTP_DigestServerPacket
函数功能：服务端请求数据摘要打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包后的HTTP验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_DigestServerPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, XCHAR* ptszNonceStr, XCHAR* ptszOpaqueStr, bool bQOPBody /* = false */)
{
	Verification_IsErrorOccur = false;

	if ((NULL == ptszMSGBuffer) || (NULL == pInt_MSGLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT;
		return false;
	}
	//realm：认证区域（字符串，由服务端定义）。
	//nonce：服务端生成的随机数（防重放攻击）。
	//opaque：服务端生成的不透明值，客户端必须原样返回。
	///qop：保护级别，常见 "auth" 或 "auth-int"。
	BaseLib_Handle_CreateStr(ptszNonceStr, 32);
	BaseLib_Handle_CreateStr(ptszOpaqueStr, 32);
	int nRet = 0;
	if (bQOPBody)
	{

	}
	else
	{
		nRet = _xsntprintf(ptszMSGBuffer,XPATH_MAX, _X("WWW-Authenticate: Digest realm=\"%s\","
			"qop=\"auth\","
			"nonce=\"%s\","
			"opaque=\"%s\"\r\n"), lpszRealm, ptszNonceStr, ptszOpaqueStr);
	}

	return true;
}
/********************************************************************
函数名称：Verification_HTTP_GetType
函数功能：获得HTTP验证类型
 参数.一：pptszListHdr
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入HTTP头
 参数.二：nHdrCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入HTTP头个数
 参数.三：pInt_Type
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出HTTP验证类型1：BASIC验证 2：摘要验证
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_GetType(XCHAR** pptszListHdr, int nHdrCount, int* pInt_Type)
{
	Verification_IsErrorOccur = false;

	if (NULL == pInt_Type)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT;
		return false;
	}
	int nAuthType = 0;
	int nAuthLen = XPATH_MAX;
	XCHAR tszAuthStr[XPATH_MAX] = {};
	if (!HttpProtocol_ServerHelp_GetAuthInfo(&pptszListHdr, nHdrCount, tszAuthStr, &nAuthLen, &nAuthType))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = HttpProtocol_GetLastError();
		return false;
	}

	if (1 != nAuthType && 2 != nAuthType)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_NOTSUPPORT;
		return false;
	}
	*pInt_Type = nAuthType;
	return true;
}
/********************************************************************
函数名称：Verification_HTTP_Basic
函数功能：HTTP基本验证
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要验证的用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要验证的密码
 参数.三：pptszListHdr
  In/Out：In
  类型：指向指针的指针
  可空：N
  意思：输入要解析的HTTP头
 参数.四：nHdrCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的HTTP头列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_Basic(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR** pptszListHdr, int nHdrCount)
{
	Verification_IsErrorOccur = false;

	if ((NULL == lpszUser) || (NULL == lpszPass))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT;
		return false;
	}
	int nAuthType = 0;
	int nAuthLen = XPATH_MAX;
	XCHAR tszAuthStr[XPATH_MAX] = {};
	if (!HttpProtocol_ServerHelp_GetAuthInfo(&pptszListHdr, nHdrCount, tszAuthStr, &nAuthLen, &nAuthType))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = HttpProtocol_GetLastError();
		return false;
	}
	//是否是BASIC
	if (1 != nAuthType)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_NOTSUPPORT;
		return false;
	}
	XCHAR tszUserStr[XPATH_MIN] = {};
	XCHAR tszPassStr[XPATH_MIN] = {};
	if (!Verification_HTTP_BasicDecoder(tszAuthStr, tszUserStr, tszPassStr))
	{
		return false;
	}
	if (0 != _tcsxnicmp(lpszUser, tszUserStr, _tcsxlen(lpszUser)))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_RESULT;
		return false;
	}
	if (0 != _tcsxnicmp(lpszPass, tszPassStr, _tcsxlen(lpszPass)))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_RESULT;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：Verification_HTTP_Digest
函数功能：HTTP摘要验证
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要验证的用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要验证的密码
 参数.三：lpszMethod
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：客户端请求的方法
 参数.四：pptszListHdr
  In/Out：In
  类型：指向指针的指针
  可空：N
  意思：输入要解析的HTTP头
 参数.五：nHdrCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的HTTP头列表个数
 参数.六：lpszNonceStr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要验证服务端设置的NONCE字符串
 参数.七：lpszOpaqueStr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要验证服务端设置的OPAQUE字符串
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_Digest(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszMethod, XCHAR** pptszListHdr, int nHdrCount, LPCXSTR lpszNonceStr /* = NULL */, LPCXSTR lpszOpaqueStr /* = NULL */)
{
	Verification_IsErrorOccur = false;

	if ((NULL == lpszUser) || (NULL == lpszPass))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT;
		return false;
	}

	int nAuthType = 0;
	int nAuthLen = XPATH_MAX;
	XCHAR tszAuthStr[XPATH_MAX] = {};

	if (!HttpProtocol_ServerHelp_GetAuthInfo(&pptszListHdr, nHdrCount, tszAuthStr, &nAuthLen, &nAuthType))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = HttpProtocol_GetLastError();
		return false;
	}
	//是否是摘要验证
	if (2 != nAuthType)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_NOTSUPPORT;
		return false;
	}
	XCHAR tszStrUser[XPATH_MIN] = {};
	XCHAR tszStrReal[XPATH_MIN] = {};
	XCHAR tszStrNonce[XPATH_MIN] = {};
	XCHAR tszStrUri[XPATH_MIN] = {};
	XCHAR tszStrResponse[XPATH_MIN] = {};
	XCHAR tszStrOpaque[XPATH_MIN] = {};
	XCHAR tszStrQop[XPATH_MIN] = {};
	XCHAR tszStrNC[XPATH_MIN] = {};
	XCHAR tszStrCnoce[XPATH_MIN] = {};
	//username="123123aa", realm="XEngine_Verification", nonce="e97lo7PMLx0e9StKBOE3oi76rzwmdoHv", uri="/", response="45c1f3bf882edea4e12e8119eee58397", opaque="NJrbSpvHXI7iruMOODb7K8RyYQ8bUKM4", qop=auth, nc=00000002, cnonce="2ba467fb3ee6b932"
	XCHAR* ptszTokTmp = NULL;
	XCHAR* ptszTokStr = _tcsxtok_s(tszAuthStr, _X(","), &ptszTokTmp);
	while (NULL != ptszTokStr)
	{
		int nPos = 0;
		//是否有空格
		if (' ' == ptszTokStr[0])
		{
			nPos++;
		}
		XCHAR tszStrKey[64] = {};
		XCHAR tszStrVlu[64] = {};
		LPCXSTR lpszStrUser = _X("username");
		LPCXSTR lpszStrReal = _X("realm");
		LPCXSTR lpszStrNonce = _X("nonce");
		LPCXSTR lpszStrUri = _X("uri");
		LPCXSTR lpszStrResponse = _X("response");
		LPCXSTR lpszStrOpaque = _X("opaque");
		LPCXSTR lpszStrQop = _X("qop");
		LPCXSTR lpszStrNC = _X("nc");
		LPCXSTR lpszStrCnoce = _X("cnonce");
		BaseLib_String_GetKeyValueA(ptszTokStr + nPos, _X("="), tszStrKey, tszStrVlu);
		if (0 == _tcsxnicmp(lpszStrUser, tszStrKey, _tcsxlen(lpszStrUser)))
		{
			BaseLib_String_GetStartEnd(tszStrVlu, tszStrUser, _X("\""), _X("\""));
		}
		else if (0 == _tcsxnicmp(lpszStrReal, tszStrKey, _tcsxlen(lpszStrReal)))
		{
			BaseLib_String_GetStartEnd(tszStrVlu, tszStrReal, _X("\""), _X("\""));
		}
		else if (0 == _tcsxnicmp(lpszStrNonce, tszStrKey, _tcsxlen(lpszStrNonce)))
		{
			BaseLib_String_GetStartEnd(tszStrVlu, tszStrNonce, _X("\""), _X("\""));
		}
		else if (0 == _tcsxnicmp(lpszStrUri, tszStrKey, _tcsxlen(lpszStrUri)))
		{
			BaseLib_String_GetStartEnd(tszStrVlu, tszStrUri, _X("\""), _X("\""));
		}
		else if (0 == _tcsxnicmp(lpszStrResponse, tszStrKey, _tcsxlen(lpszStrResponse)))
		{
			BaseLib_String_GetStartEnd(tszStrVlu, tszStrResponse, _X("\""), _X("\""));
		}
		else if (0 == _tcsxnicmp(lpszStrOpaque, tszStrKey, _tcsxlen(lpszStrOpaque)))
		{
			BaseLib_String_GetStartEnd(tszStrVlu, tszStrOpaque, _X("\""), _X("\""));
		}
		else if (0 == _tcsxnicmp(lpszStrQop, tszStrKey, _tcsxlen(lpszStrQop)))
		{
			_tcsxcpy(tszStrQop, tszStrVlu);
		}
		else if (0 == _tcsxnicmp(lpszStrNC, tszStrKey, _tcsxlen(lpszStrNC)))
		{
			_tcsxcpy(tszStrNC, tszStrVlu);
		}
		else if (0 == _tcsxnicmp(lpszStrCnoce, tszStrKey, _tcsxlen(lpszStrCnoce)))
		{
			BaseLib_String_GetStartEnd(tszStrVlu, tszStrCnoce, _X("\""), _X("\""));
		}
		ptszTokStr = _tcsxtok_s(NULL, _X(","), &ptszTokTmp);
	}
	//信息检查
	if (NULL != lpszNonceStr)
	{
		if (0 == _tcsxnicmp(lpszNonceStr, tszStrNonce, _tcsxlen(lpszNonceStr)))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_NONCE;
			return false;
		}
	}
	if (NULL != lpszOpaqueStr)
	{
		if (0 == _tcsxnicmp(lpszOpaqueStr, tszStrOpaque, _tcsxlen(lpszOpaqueStr)))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_OPAQUE;
			return false;
		}
	}
	
	XCHAR tszResponseStr[XPATH_MAX] = {};
	Verification_HTTP_DigestVer(tszResponseStr, lpszUser, lpszPass, lpszMethod, tszStrUri, tszStrNonce, tszStrCnoce, tszStrNC);
	if (0 != _tcsxnicmp(tszStrResponse, tszResponseStr, _tcsxlen(tszResponseStr)))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_RESULT;
		return false;
	}
	
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Verification_HTTP_DigestVer
函数功能：摘要计算函数
 参数.一：ptszResponseStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出计算后的RESPONSE字符串
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
 参数.四：lpszMethod
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求的方法
 参数.五：lpszUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求的URL地址
 参数.六：lpszNonce
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器生成并且发送的
 参数.七：lpszCNonce
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：客户端生成的随机字符串
 参数.八：lpszNC
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：验证的次数,建议为NULL.
返回值
  类型：逻辑型
  意思：是否成功
备注：信息摘要支持QOP=AUTH 算法MD5的验证模式
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_DigestVer(XCHAR* ptszResponseStr, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszMethod, LPCXSTR lpszUrl, LPCXSTR lpszNonce, LPCXSTR lpszCNonce, LPCXSTR lpszNC /* = _X("00000001") */, bool bQOPBody /* = false */, LPCXSTR lpszMSGBody /* = NULL */)
{
	Verification_IsErrorOccur = false;

	XCHAR tszMD1Com[XPATH_MIN] = {};
	XBYTE tszMD1Hex[XPATH_MIN] = {};
	XCHAR tszMD1Str[XPATH_MIN] = {};
	//计算HA1 的MD5字符串 MD5(USER:REAL:PASS)
	int nRet = _xstprintf(tszMD1Com, _X("%s:%s:%s"), lpszUser, lpszRealm, lpszPass);
	if (!Cryption_Api_Digest(tszMD1Com, tszMD1Hex, &nRet, false, ENUM_XENGINE_CRYPTION_DIGEST_MD5))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	BaseLib_String_StrToHex((XCHAR*)tszMD1Hex, nRet, tszMD1Str);
	Verification_HTTP_ConvertToLower(tszMD1Str);
	//计算HA2 的MD5字符串,根据验证模式来计算
	XCHAR tszMD2Com[XPATH_MIN] = {};
	XBYTE tszMD2Hex[XPATH_MIN] = {};
	XCHAR tszMD2Str[XPATH_MIN] = {};
	if (bQOPBody)
	{
		nRet = _xstprintf(tszMD2Com, _X("%s:%s:%s"), lpszMethod, lpszUrl, lpszMSGBody);
	}
	else
	{
		nRet = _xstprintf(tszMD2Com, _X("%s:%s"), lpszMethod, lpszUrl);
	}
	if (!Cryption_Api_Digest(tszMD2Com, tszMD2Hex, &nRet, false, ENUM_XENGINE_CRYPTION_DIGEST_MD5))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	BaseLib_String_StrToHex((XCHAR*)tszMD2Hex, nRet, tszMD2Str);
	Verification_HTTP_ConvertToLower(tszMD2Str);
	//计算RESPONSE值
	XCHAR tszResponseStr[XPATH_MAX] = {};
	XBYTE tszResponseMD5[XPATH_MID] = {};
	if (bQOPBody)
	{
		nRet = _xstprintf(tszResponseStr, _X("%s:%s:%s:%s:auth-int:%s"), tszMD1Str, lpszNonce, lpszNC, lpszCNonce, tszMD2Str);
	}
	else
	{
		nRet = _xstprintf(tszResponseStr, _X("%s:%s:%s:%s:auth:%s"), tszMD1Str, lpszNonce, lpszNC, lpszCNonce, tszMD2Str);
	}
	if (!Cryption_Api_Digest(tszResponseStr, tszResponseMD5, &nRet, false, ENUM_XENGINE_CRYPTION_DIGEST_MD5))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	BaseLib_String_StrToHex((XCHAR*)tszResponseMD5, nRet, ptszResponseStr);
	Verification_HTTP_ConvertToLower(ptszResponseStr);
	return true;
}
/********************************************************************
函数名称：APIHelp_Cryption_BasicEncoder
函数功能：HTTP基本验证加密函数
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.三：ptszMsgBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出加密后的缓冲区
 参数.四：bADD
  In/Out：In
  类型：逻辑型
  可空：N
  意思：是否添加验证算法类型
返回值
  类型：逻辑型
  意思：是否成功
备注：格式:Basic BASE64
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_BasicEncoder(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR* ptszMsgBuffer, bool bADD /* = true */)
{
	Verification_IsErrorOccur = false;

	if ((NULL == lpszUser) || (NULL == lpszPass) || (NULL == ptszMsgBuffer))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT;
		return false;
	}
	XCHAR tszMsgBuffer[2048];
	XCHAR tszBaseBuffer[2048];

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(tszBaseBuffer, '\0', sizeof(tszBaseBuffer));

	int nLen = _xstprintf(tszMsgBuffer, _X("%s:%s"), lpszUser, lpszPass);
	if (!Cryption_Codec_Base64(tszMsgBuffer, tszBaseBuffer, &nLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	if (bADD)
	{
		_xstprintf(ptszMsgBuffer, _X("Authorization: Basic %s\r\n"), tszBaseBuffer);
	}
	else
	{
		_tcsxcpy(ptszMsgBuffer, tszBaseBuffer);
	}
	return true;
}
/********************************************************************
函数名称：APIHelp_Cryption_BasicDecoder
函数功能：HTTP基本验证解密函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解密的缓冲区
 参数.二：ptszUser
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出用户名
 参数.三：ptszPass
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出密码
返回值
  类型：逻辑型
  意思：是否成功
备注：HttpProtocol_ServerHelp_GetAuthInfo 返回的内容
	  格式:Basic BASE64
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_BasicDecoder(LPCXSTR lpszMsgBuffer, XCHAR* ptszUser, XCHAR* ptszPass)
{
	Verification_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == ptszUser) || (NULL == ptszPass))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT;
		return false;
	}
	XCHAR tszBaseBuffer[2048];
	memset(tszBaseBuffer, '\0', sizeof(tszBaseBuffer));
	//是否有Basic
	if (NULL == _tcsxstr(lpszMsgBuffer, _X("Basic")))
	{
		int nMsgLen = _tcsxlen(lpszMsgBuffer);
		if (!Cryption_Codec_Base64(lpszMsgBuffer, tszBaseBuffer, &nMsgLen, false))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = Cryption_GetLastError();
			return false;
		}
	}
	else
	{
		int nMsgLen = _tcsxlen(lpszMsgBuffer) - 6;
		if (!Cryption_Codec_Base64(lpszMsgBuffer + 6, tszBaseBuffer, &nMsgLen, false))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = Cryption_GetLastError();
			return false;
		}
	}
	//分割用户名和密码
	XCHAR* ptszPassString = NULL;
	XCHAR* ptszUserString = _tcsxtok_s(tszBaseBuffer, _X(":"), &ptszPassString);
	if (NULL == ptszUserString)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_NOTSUPPORT;
		return false;
	}
	_tcsxcpy(ptszUser, ptszUserString);
	_tcsxcpy(ptszPass, ptszPassString);

	return true;
}
/********************************************************************
函数名称：Verification_HTTP_ConvertToLower
函数功能：转换为小写
 参数.一：ptszMD5Str
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输入要转换的字符串,输出转换后的字符串
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_HTTP::Verification_HTTP_ConvertToLower(XCHAR* ptszMD5Str)
{
	Verification_IsErrorOccur = false;

	while (*ptszMD5Str)
	{
		*ptszMD5Str = tolower((unsigned char)*ptszMD5Str);
		ptszMD5Str++;
	}

	return true;
}