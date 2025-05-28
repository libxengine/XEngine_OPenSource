#pragma once
/********************************************************************
//    Created:     2025/05/21  14:14:31
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token\Session_Dynamic\Session_Dynamic.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Token\Session_Dynamic
//    File Base:   Session_Dynamic
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     动态码会话管理
//    History:
*********************************************************************/
typedef struct
{
	XNETHANDLE xhToken;
	XSHOT nDynamicCode;
	time_t nTimeStart;
}DYANMICSESSION_INFOCLIENT;

class CSession_Dynamic
{
public:
	CSession_Dynamic();
	~CSession_Dynamic();
public:
	bool Session_Dynamic_Init(int nSecond);
	bool Session_Dynamic_Destory();
	bool Session_Dynamic_Create(XNETHANDLE* pxhToken, XSHOT* pInt_DynamicCode, __int64x nDynamicStart = 10000, __int64x nDynamicEnd = 65535);
	bool Session_Dynamic_Insert(XNETHANDLE xhToken, XSHOT* pInt_DynamicCode, __int64x nDynamicStart = 10000, __int64x nDynamicEnd = 65535);
	bool Session_Dynamic_Delete(XNETHANDLE xhToken);
	bool Session_Dynamic_Get(XNETHANDLE xhToken, int* pInt_DynamicCode);
protected:
	static XHTHREAD XCALLBACK Session_Dynamic_Thread(XPVOID lParam);
private:
	int m_nSescond = 0;
	bool m_bRun = false;
private:
	std::shared_mutex st_Locker;
	std::unique_ptr<std::thread> pSTDThread;
private:
	std::unordered_map<XNETHANDLE, DYANMICSESSION_INFOCLIENT> stl_MapDynamicCode;
};