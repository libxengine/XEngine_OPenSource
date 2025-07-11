#include "pch.h"
#include "AIApi_Chat/AIApi_Chat.h"
#include "AIApi_Image/AIApi_Image.h"
#include "AIApi_Help/AIApi_Help.h"
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
CAIApi_Image m_AIImage;
CAIApi_Help m_AIHelp;
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
extern "C" bool AIApi_Chat_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam, bool bHistory)
{
	return m_AIChat.AIApi_Chat_Create(pxhToken, lpszAPIUrl, lpszAPIKey, fpCall_Chat, lParam, bHistory);
}
extern "C" bool AIApi_Chat_Excute(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream, int nMaxToken)
{
	return m_AIChat.AIApi_Chat_Excute(xhToken, lpszModelName, lpszMSGBuffer, nMSGLen, bStream, nMaxToken);
}
extern "C" bool AIApi_Chat_SetRole(XNETHANDLE xhToken, LPCXSTR lpszRoleName)
{
	return m_AIChat.AIApi_Chat_SetRole(xhToken, lpszRoleName);
}
extern "C" bool AIApi_Chat_Clear(XNETHANDLE xhToken)
{
	return m_AIChat.AIApi_Chat_Clear(xhToken);
}
extern "C" bool AIApi_Chat_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode, bool bWaitExist)
{
	return m_AIChat.AIApi_Chat_GetStatus(xhToken, pbComplete, pInt_HTTPCode, bWaitExist);
}
extern "C" bool AIApi_Chat_Destory(XNETHANDLE xhToken)
{
	return m_AIChat.AIApi_Chat_Destory(xhToken);
}
/************************************************************************/
/*                        AI图像处理模型导出函数                        */
/************************************************************************/
extern "C" bool AIApi_Image_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAPIUrl, LPCXSTR lpszAPIKey, CALLBACK_XENGINE_MODULE_AIAPI_CHAT fpCall_Chat, XPVOID lParam)
{
	return m_AIImage.AIApi_Image_Create(pxhToken, lpszAPIUrl, lpszAPIKey, fpCall_Chat, lParam);
}
extern "C" bool AIApi_Image_ExcuteParse(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszUrlBase, LPCXSTR lpszMSGBuffer, int nMSGLen, bool bStream)
{
	return m_AIImage.AIApi_Image_ExcuteParse(xhToken, lpszModelName, lpszUrlBase, lpszMSGBuffer, nMSGLen, bStream);
}
extern "C" bool AIApi_Image_ExcuteCrete(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMSGSize, LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR** pptszMSGBuffer, int* pInt_MSGLen, bool bCallback)
{
	return m_AIImage.AIApi_Image_ExcuteCrete(xhToken, lpszModelName, lpszMSGSize, lpszMSGBuffer, nMSGLen, pptszMSGBuffer, pInt_MSGLen, bCallback);
}
extern "C" bool AIApi_Image_GetStatus(XNETHANDLE xhToken, bool* pbComplete, int* pInt_HTTPCode, bool bWaitExist)
{
	return m_AIImage.AIApi_Image_GetStatus(xhToken, pbComplete, pInt_HTTPCode, bWaitExist);
}
extern "C" bool AIApi_Image_Destory(XNETHANDLE xhToken)
{
	return m_AIImage.AIApi_Image_Destory(xhToken);
}
/************************************************************************/
/*                        AI帮助 导出函数                               */
/************************************************************************/
extern "C" bool AIApi_Help_Base64EncodecFile(LPCXSTR lpszFileName, XCHAR** pptszMSGBuffer, int* pInt_MSGLen)
{
	return m_AIHelp.AIApi_Help_Base64EncodecFile(lpszFileName, pptszMSGBuffer, pInt_MSGLen);
}
extern "C" bool AIApi_Help_Base64EncodecMemory(LPCXSTR lpszFileExt, LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR** pptszMSGBuffer, int* pInt_MSGLen)
{
	return m_AIHelp.AIApi_Help_Base64EncodecMemory(lpszFileExt, lpszMSGBuffer, nMSGLen, pptszMSGBuffer, pInt_MSGLen);
}
extern "C" bool AIApi_Help_Base64DecodecFile(LPCXSTR lpszMSGBuffer, int nMSGLen, LPCXSTR lpszFileName)
{
	return m_AIHelp.AIApi_Help_Base64DecodecFile(lpszMSGBuffer, nMSGLen, lpszFileName);
}
extern "C" bool AIApi_Help_Base64DecodecMemory(LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	return m_AIHelp.AIApi_Help_Base64DecodecMemory(lpszMSGBuffer, nMSGLen, ptszMSGBuffer, pInt_MSGLen);
}