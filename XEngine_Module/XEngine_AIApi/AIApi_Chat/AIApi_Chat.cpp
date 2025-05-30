#include "pch.h"
#include "AIApi_Chat.h"
/********************************************************************
//    Created:     2025/05/29  13:57:18
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Chat\AIApi_Chat.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Chat
//    File Base:   AIApi_Chat
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     AI对话函数接口
//    History:
*********************************************************************/
CAIApi_Chat::CAIApi_Chat()
{
}
CAIApi_Chat::~CAIApi_Chat()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AIApi_Chat_Create
函数功能：AI对话模型初始化
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出初始化成功的句柄
 参数.二：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入模型API地址
 参数.三：lpszAPIKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入模型的KEY
 参数.四：fpCall_Chat
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：对话输出信息
 参数.五：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Chat::AIApi_Chat_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam /* = NULL */)
{
	AIApi_IsErrorOccur = true;

	if (NULL == pxhToken || NULL == lpszAPIUrl || NULL == lpszAPIKey)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_CHAT_PARAMENT;
		return false;
	}
	AICLIENT_CHAT *pSt_AIClient = new AICLIENT_CHAT;
	if (NULL == pSt_AIClient)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_CHAT_MALLOC;
		return false;
	}
	pSt_AIClient->lParam = lParam;
	pSt_AIClient->lpCall_Chat = fpCall_Chat;

	pSt_AIClient->nTimeStart = time(NULL);
	_xstrcpy(pSt_AIClient->tszAPIUrl, lpszAPIUrl, sizeof(pSt_AIClient->tszAPIUrl));
	_xsntprintf(pSt_AIClient->tszAPIHdr, sizeof(pSt_AIClient->tszAPIHdr), _X("Content-Type: application/json\r\nAuthorization: Bearer %s"), lpszAPIKey);

	if (!APIClient_Http_Create(&pSt_AIClient->xhToken, AIApi_Chat_CBRecv, pSt_AIClient))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	XCLIENT_APIHTTP st_HTTPParam = {};
	st_HTTPParam.nTimeConnect = 2000;
	if (!APIClient_Http_SetParam(pSt_AIClient->xhToken, &st_HTTPParam))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	if (!APIClient_Http_SetUrl(pSt_AIClient->xhToken, lpszAPIUrl, _X("POST")))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	*pxhToken = pSt_AIClient->xhToken;

	st_Locker.lock();
	stl_MapAIClient.insert(std::make_pair(*pxhToken, pSt_AIClient));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：AIApi_Chat_Destory
