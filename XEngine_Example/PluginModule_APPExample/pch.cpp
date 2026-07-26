#include "pch.h"
#include "Plugin_VideoHook/Plugin_VideoHook.h"
/********************************************************************
//    Created:     2022/04/21  15:52:56
//    File Name:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook\pch.cpp
//    File Path:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook
//    File Base:   pch
//    File Ext:    cpp
//    Project:     AVCodec_PluginExample
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool VideoHook_IsErrorOccur = false;
XLONG VideoHook_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CPlugin_VideoHook m_PluginVideoHook;
//////////////////////////////////////////////////////////////////////////
//                       导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG PluginCore_GetLastError()
{
	return VideoHook_dwErrorCode;
}
/*********************************************************************************
*                                导出函数定义                                    *
*********************************************************************************/
extern "C" bool PluginCore_Init(XENGINE_PLUGINPARAM* pSt_PluginParameter)
{
	return m_PluginVideoHook.PluginCore_Init(pSt_PluginParameter);
}
extern "C" void PluginCore_UnInit()
{
	m_PluginVideoHook.PluginCore_UnInit();
}
extern "C" int PluginCore_RegisterType()
{
	return m_PluginVideoHook.PluginCore_RegisterType();
}
extern "C" void PluginCore_GetInfo(XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc)
{
	return m_PluginVideoHook.PluginCore_GetInfo(ptszPluginName, ptszPluginVersion, ptszPluginAuthor, ptszPluginDesc);
}
extern "C" bool PluginCore_Call(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLen, XCHAR*** pppInputParameters, int nInputPCount, XCHAR*** pppOutputParameters, int* pInt_OutputPCount)
{
	return m_PluginVideoHook.PluginCore_Call(ptszMsgBuffer, pInt_MsgLen, lpszMsgBufer, nMsgLen, pppInputParameters, nInputPCount, pppOutputParameters, pInt_OutputPCount);
}
extern "C" bool PluginCore_Call2(XHANDLE phBuffer)
{
	return m_PluginVideoHook.PluginCore_Call2(phBuffer);
}