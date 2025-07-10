#include "pch.h"
#include "AIApi_Image.h"
/********************************************************************
//    Created:     2025/07/10  14:08:06
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Image\AIApi_Image.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Image
//    File Base:   AIApi_Image
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     图像生成理解
//    History:
*********************************************************************/
CAIApi_Image::CAIApi_Image()
{
}
CAIApi_Image::~CAIApi_Image()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AIApi_Image_Create
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
 参数.六：bHistory
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用历史会话功能
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Image::AIApi_Image_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam /* = NULL */)
{
	AIApi_IsErrorOccur = true;

	if (NULL == pxhToken || NULL == lpszAPIUrl || NULL == lpszAPIKey)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_IMAGE_PARAMENT;
		return false;
	}
	AICLIENT_IMAGE *pSt_AIClient = new AICLIENT_IMAGE;
	if (NULL == pSt_AIClient)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_IMAGE_MALLOC;
		return false;
	}
	*pSt_AIClient = {};
	pSt_AIClient->lParam = lParam;
	pSt_AIClient->lpCall_Chat = fpCall_Chat;

	pSt_AIClient->nTimeStart = time(NULL);
	_xstrcpy(pSt_AIClient->tszAPIUrl, lpszAPIUrl, sizeof(pSt_AIClient->tszAPIUrl));
	_xsntprintf(pSt_AIClient->tszAPIHdr, sizeof(pSt_AIClient->tszAPIHdr), _X("Content-Type: application/json\r\nAuthorization: Bearer %s"), lpszAPIKey);

	pSt_AIClient->ptszMSGBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_LARGE);
	if (NULL == pSt_AIClient->ptszMSGBuffer)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_IMAGE_MALLOC;
		return false;
	}
	memset(pSt_AIClient->ptszMSGBuffer, '\0', XENGINE_MEMORY_SIZE_LARGE);

	if (!APIClient_Http_Create(&pSt_AIClient->xhToken, AIApi_Image_CBRecv, pSt_AIClient))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	XCLIENT_APIHTTP st_HTTPParam = {};
	st_HTTPParam.nTimeConnect = 60000;
	st_HTTPParam.nTimeOperator = 60000;
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
函数名称：AIApi_Image_Excute
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
bool CAIApi_Image::AIApi_Image_Excute(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszUrlBase, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream /* = false */)
{
	AIApi_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapAIClient.find(xhToken);
	if (stl_MapIterator == stl_MapAIClient.end())
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_IMAGE_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::StreamWriterBuilder st_JsonBuilder;

	st_JsonBuilder["emitUTF8"] = true;
	stl_MapIterator->second->bStream = bStream;
	//准备数据
	Json::Value st_JsonObject;
	Json::Value st_JsonContext;
	Json::Value st_JsonContextText;
	Json::Value st_JsonContextUrl;
	Json::Value st_JsonContextUrlPayload;
#ifdef _MSC_BUILD
	int nGBKLen = nMSGLen;
	XCHAR tszGBKBuffer[8192] = {};
	BaseLib_Charset_AnsiToUTF(lpszMSGBuffer, tszGBKBuffer, &nGBKLen);
	st_JsonContextText["text"] = tszGBKBuffer;
#else
	st_JsonContextText["text"] = lpszMSGBuffer;
#endif
	st_JsonContextText["type"] = _X("text");

	st_JsonContextUrlPayload["url"] = lpszUrlBase;
	st_JsonContextUrl["type"] = _X("image_url");
	st_JsonContextUrl["image_url"] = st_JsonContextUrlPayload;
	//发送的内容
	st_JsonObject.append(st_JsonContextText);
	st_JsonObject.append(st_JsonContextUrl);
	st_JsonContext["content"] = st_JsonObject;
	st_JsonContext["role"] = "user";

	st_JsonArray.append(st_JsonContext);
	
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
函数名称：AIApi_Image_GetStatus
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
 参数.三：pInt_HTTPCode
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出HTTP状态码
 参数.四：bWaitExist
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否等待执行完毕
返回值
  类型：逻辑型
  意思：是否成功
备注：pbComplete没有执行完毕需要等待执行完毕
*********************************************************************/
bool CAIApi_Image::AIApi_Image_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode /* = NULL */, bool bWaitExist /* = true */)
{
	AIApi_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapAIClient.find(xhToken);
	if (stl_MapIterator == stl_MapAIClient.end())
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_IMAGE_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}

	if (!APIClient_Http_GetResult(xhToken, pbComplete, pInt_HTTPCode, bWaitExist))
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
函数名称：AIApi_Image_Destory
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
bool CAIApi_Image::AIApi_Image_Destory(XNETHANDLE xhToken)
{
	AIApi_IsErrorOccur = false;

	st_Locker.lock();
	auto stl_MapIterator = stl_MapAIClient.find(xhToken);
	if (stl_MapIterator != stl_MapAIClient.end())
	{
		APIClient_Http_Close(stl_MapIterator->second->xhToken);

		free(stl_MapIterator->second->ptszMSGBuffer);
		stl_MapIterator->second->ptszMSGBuffer = NULL;

		delete stl_MapIterator->second;
		stl_MapIterator->second = NULL;
		stl_MapAIClient.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
bool CAIApi_Image::AIApi_Image_Parse(AICLIENT_IMAGE* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bSSEReply)
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
			if (st_JsonMessage["content"].isNull() || st_JsonMessage["role"].isNull())
			{
				continue;
			}
		}
		//某些回复没有内容
		if (st_JsonMessage["content"].asString().length() > 0)
		{
#ifdef _MSC_BUILD
			int nGBKLen = st_JsonMessage["content"].asString().length();
			XCHAR tszGBKBuffer[8192] = {};
			BaseLib_Charset_UTFToAnsi(st_JsonMessage["content"].asString().c_str(), tszGBKBuffer, &nGBKLen);
			pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_JsonRoot["model"].asCString(), tszGBKBuffer, nGBKLen, false, pSt_AIClient->lParam);
#else
			pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_JsonRoot["model"].asCString(), st_JsonMessage["content"].asString().c_str(), st_JsonMessage["content"].asString().length(), false, pSt_AIClient->lParam);
