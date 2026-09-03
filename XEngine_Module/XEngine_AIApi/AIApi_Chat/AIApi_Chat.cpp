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
 参数.六：bHistory
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用历史会话功能
 参数.七：enAPIVersion
  In/Out：In
  类型：枚举型
  可空：Y
  意思：API版本
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Chat::AIApi_Chat_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam /* = NULL */, bool bHistory /* = true */, XENGINE_MODULE_AIAPI_VERSION enAPIVersion /* = XENGINE_MODULE_AIAPI_VERSION_V1 */)
{
	AIApi_IsErrorOccur = true;

	if (NULL == pxhToken || NULL == lpszAPIUrl || NULL == lpszAPIKey)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_CHAT_PARAMENT;
		return false;
	}
	AICLIENT_CHAT *pSt_AIClient = new (std::nothrow) AICLIENT_CHAT;
	if (NULL == pSt_AIClient)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_CHAT_MALLOC;
		return false;
	}
	*pSt_AIClient = {};
	pSt_AIClient->bHistory = bHistory;
	pSt_AIClient->lParam = lParam;
	pSt_AIClient->lpCall_Chat = fpCall_Chat;
	pSt_AIClient->enAPIVersion = enAPIVersion; // 新增：保存接口版本类型
	pSt_AIClient->lClass = this; 

	pSt_AIClient->nTimeStart = time(NULL);
	_xstrcpy(pSt_AIClient->tszAPIUrl, lpszAPIUrl, sizeof(pSt_AIClient->tszAPIUrl));
	_xsntprintf(pSt_AIClient->tszAPIHdr, sizeof(pSt_AIClient->tszAPIHdr), _X("Content-Type: application/json\r\nAuthorization: Bearer %s"), lpszAPIKey);

	pSt_AIClient->pStl_ListHistory.reset(new (std::nothrow) std::list<AICLIENT_HISTORY>());
	if (NULL == pSt_AIClient->pStl_ListHistory)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_CHAT_MALLOC;
		delete pSt_AIClient;
		return false;
	}
	pSt_AIClient->ptszMSGBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	if (NULL == pSt_AIClient->ptszMSGBuffer)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_CHAT_MALLOC;
		delete pSt_AIClient;
		return false;
	}
	memset(pSt_AIClient->ptszMSGBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);

	if (!APIClient_Http_Create(&pSt_AIClient->xhToken, AIApi_Chat_CBRecv, pSt_AIClient))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		free(pSt_AIClient->ptszMSGBuffer);
		delete pSt_AIClient;
		return false;
	}
	XCLIENT_APIHTTP st_HTTPParam = {};
	st_HTTPParam.nTimeConnect = 60000;
	st_HTTPParam.nTimeOperator = 60000;
	if (!APIClient_Http_SetParam(pSt_AIClient->xhToken, &st_HTTPParam))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		APIClient_Http_Close(pSt_AIClient->xhToken);
		free(pSt_AIClient->ptszMSGBuffer);
		delete pSt_AIClient;
		return false;
	}
	if (!APIClient_Http_SetUrl(pSt_AIClient->xhToken, lpszAPIUrl, _X("POST")))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		APIClient_Http_Close(pSt_AIClient->xhToken);
		free(pSt_AIClient->ptszMSGBuffer);
		delete pSt_AIClient;
		return false;
	}
	*pxhToken = pSt_AIClient->xhToken;

	st_Locker.lock();
	stl_MapAIClient.insert(std::make_pair(*pxhToken, pSt_AIClient));
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
 参数.五：nMaxToken
  In/Out：In
  类型：整数型
  可空：Y
  意思：允许的最大token个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Chat::AIApi_Chat_Excute(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bThink /* = false */, int nMaxToken /* = 0 */)
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
	Json::StreamWriterBuilder st_JsonBuilder;

	st_JsonBuilder["emitUTF8"] = true;
	//主要帮助角色信息
	if (_tcsxlen(stl_MapIterator->second->tszAPIRole) > 0)
	{
		Json::Value st_JsonSystemRole;
		st_JsonSystemRole["role"] = _X("system");
		st_JsonSystemRole["content"] = stl_MapIterator->second->tszAPIRole;

		st_JsonArray.append(st_JsonSystemRole);
	}
	//准备数据
	Json::Value st_JsonObject;
	AICLIENT_HISTORY st_AIHistory = {};
	_xstrcpy(st_AIHistory.tszRoleName, _X("user"), sizeof(st_AIHistory.tszRoleName));
#ifdef _MSC_BUILD
	BaseLib_Charset_AnsiToUTF(lpszMSGBuffer, st_AIHistory.tszRoleContent, &nMSGLen);
#else
	_xstrcpy(st_AIHistory.tszRoleContent, lpszMSGBuffer, sizeof(st_AIHistory.tszRoleContent));
