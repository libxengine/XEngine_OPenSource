#pragma once
/********************************************************************
//    Created:     2026/01/27  15:28:19
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\PluginExtension_LuaCore\PluginExtension_LuaCore.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\PluginExtension_LuaCore
//    File Base:   PluginExtension_LuaCore
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     LUA脚本插件
//    History:
*********************************************************************/
typedef struct
{
	XCHAR tszModuleFile[XPATH_MAX];
	XCHAR tszModuleName[128];
	XCHAR tszModuleAuthor[128];
	XCHAR tszModuleDesc[128];
	XCHAR tszModuleVer[64];
#ifdef _XENGINE_BUILD_SWITCH_LUA
	lua_State* pSt_LuaState;
#endif
}PLUGINCORE_LUAFRAMEWORK;

class CPluginExtension_LuaCore
{
public:
	CPluginExtension_LuaCore();
	~CPluginExtension_LuaCore();
public:
	bool PluginExtension_LuaCore_Init();
	bool PluginExtension_LuaCore_Push(XNETHANDLE* pxhModule, LPCXSTR lpszPluginFile, XENGINE_PLUGINPARAM* pSt_PluginParameter = NULL);
	bool PluginExtension_LuaCore_Exec(XNETHANDLE xhModule, XCHAR*** pppHDRList, int nListCount, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer = NULL, int nMsgLen = 0, int* pInt_HTTPCode = NULL);
	bool PluginExtension_LuaCore_Get(XNETHANDLE xhModule, XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc);
	bool PluginExtension_LuaCore_Destroy();
protected:
	bool PluginExtension_LuaCore_Add(XNETHANDLE xhNet, LPCXSTR lpszPluginFile, XENGINE_PLUGINPARAM* pSt_PluginParameter = NULL);
private:
	shared_mutex st_csStl;
private:
	unordered_map<XNETHANDLE, PLUGINCORE_LUAFRAMEWORK> stl_MapFrameWork;
};