#endif
		}
	}
	
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      回调函数
//////////////////////////////////////////////////////////////////////////
void CAIApi_Image::AIApi_Image_CBRecv(XNETHANDLE xhToken, XPVOID lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	AICLIENT_IMAGE* pSt_AIClient = (AICLIENT_IMAGE*)lParam;
	CAIApi_Image* pClass_This = (CAIApi_Image*)pSt_AIClient->lClass;

	if (pSt_AIClient->nMSGLen + nMsgLen > XENGINE_MEMORY_SIZE_LARGE)
	{
		//清空旧数据并重置
		pSt_AIClient->nMSGLen = 0;
	}
	memcpy(pSt_AIClient->ptszMSGBuffer + pSt_AIClient->nMSGLen, lpszMsgBuffer, nMsgLen);
	pSt_AIClient->nMSGLen += nMsgLen;
	if (pSt_AIClient->bStream)
	{
		//SSE
		int nPos = 0;
		LPCXSTR lpszSSEStr = _X("data: ");
		LPCXSTR lpszSSEEnd = _X("[DONE]");

		XCHAR* ptszStart = pSt_AIClient->ptszMSGBuffer;
		XCHAR* ptszEnd = ptszStart + pSt_AIClient->nMSGLen;
		while (true)
		{
			// 查找是否存在完整的一个消息（由 \n\n 分隔）
			XCHAR* ptszSplit = _tcsxstr(ptszStart, _X("\n\n"));
			if (NULL == ptszSplit)
			{
				// 不完整，等待更多数据
				break;
			}
			// 得到一个完整消息的结束位置（包含 \r\n\r\n 的结尾）
			XCHAR* ptszNextMsg = ptszSplit + 2;  // Skip "\n\n"
			int nOneMsgLen = (int)(ptszSplit - ptszStart); // 不含 "\n\n"
			// 检查是否是 data: 开头的 SSE 消息
			int nPos = 0;
			if (_tcsxnicmp(ptszStart, lpszSSEStr, _tcsxlen(lpszSSEStr)) == 0)
			{
				nPos = _tcsxlen(lpszSSEStr);
			}
			if (_tcsxnicmp(lpszSSEEnd, ptszStart + nPos, _tcsxlen(lpszSSEEnd)) == 0)
			{
				//结束
				pSt_AIClient->nMSGLen = 0;
				memset(pSt_AIClient->ptszMSGBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
				break;
			}
			// 解析当前消息体
			if (!pClass_This->AIApi_Image_Parse(pSt_AIClient, ptszStart + nPos, nOneMsgLen - nPos, true))
			{
				break; 
			}
			// 计算剩余数据长度
			int nRemainLen = (int)(ptszEnd - ptszNextMsg);
			if (nRemainLen > 0)
			{
				memmove(pSt_AIClient->ptszMSGBuffer, ptszNextMsg, nRemainLen * sizeof(XCHAR));
			}
			// 更新指针和长度
			pSt_AIClient->nMSGLen = nRemainLen;
			memset(pSt_AIClient->ptszMSGBuffer + pSt_AIClient->nMSGLen, '\0', XENGINE_MEMORY_SIZE_MAX - pSt_AIClient->nMSGLen);

			ptszStart = pSt_AIClient->ptszMSGBuffer;
			ptszEnd = ptszStart + nRemainLen;
		}
	}
	else
	{
		if (pClass_This->AIApi_Image_Parse(pSt_AIClient, (LPCXSTR)pSt_AIClient->ptszMSGBuffer, pSt_AIClient->nMSGLen, false))
		{
			memset(pSt_AIClient->ptszMSGBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
			pSt_AIClient->nMSGLen = 0;
		}
	}
}