#endif
	//是否启用了历史会话
	if (stl_MapIterator->second->bHistory)
	{
		for (const auto& stl_ListHistory : *(stl_MapIterator->second->pStl_ListHistory))
		{
			Json::Value st_JsonHistory;
			st_JsonHistory["role"] = stl_ListHistory.tszRoleName;
			st_JsonHistory["content"] = stl_ListHistory.tszRoleContent;
			st_JsonArray.append(st_JsonHistory);
		}
		//需要加入对话列表
		stl_MapIterator->second->pStl_ListHistory->push_back(st_AIHistory);
	}
	//发送的内容
	st_JsonObject["content"] = st_AIHistory.tszRoleContent;
	st_JsonObject["role"] = st_AIHistory.tszRoleName;
	st_JsonArray.append(st_JsonObject);
	
	st_JsonRoot["model"] = lpszModelName;
	st_JsonRoot["stream"] = true;
	if (nMaxToken > 0)
	{
		st_JsonRoot["max_tokens"] = nMaxToken;
	}
	if (stl_MapIterator->second->enAPIVersion == XENGINE_MODULE_AIAPI_VERSION_V3)
	{
		// 明确告诉 V3 引擎思考模式
		Json::Value st_JsonThinking;
		if (bThink)
		{
			st_JsonThinking["type"] = "enable";
		}
		else
		{
			st_JsonThinking["type"] = "disabled";
		}
		st_JsonRoot["thinking"] = st_JsonThinking;

		st_JsonRoot["input"] = st_JsonArray;
	}
	else
	{
		st_JsonRoot["messages"] = st_JsonArray;
	}

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
函数名称：AIApi_Chat_SetRole
函数功能：设置系统角色
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszModelName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入系统角色信息,比如:你是一个C/C++语言的开发大师
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Chat::AIApi_Chat_SetRole(XNETHANDLE xhToken, LPCXSTR lpszRoleName)
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
#ifdef _MSC_BUILD
	int nLen = _tcsxlen(lpszRoleName);
	XCHAR tszUTFBuffer[8192] = {};
	BaseLib_Charset_AnsiToUTF(lpszRoleName, stl_MapIterator->second->tszAPIRole, &nLen);
#else
	_xstrcpy(stl_MapIterator->second->tszAPIRole, lpszRoleName, sizeof(stl_MapIterator->second->tszAPIRole));
#endif
	
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：AIApi_Chat_Clear
函数功能：清理对话历史记录
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Chat::AIApi_Chat_Clear(XNETHANDLE xhToken)
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
	stl_MapIterator->second->pStl_ListHistory->clear();
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
bool CAIApi_Chat::AIApi_Chat_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode /* = NULL */, bool bWaitExist /* = true */)
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
	int nHTTPCode = 0;
	if (!APIClient_Http_GetResult(xhToken, pbComplete, &nHTTPCode, bWaitExist))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = APIClient_GetLastError();
		st_Locker.unlock_shared();
		return false;
	}
	st_Locker.unlock_shared();

	if (200 != nHTTPCode)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_CHAT_HTTPCODE;
		return false;
	}
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
bool CAIApi_Chat::AIApi_Chat_Parse(AICLIENT_CHAT* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bThink)
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
		Json::Value st_JsonErrorInfo = st_JsonRoot["error"];
		if (!st_JsonErrorInfo.isNull())
		{
			pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_JsonErrorInfo["message"].asCString(), st_JsonErrorInfo["message"].size(), false, pSt_AIClient->lParam);
		}
		return false;
	}

	for (unsigned int i = 0; i < st_JsonChoices.size(); i++)
	{
		Json::Value st_JsonMessage;
		st_JsonMessage = st_JsonChoices[i]["delta"];
		if (st_JsonMessage.isNull())
		{
			continue;
		}
		AICLIENT_HISTORY st_AIHistory = {};
		
		if (!st_JsonMessage["role"].isNull())
		{
			_xstrcpy(st_AIHistory.tszRoleName, st_JsonMessage["role"].asCString(), sizeof(st_AIHistory.tszRoleName));
		}
		int nGBKLen = 0;

		if (st_JsonMessage["reasoning_content"].isNull())
		{
			//为空,表示对话
			nGBKLen = st_JsonMessage["content"].asString().length();
			_xstrcpy(st_AIHistory.tszRoleContent, st_JsonMessage["content"].asString().c_str(), sizeof(st_AIHistory.tszRoleContent));
		}
		else
		{
			//不为空,表示思考模型
			nGBKLen = st_JsonMessage["reasoning_content"].asString().length();
			_xstrcpy(st_AIHistory.tszThinkContent, st_JsonMessage["reasoning_content"].asString().c_str(), sizeof(st_AIHistory.tszThinkContent));
		}
		//某些回复没有内容
		if (_tcsxlen(st_AIHistory.tszRoleContent) > 0)
		{
#ifdef _MSC_BUILD
			BaseLib_Charset_UTFToAnsi(st_JsonMessage["content"].asString().c_str(), st_AIHistory.tszRoleContent, &nGBKLen);
			pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_AIHistory.tszRoleContent, nGBKLen, false, pSt_AIClient->lParam);
#else
			pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_AIHistory.tszRoleContent, nGBKLen, false, pSt_AIClient->lParam);
