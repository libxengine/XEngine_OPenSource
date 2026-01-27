#pragma once
/********************************************************************
//    Created:     2026/01/27  15:23:37
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\PluginExtension_Loader\PluginExtension_Loader.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\PluginExtension_Loader
//    File Base:   PluginExtension_Loader
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     插件加载工具
//    History:
*********************************************************************/
typedef struct
{
	XCHAR tszModuleFile[XPATH_MAX];
	XCHAR tszModuleName[128];
	XCHAR tszModuleAuthor[128];
	XCHAR tszModuleDesc[128];
	XCHAR tszModuleVer[128];

	XNETHANDLE xhToken;
	int nType;
}PLUGINCORE_LOADER, * LPPLUGINCORE_LOADER;

class CPluginExtension_Loader
{
public:
	CPluginExtension_Loader();
	~CPluginExtension_Loader();
public:
	bool PluginExtension_Loader_Init();
	bool PluginExtension_Loader_Insert(LPCXSTR lpszModuleName, int nType, XENGINE_PLUGINPARAM* pSt_PluginParameter = NULL);
	bool PluginExtension_Loader_Find(LPCXSTR lpszMethodName, int* pInt_Type);
	bool PluginExtension_Loader_Get(LPCXSTR lpszMethodName, XCHAR* ptszPluginName = NULL, XCHAR* ptszPluginVersion = NULL, XCHAR* ptszPluginAuthor = NULL, XCHAR* ptszPluginDesc = NULL);
	bool PluginExtension_Loader_GetForModule(LPCXSTR lpszModuleName, XCHAR* ptszPluginName = NULL, XCHAR* ptszPluginVersion = NULL, XCHAR* ptszPluginAuthor = NULL, XCHAR* ptszPluginDesc = NULL);
	bool PluginExtension_Loader_Exec(LPCXSTR lpszMethodName, XCHAR*** pppHDRList, int nListCount, int* pInt_HTTPCode, XCHAR* ptszMsgBuffer, int* pInt_MsgLen);
	bool PluginExtension_Loader_Destory();
protected:
private:
	shared_mutex st_Locker;
private:
	unordered_map<string, PLUGINCORE_LOADER> stl_MapLoader;
};
