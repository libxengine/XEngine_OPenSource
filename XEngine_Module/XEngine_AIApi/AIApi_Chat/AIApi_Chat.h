#pragma once
/********************************************************************
//    Created:     2025/05/29  13:56:52
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Chat\AIApi_Chat.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Chat
//    File Base:   AIApi_Chat
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     AI对话函数接口
//    History:
*********************************************************************/
typedef struct
{
	int nCLen;
	XCHAR tszThinkContent[8192];
	XCHAR tszRoleContent[8192];
	XCHAR tszRoleName[128];
}AICLIENT_HISTORY;
typedef struct
{
	AICLIENT_HISTORY st_HisStream;       //流式有用
	XCHAR tszAPIUrl[XPATH_MAX];
	XCHAR tszAPIHdr[XPATH_MAX];
	XCHAR tszAPIRole[XPATH_MAX];
	XNETHANDLE xhToken;
	time_t nTimeStart;
	int nMSGLen;
	
	bool bHistory;
	XENGINE_MODULE_AIAPI_VERSION enAPIVersion; // 新增：保存当前句柄使用的API版本
	std::unique_ptr<std::list<AICLIENT_HISTORY>> pStl_ListHistory;

	XCHAR* ptszMSGBuffer;
	XPVOID lClass;
	XPVOID lParam;
	CALLBACK_XENGINE_MODULE_AIAPI_CHAT lpCall_Chat;
}AICLIENT_CHAT;

class CAIApi_Chat
{
public:
	CAIApi_Chat();
	~CAIApi_Chat();
public:
	bool AIApi_Chat_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam = NULL, bool bHistory = true, XENGINE_MODULE_AIAPI_VERSION enAPIVersion = XENGINE_MODULE_AIAPI_VERSION_V1);
	bool AIApi_Chat_Excute(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bThink = false, int nMaxToken = 0);
	bool AIApi_Chat_SetRole(XNETHANDLE xhToken, LPCXSTR lpszRoleName);
	bool AIApi_Chat_Clear(XNETHANDLE xhToken);
	bool AIApi_Chat_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode = NULL, bool bWaitExist = true);
	bool AIApi_Chat_Destory(XNETHANDLE xhToken);
protected:
	bool AIApi_Chat_Parse(AICLIENT_CHAT* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bThink);
	// V3 版本解析封装函数
	bool AIApi_Chat_ParseV3(AICLIENT_CHAT* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bThink);
protected:
	static void XCALLBACK AIApi_Chat_CBRecv(XNETHANDLE xhToken, XPVOID lpszMsgBuffer, int nMsgLen, XPVOID lParam);
	void AIApi_Chat_CBRecvV1(AICLIENT_CHAT* pSt_AIClient);
	void AIApi_Chat_CBRecvV3(AICLIENT_CHAT* pSt_AIClient);
private:
	std::shared_mutex st_Locker;
private:
	std::unordered_map<XNETHANDLE, AICLIENT_CHAT*> stl_MapAIClient;
};