#endif
			//流式数据需要单独处理保存
			memcpy(pSt_AIClient->st_HisStream.tszRoleContent + pSt_AIClient->st_HisStream.nCLen, st_AIHistory.tszRoleContent, nGBKLen);
			pSt_AIClient->st_HisStream.nCLen += nGBKLen;
		}
		//思考对话
		if (_tcsxlen(st_AIHistory.tszThinkContent) > 0)
		{
#ifdef _MSC_BUILD
			BaseLib_Charset_UTFToAnsi(st_JsonMessage["reasoning_content"].asString().c_str(), st_AIHistory.tszThinkContent, &nGBKLen);
			pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_AIHistory.tszThinkContent, nGBKLen, true, pSt_AIClient->lParam);
#else
			pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_AIHistory.tszThinkContent, nGBKLen, true, pSt_AIClient->lParam);
#endif
		}
	}
	
	return true;
}
bool CAIApi_Chat::AIApi_Chat_ParseV3(AICLIENT_CHAT* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bThink)
{
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMSGBuffer, lpszMSGBuffer + nMSGLen, &st_JsonRoot, &st_JsonError))
	{
		return false;
	}
	
	AICLIENT_HISTORY st_AIHistory = {};
	if (!st_JsonRoot["delta"].isNull())
	{
		int nGBKLen = st_JsonRoot["delta"].size();
#ifdef _MSC_BUILD
		BaseLib_Charset_UTFToAnsi(st_JsonRoot["delta"].asCString(), st_AIHistory.tszRoleContent, &nGBKLen);
		pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_AIHistory.tszRoleContent, nGBKLen, bThink, pSt_AIClient->lParam);
#else
		pSt_AIClient->lpCall_Chat(pSt_AIClient->xhToken, st_AIHistory.tszRoleContent, nGBKLen, bThink, pSt_AIClient->lParam);
