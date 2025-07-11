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
函数功能：AI图像处理模型初始化
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
函数名称：AIApi_Image_ExcuteParse
函数功能：处理图像理解
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
 参数.三：lpszUrlBase
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入URL或者BASE64编码数据
 参数.四：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入怎么处理图像数据
 参数.五：nMSGLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入lpszMSGBuffer大小
 参数.六：bStream
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否使用流式数据回复
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Image::AIApi_Image_ExcuteParse(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszUrlBase, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream /* = false */)
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
函数名称：AIApi_Image_ExcuteCrete
函数功能：AI创建图像
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
 参数.三：lpszMSGSize
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入创建的分辨率1024x1024.需要根据模型来设置
 参数.四：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：描述要创建的什么样的图像
 参数.五：nMSGLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入lpszMSGBuffer大小
 参数.六：pptszMSGBuffer
  In/Out：Out
  类型：二级指针
  可空：Y
  意思：输出创建后的图像数据,此内存需要用户释放,如果bCallback为真,那么此参数不起作用
 参数.七：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出图像数据大小,如果bCallback为真,那么此参数不起作用
 参数.八：bCallback
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：输入图像输出方法,可以为回调或者直接输出
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Image::AIApi_Image_ExcuteCrete(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGSize, LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR** pptszMSGBuffer, int* pInt_MSGLen, bool bCallback /* = false */)
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
	Json::StreamWriterBuilder st_JsonBuilder;

	st_JsonBuilder["emitUTF8"] = true;
#ifdef _MSC_BUILD
	int nGBKLen = nMSGLen;
	XCHAR tszGBKBuffer[8192] = {};
	BaseLib_Charset_AnsiToUTF(lpszMSGBuffer, tszGBKBuffer, &nGBKLen);
	st_JsonRoot["prompt"] = tszGBKBuffer;
#else
	st_JsonRoot["prompt"] = lpszMSGBuffer;
#endif
	st_JsonRoot["size"] = lpszMSGSize;
	st_JsonRoot["model"] = lpszModelName;
	st_JsonRoot["watermark"] = false;
	st_JsonRoot["response_format"] = _X("b64_json");

	stl_MapIterator->second->bCreate = true;
	xstring m_StrBody = Json::writeString(st_JsonBuilder, st_JsonRoot);

	if (bCallback)
	{
		if (!APIClient_Http_Excute(xhToken, m_StrBody.c_str(), m_StrBody.length(), stl_MapIterator->second->tszAPIHdr))
		{
			AIApi_IsErrorOccur = true;
			AIApi_dwErrorCode = APIClient_GetLastError();
			st_Locker.unlock_shared();
			return false;
		}
	}
	else
	{
		int nHTTPCode = 0;
		int nHTTPSize = 0;
		XCHAR* ptszMSGBuffer = NULL;
		XCLIENT_APIHTTP st_APIHttp = {};
		st_APIHttp.nTimeConnect = 20000;
		st_APIHttp.nTimeOperator = 20000;

		if (!APIClient_Http_Request(_X("POST"), stl_MapIterator->second->tszAPIUrl, m_StrBody.c_str(), &nHTTPCode, &ptszMSGBuffer, &nHTTPSize, stl_MapIterator->second->tszAPIHdr, NULL, &st_APIHttp))
		{
			AIApi_IsErrorOccur = true;
			AIApi_dwErrorCode = APIClient_GetLastError();
			st_Locker.unlock_shared();
			return false;
		}
		*pptszMSGBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_LARGE);
		if (NULL == *pptszMSGBuffer)
		{
			AIApi_IsErrorOccur = true;
			AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_IMAGE_MALLOC;
			st_Locker.unlock_shared();
			return false;
		}
		memset(*pptszMSGBuffer, '\0', XENGINE_MEMORY_SIZE_LARGE);

		if (!AIApi_Image_JsonCreate(stl_MapIterator->second, ptszMSGBuffer, nHTTPSize, *pptszMSGBuffer, pInt_MSGLen))
		{
			free(ptszMSGBuffer);
			ptszMSGBuffer = NULL;
			st_Locker.unlock_shared();
			return false;
		}
		free(ptszMSGBuffer);
		ptszMSGBuffer = NULL;
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
bool CAIApi_Image::AIApi_Image_JsonParse(AICLIENT_IMAGE* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bSSEReply)
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
bool CAIApi_Image::AIApi_Image_JsonCreate(AICLIENT_IMAGE* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	AIApi_IsErrorOccur = false;

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMSGBuffer, lpszMSGBuffer + nMSGLen, &st_JsonRoot, &st_JsonError))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_IMAGE_PARSE;
		return false;
	}
	if (st_JsonRoot["created"].isNull())
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_IMAGE_FORMAT;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["data"];
	for (unsigned int i = 0; i < st_JsonObject.size(); i++)
	{
		if (!st_JsonObject[i]["b64_json"].isNull())
		{
			if (NULL == ptszMSGBuffer)
			{
				pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_JsonRoot["model"].asCString(), st_JsonObject[i]["b64_json"].asCString(), st_JsonObject[i]["b64_json"].asString().length(), false, pSt_AIClient->lParam);
			}
			else
			{
				*pInt_MSGLen = st_JsonObject[i]["b64_json"].asString().length();
				memcpy(ptszMSGBuffer, st_JsonObject[i]["b64_json"].asCString(), *pInt_MSGLen);
			}
		}
		if (!st_JsonObject[i]["url"].isNull())
		{
			if (NULL == ptszMSGBuffer)
			{
				pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_JsonRoot["model"].asCString(), st_JsonObject[i]["url"].asCString(), st_JsonObject[i]["url"].asString().length(), false, pSt_AIClient->lParam);
			}
			else
			{
				*pInt_MSGLen = st_JsonObject[i]["b64_json"].asString().length();
				memcpy(ptszMSGBuffer, st_JsonObject[i]["url"].asCString(), *pInt_MSGLen);
			}
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
			if (!pClass_This->AIApi_Image_JsonParse(pSt_AIClient, ptszStart + nPos, nOneMsgLen - nPos, true))
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
		if (pSt_AIClient->bCreate)
		{
			if (pClass_This->AIApi_Image_JsonParse(pSt_AIClient, (LPCXSTR)pSt_AIClient->ptszMSGBuffer, pSt_AIClient->nMSGLen, false))
			{
				memset(pSt_AIClient->ptszMSGBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
				pSt_AIClient->nMSGLen = 0;
			}
		}
		else
		{
			if (pClass_This->AIApi_Image_JsonCreate(pSt_AIClient, (LPCXSTR)pSt_AIClient->ptszMSGBuffer, pSt_AIClient->nMSGLen))
			{
				memset(pSt_AIClient->ptszMSGBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
				pSt_AIClient->nMSGLen = 0;
			}
		}
	}
}