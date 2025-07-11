#pragma once
/********************************************************************
//    Created:     2025/07/11  10:03:00
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Vision\AIApi_Vision.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Vision
//    File Base:   AIApi_Vision
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     视觉处理类
//    History:
*********************************************************************/
typedef struct
{
	XCHAR tszAPIUrl[XPATH_MAX];
	XCHAR tszAPIHdr[XPATH_MAX];
	XNETHANDLE xhToken;
	time_t nTimeStart;
	int nMSGLen;
	
	bool bStream;
	bool bCreate;

	XCHAR* ptszMSGBuffer;
	XPVOID lClass;
	XPVOID lParam;
	CALLBACK_XENGINE_MODULE_AIAPI_CHAT lpCall_Chat;
}AICLIENT_VISION;

class CAIApi_Vision
{
public:
	CAIApi_Vision();
	~CAIApi_Vision();
public:
	bool AIApi_Vision_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam = NULL);
	bool AIApi_Vision_ExcuteParse(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszUrlBase, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream = false);
	bool AIApi_Vision_ExcuteCrete(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGSize, LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR** pptszMSGBuffer, int* pInt_MSGLen, bool bCallback = false);
	bool AIApi_Vision_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode = NULL, bool bWaitExist = true);
	bool AIApi_Vision_Destory(XNETHANDLE xhToken);
protected:
	bool AIApi_Vision_JsonParse(AICLIENT_VISION* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bSSEReply);
	bool AIApi_Vision_JsonCreate(AICLIENT_VISION* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR* ptszMSGBuffer = NULL, int* pInt_MSGLen = NULL);
protected:
	static void XCALLBACK AIApi_Vision_CBRecv(XNETHANDLE xhToken, XPVOID lpszMsgBuffer, int nMsgLen, XPVOID lParam);
private:
	std::shared_mutex st_Locker;
private:
	std::unordered_map<XNETHANDLE, AICLIENT_VISION*> stl_MapAIClient;
};