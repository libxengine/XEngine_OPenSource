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
	XCHAR tszRoleContent[8192];
	XCHAR tszRoleName[128];
}AICLIENT_HISTORY;
typedef struct
{
	XCHAR tszAPIUrl[XPATH_MAX];
	XCHAR tszAPIHdr[XPATH_MAX];
	XCHAR tszAPIRole[XPATH_MAX];
	XNETHANDLE xhToken;
	time_t nTimeStart;

	bool bStream;
	bool bHistory;
	std::unique_ptr<std::list<AICLIENT_HISTORY>> pStl_ListHistory;

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
	bool AIApi_Chat_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam = NULL, bool bHistory = true);
	bool AIApi_Chat_Excute(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream = false);
	bool AIApi_Chat_SetRole(XNETHANDLE xhToken, LPCXSTR lpszRoleName);
	bool AIApi_Chat_Clear(XNETHANDLE xhToken);
	bool AIApi_Chat_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode = NULL, bool bWaitExist = true);
	bool AIApi_Chat_Destory(XNETHANDLE xhToken);
protected:
	bool AIApi_Chat_Parse(AICLIENT_CHAT* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bSSEReply);
protected:
	static void XCALLBACK AIApi_Chat_CBRecv(XNETHANDLE xhToken, XPVOID lpszMsgBuffer, int nMsgLen, XPVOID lParam);
private:
	std::shared_mutex st_Locker;
private:
	std::unordered_map<XNETHANDLE, AICLIENT_CHAT*> stl_MapAIClient;
};