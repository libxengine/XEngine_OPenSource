#include "pch.h"
#include "MSGNotify_SMS.h"
/********************************************************************
//    Created:     2025/05/20  14:29:03
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\MSGNotify_SMS\MSGNotify_SMS.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\MSGNotify_SMS
//    File Base:   MSGNotify_SMS
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     短信通知
//    History:
*********************************************************************/
CMSGNotify_SMS::CMSGNotify_SMS()
{}
CMSGNotify_SMS::~CMSGNotify_SMS()
{}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：MSGNotify_SMS_SendTencent
函数功能：腾讯API发送短信消息
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszSecretID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的秘钥ID
 参数.三：lpszSecretKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的秘钥KEY
 参数.四：lpszRegionStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的地区 (如: ap-guangzhou)
 参数.五：lpszAPPID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的APP ID
 参数.六：lpszSignName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：签名内容
 参数.七：lpszTemplateID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的模版ID
 参数.八：ppptszPhoneNumber
  In/Out：In
  类型：三级指针
  可空：N
  意思：发送给谁 +8613333
 参数.九：nListCount
  In/Out：In
  类型：整数型
  可空：Y
  意思：发送的数量
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CMSGNotify_SMS::MSGNotify_SMS_SendTencent(LPCXSTR lpszAPIUrl, LPCXSTR lpszSecretID, LPCXSTR lpszSecretKey, LPCXSTR lpszRegionStr, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, XCHAR*** ppptszPhoneNumber, int nListCount)
{
	MSGNotify_IsErrorOccur = false;

	if ((NULL == lpszAPIUrl) || (NULL == lpszRegionStr) || (NULL == lpszAPPID) || (NULL == lpszSignName) || (NULL == lpszTemplateID))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PARAMENT;
		return false;
	}
	//负载数据
	Json::Value st_JsonRoot;
	Json::StreamWriterBuilder st_JsonWrite;
	Json::Value st_JsonArrayPhone(Json::arrayValue);

	for (int i = 0; i < nListCount; i++)
	{
		st_JsonArrayPhone.append((*ppptszPhoneNumber)[i]);
	}
	st_JsonRoot["PhoneNumberSet"] = st_JsonArrayPhone;
	st_JsonRoot["TemplateId"] = lpszTemplateID;             //模板 ID，必须填写已审核通过的模板 ID
	st_JsonRoot["SmsSdkAppId"] = lpszAPPID;                 //短信 SdkAppId，在 短信控制台 添加应用后生成的实际 SdkAppId
	st_JsonRoot["SignName"] = lpszSignName;                 //短信签名内容，使用 UTF-8 编码，必须填写已审核通过的签名
	// 输出 JSON
	xstring m_StrHTTPBody = Json::writeString(st_JsonWrite, st_JsonRoot);
	// HTTP 请求头
	APIADDR_DOMAIN st_APIDomain = {};
	ENUM_NETHELP_APIADDR_DOMAIN_TYPE enDomainType;
	if (!APIAddr_Domain_GetInfo(lpszAPIUrl, &st_APIDomain, &enDomainType))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = APIAddr_GetLastError();
		return false;
	}

	__int64u m_nTime = time(NULL);
	XCHAR tszTimeStr[20] = {};
	MSGNotify_SMS_UTCTime((time_t)m_nTime, tszTimeStr);
	// ************* 拼接规范请求串 *************
	LPCXSTR lpszHTTPMethod = "POST";
	LPCXSTR lpszHTTPUri = "/";
	LPCXSTR lpszHTTPQuery = "";

	XCHAR tszCanonicalHeaders[XPATH_MAX] = {};
	_xstprintf(tszCanonicalHeaders, _X("content-type:application/json; charset=utf-8\nhost:%s\n"), st_APIDomain.tszFullDomain);
	LPCXSTR lpszSignedHeaders = _X("content-type;host");

	XCHAR tszHashPayload[XPATH_MIN] = {};
	MSGNotify_SMS_Sha256Hex(m_StrHTTPBody.c_str(), tszHashPayload);

	XCHAR tszCanonicalRequest[XPATH_MID] = {};
	_xstprintf(tszCanonicalRequest, _X("%s\n%s\n%s\n%s\n%s\n%s"), lpszHTTPMethod, lpszHTTPUri, lpszHTTPQuery, tszCanonicalHeaders, lpszSignedHeaders, tszHashPayload);
	// ************* 拼接待签名字符串 *************
	LPCXSTR lpszAlgorithmStr = _X("TC3-HMAC-SHA256");

	XCHAR tszCredentialScope[XPATH_MIN] = {};
	_xstprintf(tszCredentialScope, _X("%s/%s/tc3_request"), tszTimeStr, st_APIDomain.tszSubDomain);
	XCHAR tszHashCanonicalRequest[XPATH_MIN] = {};
	MSGNotify_SMS_Sha256Hex(tszCanonicalRequest, tszHashCanonicalRequest);
	XCHAR tszString2Sign[XPATH_MID] = {};
	_xstprintf(tszString2Sign, _X("%s\n%lld\n%s\n%s"), lpszAlgorithmStr, m_nTime, tszCredentialScope, tszHashCanonicalRequest);
	// ************* 计算签名 ***************
	XCHAR tszKeyStr[64] = {};
	_xstprintf(tszKeyStr, _X("TC3%s"), lpszSecretKey);

	XBYTE k_date[64] = {};
	int output_len = (int)_tcsxlen(tszTimeStr);
	Cryption_Api_HMac(tszTimeStr, k_date, &output_len, tszKeyStr, ENUM_XENGINE_CRYPTION_DIGEST_SHA256, (int)_tcsxlen(tszKeyStr));

	XBYTE k_service[64] = {};
	int nLen = (int)_tcsxlen(st_APIDomain.tszSubDomain);
	Cryption_Api_HMac((LPCXSTR)st_APIDomain.tszSubDomain, k_service, &nLen, (LPCXSTR)k_date, ENUM_XENGINE_CRYPTION_DIGEST_SHA256, output_len);
	output_len = nLen;

	XBYTE k_signing[64] = {};
	nLen = (int)_tcsxlen("tc3_request");
	Cryption_Api_HMac((LPCXSTR)"tc3_request", k_signing, &nLen, (LPCXSTR)k_service, ENUM_XENGINE_CRYPTION_DIGEST_SHA256, output_len);
	output_len = nLen;

	XBYTE k_hmac_sha_sign[64] = {};
	nLen = (int)_tcsxlen(tszString2Sign);
	Cryption_Api_HMac((LPCXSTR)tszString2Sign, k_hmac_sha_sign, &nLen, (LPCXSTR)k_signing, ENUM_XENGINE_CRYPTION_DIGEST_SHA256, output_len);
	output_len = nLen;

	XCHAR tszSignature[128] = {};
	BaseLib_String_HexToStr((LPCXSTR)k_hmac_sha_sign, output_len, tszSignature, false);
	// ************* 拼接 Authorization *************
	XCHAR tszAuthorization[XPATH_MID] = {};
	_xstprintf(tszAuthorization, _X("%s Credential=%s/%s, SignedHeaders=%s, Signature=%s"), lpszAlgorithmStr, lpszSecretID, tszCredentialScope, lpszSignedHeaders, tszSignature);

	XCHAR tszHTTPHdr[XPATH_MAX] = {};
	_xstprintf(tszHTTPHdr, "Authorization: %s\r\n"
		"Content-Type: application/json; charset=utf-8\r\n"
		"X-TC-Action: SendSms\r\n"
		"X-TC-Timestamp: %lld\r\n"
		"X-TC-Version: 2021-01-11\r\n"
		"X-TC-Region: %s\r\n",
		tszAuthorization, m_nTime, lpszRegionStr);
	/////请求
	int nHTTPCode = 0;
	int nHTTPSize = 0;
	XCHAR* ptszMSGBuffer = NULL;
	if (!APIClient_Http_Request(_X("POST"), lpszAPIUrl, m_StrHTTPBody.c_str(), &nHTTPCode, &ptszMSGBuffer, &nHTTPSize, tszHTTPHdr))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	st_JsonRoot.clear();

	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMSGBuffer, ptszMSGBuffer + nHTTPSize, &st_JsonRoot, &st_JsonError))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PARSEJSON;
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);
		return false;
	}
	// 释放 HTTP 响应内存
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);

	if (!st_JsonRoot.isMember("Response") || !st_JsonRoot["Response"].isObject())
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PAYLOAD;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["Response"];
	// SendStatusSet 位于 Response 对象内部
	Json::Value st_JsonResponse = st_JsonObject["SendStatusSet"];
	for (unsigned int i = 0; i < st_JsonResponse.size(); i++)
	{
		// 返回的状态码为 "Ok" 代表成功
		if (0 != _tcsxnicmp(_X("Ok"), st_JsonResponse[i]["Code"].asCString(), 2))
		{
			MSGNotify_IsErrorOccur = true;
			MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_FAILED;
			return false;
		}
	}
	return true;
}
/********************************************************************
函数名称：MSGNotify_SMS_SendTencent
函数功能：阿里云API发送短信消息
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszSecretID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的秘钥ID
 参数.三：lpszSecretKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的秘钥KEY
 参数.四：lpszRegionStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的地区
 参数.五：lpszAPPID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的APP ID
 参数.六：lpszSignName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：签名内容
 参数.七：lpszTemplateID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的模版ID
 参数.八：ppptszPhoneNumber
  In/Out：In
  类型：三级指针
  可空：N
  意思：发送给谁 
 参数.九：nListCount
  In/Out：In
  类型：整数型
  可空：Y
  意思：发送的数量
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CMSGNotify_SMS::MSGNotify_SMS_SendAliyun(LPCXSTR lpszAPIUrl, LPCXSTR lpszSecretID, LPCXSTR lpszSecretKey, LPCXSTR lpszRegionStr, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, XCHAR*** ppptszPhoneNumber, int nListCount)
{
	MSGNotify_IsErrorOccur = false;

	if ((NULL == lpszAPIUrl) || (NULL == lpszSecretID) || (NULL == lpszSecretKey) || (NULL == lpszSignName) || (NULL == lpszTemplateID) || (NULL == ppptszPhoneNumber) || (nListCount <= 0))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PARAMENT;
		return false;
	}
	// ************* 解析域名(用于 host 头) *************
	APIADDR_DOMAIN st_APIDomain = {};
	ENUM_NETHELP_APIADDR_DOMAIN_TYPE enDomainType;
	if (!APIAddr_Domain_GetInfo(lpszAPIUrl, &st_APIDomain, &enDomainType))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = APIAddr_GetLastError();
		return false;
	}
	// ************* 拼接查询参数(必须按参数名 ASCII 升序) *************
	// SendSms 的号码参数是 PhoneNumbers，逗号分隔
	xstring m_StrPhone;
	for (int i = 0; i < nListCount; i++)
	{
		if (i > 0)
		{
			m_StrPhone += ",";
		}
		m_StrPhone += (*ppptszPhoneNumber)[i];
	}
	xstring m_StrQuery;
	auto AddParam = [&](const char* lpszKey, const char* lpszValue)
		{
			if ((NULL == lpszValue) || ('\0' == lpszValue[0]))
			{
				return;
			}
			if (!m_StrQuery.empty())
			{
				m_StrQuery += "&";
			}
			m_StrQuery += lpszKey;
			m_StrQuery += "=";
			Cryption_Codec_UrlEnCodec(lpszValue, _tcsxlen(lpszValue), m_StrQuery.data());
		};
	// 顺序: OutId < PhoneNumbers < SignName < TemplateCode < TemplateParam
	AddParam("OutId", lpszAPPID);
	AddParam("PhoneNumbers", m_StrPhone.c_str());
	AddParam("SignName", lpszSignName);
	AddParam("TemplateCode", lpszTemplateID);
	// ************* 公共头部 *************
	time_t nTime = time(NULL);
	struct tm st_Time = {};
#ifdef _MSC_VER
	gmtime_s(&st_Time, &nTime);
#else
	gmtime_r(&nTime, &st_Time);
#endif
	char tszDateStr[32] = {};
	strftime(tszDateStr, sizeof(tszDateStr), "%Y-%m-%dT%H:%M:%SZ", &st_Time);

	// 每次请求唯一的随机数
	static thread_local std::mt19937_64 st_Random{ std::random_device{}() };
	char tszNonce[64] = {};
	snprintf(tszNonce, sizeof(tszNonce), "%016llx%016llx", (unsigned long long)st_Random(), (unsigned long long)st_Random());

	// body 为空，哈希为空串的 SHA256
	LPCXSTR lpszEmptyHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
	LPCXSTR lpszAlgorithmStr = "ACS3-HMAC-SHA256";
	LPCXSTR lpszSignedHeaders = "host;x-acs-action;x-acs-content-sha256;x-acs-date;x-acs-signature-nonce;x-acs-version";
	// ************* 规范请求串 *************
	// CanonicalHeaders 每行以 \n 结尾，后面再跟一个 \n 分隔，所以中间会有空行
	xstring m_StrCanonicalHeaders;
	m_StrCanonicalHeaders += "host:";
	m_StrCanonicalHeaders += st_APIDomain.tszFullDomain;
	m_StrCanonicalHeaders += "\n";
	m_StrCanonicalHeaders += "x-acs-action:SendSms\n";
	m_StrCanonicalHeaders += "x-acs-content-sha256:";
	m_StrCanonicalHeaders += lpszEmptyHash;
	m_StrCanonicalHeaders += "\n";
	m_StrCanonicalHeaders += "x-acs-date:";
	m_StrCanonicalHeaders += tszDateStr;
	m_StrCanonicalHeaders += "\n";
	m_StrCanonicalHeaders += "x-acs-signature-nonce:";
	m_StrCanonicalHeaders += tszNonce;
	m_StrCanonicalHeaders += "\n";
	m_StrCanonicalHeaders += "x-acs-version:2017-05-25\n";

	xstring m_StrCanonicalRequest;
	m_StrCanonicalRequest += "POST\n";
	m_StrCanonicalRequest += "/\n";
	m_StrCanonicalRequest += m_StrQuery;
	m_StrCanonicalRequest += "\n";
	m_StrCanonicalRequest += m_StrCanonicalHeaders;
	m_StrCanonicalRequest += "\n";
	m_StrCanonicalRequest += lpszSignedHeaders;
	m_StrCanonicalRequest += "\n";
	m_StrCanonicalRequest += lpszEmptyHash;
	// ************* 待签名字符串 *************
	XCHAR tszHashCanonicalRequest[XPATH_MIN] = {};
	if (!MSGNotify_SMS_Sha256Hex(m_StrCanonicalRequest.c_str(), tszHashCanonicalRequest))
	{
		return false;
	}
	xstring m_StrString2Sign;
	m_StrString2Sign += lpszAlgorithmStr;
	m_StrString2Sign += "\n";
	m_StrString2Sign += tszHashCanonicalRequest;
	// ************* 计算签名: HMAC-SHA256(AccessKeySecret, StringToSign) *************
	XBYTE tszHMacSign[64] = {};
	int nLen = (int)m_StrString2Sign.length();
	if (!Cryption_Api_HMac(m_StrString2Sign.c_str(), tszHMacSign, &nLen, lpszSecretKey, ENUM_XENGINE_CRYPTION_DIGEST_SHA256, (int)_tcsxlen(lpszSecretKey)))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	// 字节转小写十六进制(自己转换，避免依赖库函数的行为差异)
	XCHAR tszSignature[128] = {};
	for (int i = 0; i < nLen; i++)
	{
		snprintf(tszSignature + (i * 2), 3, "%02x", tszHMacSign[i]);
	}
	// ************* Authorization *************
	xstring m_StrHTTPHdr;
	m_StrHTTPHdr += "Authorization: ";
	m_StrHTTPHdr += lpszAlgorithmStr;
	m_StrHTTPHdr += " Credential=";
	m_StrHTTPHdr += lpszSecretID;
	m_StrHTTPHdr += ",SignedHeaders=";
	m_StrHTTPHdr += lpszSignedHeaders;
	m_StrHTTPHdr += ",Signature=";
	m_StrHTTPHdr += tszSignature;
	m_StrHTTPHdr += "\r\n";
	m_StrHTTPHdr += "x-acs-action: SendSms\r\n";
	m_StrHTTPHdr += "x-acs-version: 2017-05-25\r\n";
	m_StrHTTPHdr += "x-acs-date: ";
	m_StrHTTPHdr += tszDateStr;
	m_StrHTTPHdr += "\r\n";
	m_StrHTTPHdr += "x-acs-signature-nonce: ";
	m_StrHTTPHdr += tszNonce;
	m_StrHTTPHdr += "\r\n";
	m_StrHTTPHdr += "x-acs-content-sha256: ";
	m_StrHTTPHdr += lpszEmptyHash;
	m_StrHTTPHdr += "\r\n";
	// ************* 请求 URL: 参数在 query 中 *************
	xstring m_StrURL = lpszAPIUrl;
	while (!m_StrURL.empty() && ('/' == m_StrURL.back()))
	{
		m_StrURL.pop_back();
	}
	m_StrURL += "/?";
	m_StrURL += m_StrQuery;

	int nHTTPCode = 0;
	int nHTTPSize = 0;
	XCHAR* ptszMSGBuffer = NULL;
	if (!APIClient_Http_Request(_X("POST"), m_StrURL.c_str(), NULL, &nHTTPCode, &ptszMSGBuffer, &nHTTPSize, m_StrHTTPHdr.c_str()))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMSGBuffer, ptszMSGBuffer + nHTTPSize, &st_JsonRoot, &st_JsonError))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PARSEJSON;
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);
		return false;
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);

	if (!st_JsonRoot.isMember("Code") || !st_JsonRoot["Code"].isString())
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PAYLOAD;
		return false;
	}
	// 返回 "OK" 代表成功，完整比较避免前缀误判
	if (0 != _tcsxicmp(_X("OK"), st_JsonRoot["Code"].asCString()))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_FAILED;
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                    保护函数
//////////////////////////////////////////////////////////////////////////
bool CMSGNotify_SMS::MSGNotify_SMS_UTCTime(time_t nTime, XCHAR* ptszTimeStr)
{
	MSGNotify_IsErrorOccur = false;

	struct tm sttime;
	sttime = *gmtime(&nTime);
	strftime(ptszTimeStr, XPATH_MIN, "%Y-%m-%d", &sttime);

	return true;
}
bool CMSGNotify_SMS::MSGNotify_SMS_Sha256Hex(LPCXSTR lpszMSGBuffer, XCHAR* ptszMSGBuffer)
{
	MSGNotify_IsErrorOccur = false;

	XBYTE tszHashHex[32] = {};
	int nLen = (int)_tcsxlen(lpszMSGBuffer);

	if (!Cryption_Api_Digest(lpszMSGBuffer, tszHashHex, &nLen, false, ENUM_XENGINE_CRYPTION_DIGEST_SHA256))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	BaseLib_String_StrToHex((LPCXSTR)tszHashHex, 32, ptszMSGBuffer, false);

	return true;
}
bool CMSGNotify_SMS::MSGNotify_SMS_Sha1Hex(LPCXSTR lpszMSGBuffer, LPCXSTR lpszKeyStr, XCHAR* ptszMSGBuffer)
{
	MSGNotify_IsErrorOccur = false;

	XBYTE tszHashHex[32] = {};
	int nLen = (int)_tcsxlen(lpszMSGBuffer);

	if (!Cryption_Api_HMac(lpszMSGBuffer, tszHashHex, &nLen, lpszKeyStr, ENUM_XENGINE_CRYPTION_DIGEST_SHA1, (int)_tcsxlen(lpszKeyStr)))
	{
		MSGNotify_IsErrorOccur = true;
		MSGNotify_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	BaseLib_String_StrToHex((LPCXSTR)tszHashHex, nLen, ptszMSGBuffer, false);

	return true;
}