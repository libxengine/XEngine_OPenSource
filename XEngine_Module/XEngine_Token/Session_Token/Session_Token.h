#pragma once
/********************************************************************
//    Created:     2025/05/21  14:10:22
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token\Session_Token\Session_Token.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token\Session_Token
//    File Base:   Session_Token
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     TOKEN会话管理器
//    History:
*********************************************************************/
typedef struct
{
	XENGINE_PROTOCOL_USERINFO st_UserInfo;
	XENGINE_LIBTIME st_LibTimer;                                          //登录时间结构
	XENGINE_LIBTIME st_OutTimer;                                          //超时时间结构
	XCHAR tszTokenStr[XPATH_MID];									      //TOKEN字符串
	int nTimeout;                                                         //单独指定超时
	int nRenewalTime;                                                     //自动续期次数
}TOKENSESSION_INFOCLIENT;
//////////////////////////////////////////////////////////////////////////
class CSession_Token
{
public:
	CSession_Token();
	~CSession_Token();
public:
	bool Session_Token_Init(int nTimeout, CALLBACK_XENGIEN_MODULE_TOKEN_EVENTS fpCall_TokenEvent, XPVOID lParam = NULL);
	bool Session_Token_Destroy();
	bool Session_Token_Create(XNETHANDLE *pxhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo = NULL, int nTimeout = -1);
	bool Session_Token_Insert(XNETHANDLE xhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo = NULL, int nTimeout = -1);
	bool Session_Token_Delete(XNETHANDLE xhToken);
	bool Session_Token_UPDate(XNETHANDLE xhToken);
	bool Session_Token_Get(XNETHANDLE xhToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo = NULL);
	bool Session_Token_GetTimeInfo(XNETHANDLE xhToken, XENGINE_LIBTIME* pSt_LoginTime = NULL, XENGINE_LIBTIME* pSt_UPTime = NULL);
	bool Session_Token_GetTimeout(XNETHANDLE xhToken, __int64x* pInt_TimeLogin = NULL, __int64x* pInt_Timeout = NULL);
	bool Session_Token_GetTimeRenewal(XNETHANDLE xhToken, int* pInt_RenewalTime);
	bool Session_Token_GetUser(LPCXSTR lpszUser, LPCXSTR lpszPass, XNETHANDLE* pxhToken);
	bool Session_Token_GetList(XNETHANDLE*** pppxhToken, int* pInt_ListCount);
public:
	bool Session_Token_CreateStr(XCHAR* ptszToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo = NULL, int nTimeout = -1);
	bool Session_Token_InsertStr(LPCXSTR lpszToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo = NULL, int nTimeout = -1);
	bool Session_Token_DeleteStr(LPCXSTR lpszToken);
	bool Session_Token_UPDateStr(LPCXSTR lpszToken);
	bool Session_Token_GetStr(LPCXSTR lpszToken, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo = NULL);
	bool Session_Token_GetTimeInfoStr(LPCXSTR lpszToken, XENGINE_LIBTIME* pSt_LoginTime = NULL, XENGINE_LIBTIME* pSt_UPTime = NULL);
	bool Session_Token_GetTimeoutStr(LPCXSTR lpszToken, __int64x* pInt_TimeLogin = NULL, __int64x* pInt_Timeout = NULL);
	bool Session_Token_GetTimeRenewalStr(LPCXSTR lpszToken, int* pInt_RenewalTime);
	bool Session_Token_GetUserStr(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR* ptszToken);
	bool Session_Token_GetListStr(XCHAR*** ppptszToken, int* pInt_ListCount);
protected:
	static XHTHREAD Session_Token_Thread(XPVOID lParam);
private:
	int m_nTimeout = 0;
	bool bIsRun = false;
	std::unique_ptr<std::thread> pSTDThread;
private:
	XPVOID m_lParam = NULL;
	CALLBACK_XENGIEN_MODULE_TOKEN_EVENTS lpCall_TokenEvents = NULL;
private:
	std::shared_mutex st_Locker;
private:
	std::unordered_map<xstring, TOKENSESSION_INFOCLIENT> stl_MapTokenStr;
};
