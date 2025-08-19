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
	Verification_HTTP_Digest(tszResponseStr, lpszUser, lpszPass, lpszRequestMethod, lpszRequestUri, lpszNonceStr, tszCNonceStr);

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
			"opaque=\"%s\""), lpszUser, lpszRealm, lpszNonceStr, lpszRequestUri, tszCNonceStr, tszResponseStr, lpszOpaqueStr);
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
		nRet = _xsntprintf(ptszMSGBuffer,XPATH_MAX, _X("WWW-Authenticate: Digest realm=\"XEngine Verification\","
			"qop=\"auth\","
			"nonce=\"%s\","
			"opaque=\"%s\""), ptszNonceStr, ptszOpaqueStr);
	}

	return true;
}

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

	if (1 != nAuthType || 2 != nAuthType)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_HTTP_NOTSUPPORT;
		return false;
	}
	*pInt_Type = nAuthType;
	return true;
}
/********************************************************************
函数名称：AuthHelp_APIHelp_HttpAuth
函数功能：HTTP验证
 参数.一：ptszUser
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出用户名
 参数.二：ptszPass
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出密码
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
bool CVerification_HTTP::Verification_HTTP_Basic(XCHAR* ptszUser, XCHAR* ptszPass, XCHAR** pptszListHdr, int nHdrCount)
{
	Verification_IsErrorOccur = false;

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
	Verification_HTTP_BasicDecoder(tszAuthStr, ptszUser, ptszPass);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Verification_HTTP_Digest
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
bool CVerification_HTTP::Verification_HTTP_Digest(XCHAR* ptszResponseStr, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszMethod, LPCXSTR lpszUrl, LPCXSTR lpszNonce, LPCXSTR lpszCNonce, LPCXSTR lpszNC /* = _X("00000001") */, bool bQOPBody /* = false */, LPCXSTR lpszMSGBody /* = NULL */)
{
	Verification_IsErrorOccur = false;

	XCHAR tszMD1Com[XPATH_MIN] = {};
	XBYTE tszMD1Hex[XPATH_MIN] = {};
	XCHAR tszMD1Str[XPATH_MIN] = {};
	//计算HA1 的MD5字符串 MD5(USER:PASS:REAL)
	int nRet = _xstprintf(tszMD1Com, _X("%s:XEngine_Verification:%s"), lpszUser, lpszPass);
	if (!Cryption_Api_Digest(tszMD1Com, tszMD1Hex, &nRet, false, ENUM_XENGINE_CRYPTION_DIGEST_MD5))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	BaseLib_String_StrToHex((XCHAR*)tszMD1Hex, nRet, tszMD1Str);
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
		_xstprintf(ptszMsgBuffer, _X("WWW-Authenticate: Basic %s"), tszBaseBuffer);
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