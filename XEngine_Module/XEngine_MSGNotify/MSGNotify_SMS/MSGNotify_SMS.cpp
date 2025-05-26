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
{

}
CMSGNotify_SMS::~CMSGNotify_SMS()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：MSGNotify_SMS_SendTencent
函数功能：腾讯API发送短信消息
 参数.一：lpszServiceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszRegion
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的地区
 参数.三：lpszAPPID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的APP ID
 参数.四：lpszSignName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：签名内容
 参数.五：lpszTemplateID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的模版ID
 参数.六：lpszPhoneNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送给谁 +8613333
 参数.七：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：发送的附加会话信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CMSGNotify_SMS::MSGNotify_SMS_SendTencent(LPCXSTR lpszServiceAddr, LPCXSTR lpszRegion, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, LPCXSTR lpszPhoneNumber, LPCXSTR lpszMSGBuffer /* = NULL */)
{
	MSGNotify_IsErrorOccur = false;

	if ((NULL == lpszServiceAddr) || (NULL == lpszRegion) || (NULL == lpszAPPID) || (NULL == lpszSignName) || (NULL == lpszTemplateID) || (NULL == lpszPhoneNumber))
	{
		MSGNotify_IsErrorOccur = false;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PARAMENT;
		return false;
	}
	XCHAR tszHTTPHdr[XPATH_MAX] = {};
	_xstprintf(tszHTTPHdr, _X("Content-Type: application/json\r\n"
							  "X-TC-Action: SendSms\r\n"
	                          "X-TC-Version: 2021-01-11\r\n"
	                          "X-TC-Region: %s\r\n"), lpszRegion);     //ap-guangzhou     

	Json::Value st_JsonRoot;
	Json::StreamWriterBuilder st_JsonWrite;
	Json::Value st_JsonArrayPhone(Json::arrayValue);
	Json::Value st_JsonArrayParam(Json::arrayValue);

	st_JsonArrayPhone.append(lpszPhoneNumber);

	st_JsonRoot["PhoneNumberSet"] = st_JsonArrayPhone;
	st_JsonRoot["TemplateId"] = lpszTemplateID;             //模板 ID，必须填写已审核通过的模板 ID
	st_JsonRoot["SmsSdkAppId"] = lpszAPPID;                 //短信 SdkAppId，在 短信控制台 添加应用后生成的实际 SdkAppId
	st_JsonRoot["SignName"] = lpszSignName;                 //短信签名内容，使用 UTF-8 编码，必须填写已审核通过的签名
	st_JsonRoot["TemplateParamSet"] = st_JsonArrayParam;    //模版参数,默认为空
	st_JsonRoot["SessionContext"] = lpszMSGBuffer;
	// 输出 JSON（格式化输出）
	xstring m_StrHTTPBody = Json::writeString(st_JsonWrite, st_JsonRoot);

	int nHTTPCode = 0;
	int nHTTPSize = 0;
	XCHAR* ptszMSGBuffer = NULL;
	if (!APIClient_Http_Request(_X("POST"), lpszServiceAddr, m_StrHTTPBody.c_str(), &nHTTPCode, &ptszMSGBuffer, &nHTTPSize, tszHTTPHdr))
	{
		MSGNotify_IsErrorOccur = false;
		MSGNotify_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMSGBuffer, ptszMSGBuffer + nHTTPSize, &st_JsonRoot, &st_JsonError))
	{
		MSGNotify_IsErrorOccur = false;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PARSEJSON;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["Response"];
	if (st_JsonObject.isNull())
	{
		MSGNotify_IsErrorOccur = false;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PAYLOAD;
		return false;
	}
	Json::Value st_JsonResponse = st_JsonRoot["SendStatusSet"];
	for (unsigned int i = 0; i < st_JsonResponse.size(); i++)
	{
		//返回的状态码
		if (0 != _tcsxnicmp(_X("Ok"), st_JsonResponse[i]["Code"].asCString(), 2))
		{
			MSGNotify_IsErrorOccur = false;
			MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_FAILED;
			return false;
		}
	}
    return true;
}
/********************************************************************
函数名称：MSGNotify_SMS_SendAliyun
函数功能：阿里云API发送短信消息
 参数.一：lpszServiceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszRegion
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的地区
 参数.三：lpszAPPID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的APP ID
 参数.四：lpszSignName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：签名内容
 参数.五：lpszTemplateID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的模版ID
 参数.六：lpszPhoneNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送给谁 +8613333
 参数.七：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：发送的附加会话信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CMSGNotify_SMS::MSGNotify_SMS_SendAliyun(LPCXSTR lpszServiceAddr, LPCXSTR lpszRegion, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, LPCXSTR lpszPhoneNumber, LPCXSTR lpszMSGBuffer)
{
	MSGNotify_IsErrorOccur = false;

	if ((NULL == lpszServiceAddr) || (NULL == lpszRegion) || (NULL == lpszAPPID) || (NULL == lpszSignName) || (NULL == lpszTemplateID) || (NULL == lpszPhoneNumber))
	{
		MSGNotify_IsErrorOccur = false;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PARAMENT;
		return false;
	}
	XCHAR tszHTTPHdr[XPATH_MAX] = {};
	_xstprintf(tszHTTPHdr, _X("Content-Type: application/json\r\n"
		"x-acs-action: SendSms\r\n"
		"x-acs-version: 2021-01-11\r\n"
		"x-acs-signature-nonce: %s\r\n"), lpszRegion);     //ap-guangzhou     

	Json::Value st_JsonRoot;
	Json::StreamWriterBuilder st_JsonWrite;
	Json::Value st_JsonArrayParam(Json::arrayValue);

	st_JsonRoot["PhoneNumbers"] = lpszPhoneNumber;
	st_JsonRoot["SignName"] = lpszTemplateID;               //短信签名内容，使用 UTF-8 编码，必须填写已审核通过的签名
	st_JsonRoot["TemplateCode"] = lpszAPPID;                //模板 ID，必须填写已审核通过的模板 ID
	st_JsonRoot["TemplateParam"] = st_JsonArrayParam;       //模版参数,默认为空
	// 输出 JSON（格式化输出）
	xstring m_StrHTTPBody = Json::writeString(st_JsonWrite, st_JsonRoot);

	int nHTTPCode = 0;
	int nHTTPSize = 0;
	XCHAR* ptszMSGBuffer = NULL;
	if (!APIClient_Http_Request(_X("POST"), lpszServiceAddr, m_StrHTTPBody.c_str(), &nHTTPCode, &ptszMSGBuffer, &nHTTPSize, tszHTTPHdr))
	{
		MSGNotify_IsErrorOccur = false;
		MSGNotify_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMSGBuffer, ptszMSGBuffer + nHTTPSize, &st_JsonRoot, &st_JsonError))
	{
		MSGNotify_IsErrorOccur = false;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_PARSEJSON;
		return false;
	}
	//返回的状态码
	if (0 != _tcsxnicmp(_X("Ok"), st_JsonRoot["Code"].asCString(), 2))
	{
		MSGNotify_IsErrorOccur = false;
		MSGNotify_dwErrorCode = ERROR_XENGINE_THIRDPART_MSGNOTIFY_FAILED;
		return false;
	}
	return true;
}