函数功能：销毁一个对话模型
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的模型句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Chat::AIApi_Chat_Destory(XNETHANDLE xhToken)
{
	AIApi_IsErrorOccur = false;

	st_Locker.lock();
	auto stl_MapIterator = stl_MapAIClient.find(xhToken);
	if (stl_MapIterator != stl_MapAIClient.end())
	{
		APIClient_Http_Close(stl_MapIterator->second->xhToken);

		delete stl_MapIterator->second;
		stl_MapIterator->second = NULL;
		stl_MapAIClient.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：AIApi_Chat_Excute
函数功能：执行对话
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszModelName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入模型名称
 参数.三：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要对话的数据
 参数.四：nMSGLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入对话数据大小
 参数.五：bStream
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否使用流式数据回复
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Chat::AIApi_Chat_Excute(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream /* = false */)
{
	AIApi_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapAIClient.find(xhToken);
	if (stl_MapIterator == stl_MapAIClient.end())
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_CHAT_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::Value st_JsonObject;
	Json::StreamWriterBuilder st_JsonBuilder;

	st_JsonBuilder["emitUTF8"] = true;

#ifdef _MSC_BUILD
	XCHAR tszUTFBuffer[8192] = {};
	BaseLib_Charset_AnsiToUTF(lpszMSGBuffer, tszUTFBuffer, &nMSGLen);
	st_JsonObject["content"] = tszUTFBuffer;
#else
	st_JsonObject["content"] = lpszMSGBuffer;
#endif
	st_JsonObject["role"] = "user";
	
	st_JsonArray.append(st_JsonObject);

	st_JsonRoot["model"] = lpszModelName;
	st_JsonRoot["stream"] = bStream;
	st_JsonRoot["messages"] = st_JsonArray;

	xstring m_StrBody = Json::writeString(st_JsonBuilder, st_JsonRoot);
	if (!APIClient_Http_Excute(xhToken, m_StrBody.c_str(), m_StrBody.length(), stl_MapIterator->second->tszAPIHdr))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		st_Locker.unlock_shared();
		return false;
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：AIApi_Chat_GetStatus
函数功能：获取执行状态
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pbComplete
  In/Out：Out
  类型：逻辑型指针
  可空：N
  意思：输出是否执行完毕
 参数.二：pInt_HTTPCode
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出HTTP状态码
返回值
  类型：逻辑型
  意思：是否成功
备注：pbComplete没有执行完毕需要等待执行完毕
*********************************************************************/
bool CAIApi_Chat::AIApi_Chat_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode /* = NULL */)
{
	AIApi_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapAIClient.find(xhToken);
	if (stl_MapIterator == stl_MapAIClient.end())
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_CHAT_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	if (!APIClient_Http_GetResult(xhToken, pbComplete, pInt_HTTPCode))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		st_Locker.unlock_shared();
		return false;
	}

	st_Locker.unlock_shared();
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
bool CAIApi_Chat::AIApi_Chat_Parse(AICLIENT_CHAT* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bSSEReply)
{
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMSGBuffer, lpszMSGBuffer + nMSGLen, &st_JsonRoot, &st_JsonError))
	{
		return false;
	}
	Json::Value st_JsonChoices = st_JsonRoot["choices"];
	if (st_JsonChoices.isNull())
	{
		return false;
	}

	for (unsigned int i = 0; i < st_JsonChoices.size(); i++)
	{
		Json::Value st_JsonMessage;
		if (bSSEReply)
		{
			st_JsonMessage = st_JsonChoices[i]["delta"];
			if (st_JsonMessage.isNull())
			{
				continue;
			}
		}
		else
		{
			st_JsonMessage = st_JsonChoices[i]["message"];
			if (st_JsonMessage.isNull())
			{
				continue;
			}
		}
#ifdef _MSC_BUILD
		int nGBKLen = st_JsonMessage["content"].asString().length();
		XCHAR tszGBKBuffer[8192] = {};
		BaseLib_Charset_UTFToAnsi(st_JsonMessage["content"].asString().c_str(), tszGBKBuffer, &nGBKLen);
		pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_JsonRoot["model"].asCString(), tszGBKBuffer, nGBKLen, pSt_AIClient->lParam);
#else
		pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_JsonRoot["model"].asCString(), st_JsonMessage["content"].asString().c_str(), st_JsonMessage["content"].asString().length(), pSt_AIClient->lParam);
#endif
	}
	
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      回调函数
//////////////////////////////////////////////////////////////////////////
void CAIApi_Chat::AIApi_Chat_CBRecv(XNETHANDLE xhToken, XPVOID lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	AICLIENT_CHAT* pSt_AIClient = (AICLIENT_CHAT*)lParam;
	CAIApi_Chat* pClass_This = (CAIApi_Chat*)pSt_AIClient->lClass;

	int nPos = 0;
	bool bSSEReply = false;
	LPCXSTR lpszSSEStr = _X("data: ");
	if (0 == _tcsxnicmp((LPCXSTR)lpszMsgBuffer, lpszSSEStr, _tcsxlen(lpszSSEStr)))
	{
		nPos = _tcsxlen(lpszSSEStr);
		bSSEReply = true;
	}
	pClass_This->AIApi_Chat_Parse(pSt_AIClient, (LPCXSTR)lpszMsgBuffer + nPos, nMsgLen - nPos, bSSEReply);
}