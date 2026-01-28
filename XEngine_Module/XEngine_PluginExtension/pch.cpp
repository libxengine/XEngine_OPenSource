#include "pch.h"
#include "PluginExtension_LibCore/PluginExtension_LibCore.h"
#include "PluginExtension_LuaCore/PluginExtension_LuaCore.h"
#include "PluginExtension_Loader/PluginExtension_Loader.h"
/********************************************************************
//    Created:     2026/01/27  15:36:26
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\pch.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     插件核心导出函数实现
//    History:
*********************************************************************/
bool PluginExtension_IsErrorOccur = false;
XLONG PluginExtension_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CPluginExtension_LibCore m_PluginLib;
CPluginExtension_LuaCore m_PluginLua;
CPluginExtension_Loader m_PluginLoader;
//////////////////////////////////////////////////////////////////////////
//                       导出函数实现
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG PluginExtension_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return PluginExtension_dwErrorCode;
}
/*********************************************************************************
*                        插件框架导出函数定义                                       *
*********************************************************************************/
extern "C" bool PluginExtension_LibCore_Init()
{
	return m_PluginLib.PluginExtension_LibCore_Init();
}
extern "C" bool PluginExtension_LibCore_Push(XNETHANDLE* pxhModule, LPCXSTR lpszPluginFile, XENGINE_PLUGINPARAM* pSt_PluginParameter)
{
	return m_PluginLib.PluginExtension_LibCore_Push(pxhModule, lpszPluginFile, pSt_PluginParameter);
}
extern "C" bool PluginExtension_LibCore_Exec(XNETHANDLE xhModule, XCHAR*** pppHDRList, int nListCount, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLen, int* pInt_HTTPCode)
{
	return m_PluginLib.PluginExtension_LibCore_Exec(xhModule, pppHDRList, nListCount, ptszMsgBuffer, pInt_MsgLen, lpszMsgBufer, nMsgLen, pInt_HTTPCode);
}
extern "C" bool PluginExtension_LibCore_Get(XNETHANDLE xhModule, XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc)
{
	return m_PluginLib.PluginExtension_LibCore_Get(xhModule, ptszPluginName, ptszPluginVersion, ptszPluginAuthor, ptszPluginDesc);
}
extern "C" bool PluginExtension_LibCore_Destroy()
{
	return m_PluginLib.PluginExtension_LibCore_Destroy();
}
/*********************************************************************************
*                        LUA插件框架导出函数定义                                 *
*********************************************************************************/
extern "C" bool PluginExtension_Loader_Init()
{
	return m_PluginLoader.PluginExtension_Loader_Init();
}
extern "C" bool PluginExtension_LuaCore_Init()
{
	return m_PluginLua.PluginExtension_LuaCore_Init();
}
extern "C" bool PluginExtension_LuaCore_Push(XNETHANDLE* pxhModule, LPCXSTR lpszPluginFile, XENGINE_PLUGINPARAM* pSt_PluginParameter)
{
	return m_PluginLua.PluginExtension_LuaCore_Push(pxhModule, lpszPluginFile, pSt_PluginParameter);
}
extern "C" bool PluginExtension_LuaCore_Exec(XNETHANDLE xhModule, XCHAR*** pppHDRList, int nListCount, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLen, int* pInt_HTTPCode)
{
	return m_PluginLua.PluginExtension_LuaCore_Exec(xhModule, pppHDRList, nListCount, ptszMsgBuffer, pInt_MsgLen, lpszMsgBufer, nMsgLen, pInt_HTTPCode);
}
extern "C" bool PluginExtension_LuaCore_Get(XNETHANDLE xhModule, XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc)
{
	return m_PluginLua.PluginExtension_LuaCore_Get(xhModule, ptszPluginName, ptszPluginVersion, ptszPluginAuthor, ptszPluginDesc);
}
extern "C" bool PluginExtension_LuaCore_Destroy()
{
	return m_PluginLua.PluginExtension_LuaCore_Destroy();
}
/*********************************************************************************
*                        加载器导出函数定义                                      *
*********************************************************************************/
extern "C" bool PluginExtension_Loader_Insert(LPCXSTR lpszModuleName, int nType, XENGINE_PLUGINPARAM* pSt_PluginParameter)
{
	return m_PluginLoader.PluginExtension_Loader_Insert(lpszModuleName, nType, pSt_PluginParameter);
}
extern "C" bool PluginExtension_Loader_Find(LPCXSTR lpszMethodName, int* pInt_Type)
{
	return m_PluginLoader.PluginExtension_Loader_Find(lpszMethodName, pInt_Type);
}
extern "C" bool PluginExtension_Loader_Get(LPCXSTR lpszMethodName, XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc)
{
	return m_PluginLoader.PluginExtension_Loader_Get(lpszMethodName, ptszPluginName, ptszPluginVersion, ptszPluginAuthor, ptszPluginDesc);
}
extern "C" bool PluginExtension_Loader_GetForModule(LPCXSTR lpszModuleName, XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc)
{
	return m_PluginLoader.PluginExtension_Loader_GetForModule(lpszModuleName, ptszPluginName, ptszPluginVersion, ptszPluginAuthor, ptszPluginDesc);
}
extern "C" bool PluginExtension_Loader_Exec(LPCXSTR lpszMethodName, XCHAR*** pppHDRList, int nListCount, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLen, int* pInt_HTTPCode)
{
	return m_PluginLoader.PluginExtension_Loader_Exec(lpszMethodName, pppHDRList, nListCount, ptszMsgBuffer, pInt_MsgLen, lpszMsgBufer, nMsgLen, pInt_HTTPCode);
}
extern "C" bool PluginExtension_Loader_Destory()
{
	return m_PluginLoader.PluginExtension_Loader_Destory();
}