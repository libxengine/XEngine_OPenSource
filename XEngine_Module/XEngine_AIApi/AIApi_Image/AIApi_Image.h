#pragma once
/********************************************************************
//    Created:     2025/07/10  14:07:37
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Image\AIApi_Image.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Image
//    File Base:   AIApi_Image
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     图像生成理解
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

	XCHAR* ptszMSGBuffer;
	XPVOID lClass;
	XPVOID lParam;
	CALLBACK_XENGINE_MODULE_AIAPI_CHAT lpCall_Chat;
}AICLIENT_IMAGE;

class CAIApi_Image
{
public:
	CAIApi_Image();
	~CAIApi_Image();
public:
	bool AIApi_Image_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam = NULL);
	bool AIApi_Image_Excute(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszUrlBase, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream = false);
	bool AIApi_Image_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode = NULL, bool bWaitExist = true);
	bool AIApi_Image_Destory(XNETHANDLE xhToken);
protected:
	bool AIApi_Image_Parse(AICLIENT_IMAGE* pSt_AIClient, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bSSEReply);
protected:
	static void XCALLBACK AIApi_Image_CBRecv(XNETHANDLE xhToken, XPVOID lpszMsgBuffer, int nMsgLen, XPVOID lParam);
private:
	std::shared_mutex st_Locker;
private:
	std::unordered_map<XNETHANDLE, AICLIENT_IMAGE*> stl_MapAIClient;
};