#endif
		if (!bThink)
		{
			memcpy(pSt_AIClient->st_HisStream.tszRoleContent + pSt_AIClient->st_HisStream.nCLen, st_AIHistory.tszRoleContent, nGBKLen);
			pSt_AIClient->st_HisStream.nCLen += nGBKLen;
		}
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

	// 1. 底层通用逻辑：内存边界检查
	if (pSt_AIClient->nMSGLen + nMsgLen > XENGINE_MEMORY_SIZE_MAX)
	{
		pSt_AIClient->nMSGLen = 0;
	}
	// 2. 底层通用逻辑：追加数据到包缓冲区
	memcpy(pSt_AIClient->ptszMSGBuffer + pSt_AIClient->nMSGLen, lpszMsgBuffer, nMsgLen);
	pSt_AIClient->nMSGLen += nMsgLen;

	// 3. 纵向分流：根据创建时的接口版本，调用不同的核心接收解析状态机
	if (pSt_AIClient->enAPIVersion == XENGINE_MODULE_AIAPI_VERSION_V3)
	{
		pClass_This->AIApi_Chat_CBRecvV3(pSt_AIClient);
	}
	else
	{
		pClass_This->AIApi_Chat_CBRecvV1(pSt_AIClient);
	}
}
void CAIApi_Chat::AIApi_Chat_CBRecvV1(AICLIENT_CHAT* pSt_AIClient)
{
	//SSE
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
			if (pSt_AIClient->bHistory)
			{
				_xstrcpy(pSt_AIClient->st_HisStream.tszRoleName, _X("assistant"), sizeof(pSt_AIClient->st_HisStream.tszRoleName));
				pSt_AIClient->pStl_ListHistory->push_back(pSt_AIClient->st_HisStream);
				memset(&pSt_AIClient->st_HisStream, '\0', sizeof(AICLIENT_HISTORY));
			}
			pSt_AIClient->nMSGLen = 0;
			memset(pSt_AIClient->ptszMSGBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
			break;
		}
		// 解析当前消息体
		if (!AIApi_Chat_Parse(pSt_AIClient, ptszStart + nPos, nOneMsgLen - nPos, true))
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
void CAIApi_Chat::AIApi_Chat_CBRecvV3(AICLIENT_CHAT* pSt_AIClient)
{
	// V3 流式处理（此处可按 V3 独有的协议帧如 event-stream 变体或新定义终结符扩展）
	LPCXSTR lpszSSEEvent = _X("event: ");
	LPCXSTR lpszSSEStr = _X("data: ");
	LPCXSTR lpszSSEEnd = _X("[DONE]");
	//得到EVENT
	XCHAR* ptszBuf = pSt_AIClient->ptszMSGBuffer;
	int& nLen = pSt_AIClient->nMSGLen;

	std::vector<xstring> stl_VectorFields;
	std::string_view m_StrField(ptszBuf, nLen);
	size_t nStart = 0;
	size_t nEnd = m_StrField.find("\n\n");
	while (nEnd != std::string::npos)
	{
		// 截取从 start 开始，长度为 (end - start) 的子串
		stl_VectorFields.push_back(xstring(m_StrField.substr(nStart, nEnd - nStart)));
		// 将下一次查找的起点移到当前分隔符的后面
		nStart = nEnd + 2;  // Skip "\n\n"
		// 寻找下一个 \n\n
		nEnd = m_StrField.find("\n\n", nStart);
	}
	// --- 核心改动：平移残余数据并更新大小 ---
	if (nStart > 0)
	{
		if (nStart < nLen)
		{
			// 情况 A：有残余数据（数据不全的尾巴）
			size_t nRemainLen = nLen - nStart; // 计算剩下的不完整数据长度
			// 使用 memmove 将后面的残余数据平移到缓冲区最前端
			// 注意：因为源内存和目标内存可能重叠，必须用 memmove，不能用 memcpy
			::memmove(ptszBuf, ptszBuf + nStart, nRemainLen);
			// 更新缓冲区中剩余的有效数据大小
			nLen = nRemainLen;
			// (可选) 如果是字符串缓冲区，习惯性在截断处补一个 \0 保证安全
			ptszBuf[nLen] = '\0';
		}
		else
		{
			// 情况 B：数据刚好完美解析完，没有残留
			nLen = 0;
			if (ptszBuf != nullptr)
			{
				ptszBuf[0] = '\0';
			}
		}
	}
	// 别忘了把最后一个分隔符后面的尾巴（第三段）带上
	if (nStart < m_StrField.length())
	{
		stl_VectorFields.push_back(xstring(m_StrField.substr(nStart, m_StrField.length() - nStart)));
	}

	for (const auto& stl_VectorIterator : stl_VectorFields)
	{
		xstring m_StrEvent;
		xstring m_StrData;
		xstring m_StrMessage;
		std::stringstream m_StrStream(stl_VectorIterator);
		while (std::getline(m_StrStream, m_StrMessage))
		{
			// 去除可能存在的行尾回车符（兼容 Windows 的 \r\n）
			if (!m_StrMessage.empty() && m_StrMessage.back() == '\r')
			{
				m_StrMessage.pop_back();
			}
			// 1. 解析 event 字段
			if (m_StrMessage.rfind(lpszSSEEvent, 0) == 0)
			{
				m_StrEvent = m_StrMessage.substr(7);
			}
			// 2. 解析 data 字段
			else if (m_StrMessage.rfind(lpszSSEStr, 0) == 0)
			{
				m_StrData = m_StrMessage.substr(6);
			}

			if (m_StrData.rfind(lpszSSEEnd, 0) == 0)
			{
				if (pSt_AIClient->bHistory)
				{
					_xstrcpy(pSt_AIClient->st_HisStream.tszRoleName, _X("assistant"), sizeof(pSt_AIClient->st_HisStream.tszRoleName));
					pSt_AIClient->pStl_ListHistory->push_back(pSt_AIClient->st_HisStream);
					memset(&pSt_AIClient->st_HisStream, '\0', sizeof(AICLIENT_HISTORY));
				}
				pSt_AIClient->nMSGLen = 0;
				memset(pSt_AIClient->ptszMSGBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
				break;
			}
		}
		// 调用针对 V3 新封装的解析器
		LPCXSTR lpszEventThinkPrefix = _X("response.output_text.delta");
		LPCXSTR lpszEventDeltaPrefix = _X("response.reasoning_summary_text.delta");
		if (0 == _tcsxnicmp(m_StrEvent.c_str(), lpszEventThinkPrefix, m_StrEvent.length()))
		{
			if (!AIApi_Chat_ParseV3(pSt_AIClient, m_StrData.c_str(), m_StrData.length(), false))
			{
				break;
			}
		}
		else if (0 == _tcsxnicmp(m_StrEvent.c_str(), lpszEventDeltaPrefix, m_StrEvent.length()))
		{
			if (!AIApi_Chat_ParseV3(pSt_AIClient, m_StrData.c_str(), m_StrData.length(), true))
			{
				break;
			}
		}
	}
}