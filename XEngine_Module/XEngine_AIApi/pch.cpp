#include "pch.h"
#include "AIApi_Chat/AIApi_Chat.h"
/********************************************************************
//    Created:     2025/05/29  14:07:51
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\pch.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool AIApi_IsErrorOccur = false;
XLONG AIApi_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAIApi_Chat m_AIChat;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG AIApi_GetLastError(int* pInt_ErrorCode)
{
	if (NULL != pInt_ErrorCode)
	{
		*pInt_ErrorCode = errno;
	}
	return AIApi_dwErrorCode;
}
/************************************************************************/
/*                        AI对话模型导出函数                            */
/************************************************************************/
extern "C" bool AIApi_Chat_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam)
{
	return m_AIChat.AIApi_Chat_Create(pxhToken, lpszAPIUrl, lpszAPIKey, fpCall_Chat, lParam);
}
extern "C" bool AIApi_Chat_Excute(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream)
{
	return m_AIChat.AIApi_Chat_Excute(xhToken, lpszModelName, lpszMSGBuffer, nMSGLen, bStream);
}
extern "C" bool AIApi_Chat_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode)
{
	return m_AIChat.AIApi_Chat_GetStatus(xhToken, pbComplete, pInt_HTTPCode);
}
extern "C" bool AIApi_Chat_Destory(XNETHANDLE xhToken)
{
	return m_AIChat.AIApi_Chat_Destory(xhToken);
}