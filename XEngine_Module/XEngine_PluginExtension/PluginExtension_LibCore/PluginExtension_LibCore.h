#pragma once
/********************************************************************
//    Created:     2026/01/27  15:26:23
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\PluginExtension_LibCore\PluginExtension_LibCore.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\PluginExtension_LibCore
//    File Base:   PluginExtension_LibCore
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     模块插件核心架构
//    History:
*********************************************************************/
typedef bool(*FPCall_PluginCore_Init)(XENGINE_PLUGINPARAM* pSt_PluginParameter);
typedef void(*FPCall_PluginCore_UnInit)();
typedef void(*FPCall_PluginCore_GetInfo)(XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc);
typedef bool(*FPCall_PluginCore_Call)(XCHAR*** pppHDRList, int nListCount, int* pInt_HTTPCode, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLen);
typedef XLONG(*FPCall_PluginCore_GetLastError)();

typedef struct 
{
#ifdef _MSC_BUILD
	HMODULE mhFile;
#else
	void* mhFile;
#endif
	XCHAR tszModuleFile[XPATH_MAX];
	XCHAR tszModuleName[128];
	XCHAR tszModuleAuthor[128];
	XCHAR tszModuleDesc[128];
	XCHAR tszModuleVer[64];

	bool(*fpCall_PluginCore_Init)(XENGINE_PLUGINPARAM* pSt_PluginParameter);
	void(*fpCall_PluginCore_UnInit)();
	void(*fpCall_PluginCore_GetInfo)(XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc);
	bool(*fpCall_PluginCore_Call)(XCHAR*** pppHDRList, int nListCount, int* pInt_HTTPCode, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLen);
	XLONG(*fpCall_PluginCore_GetLastError)();
}PLUGINCORE_LIBFRAMEWORK;

class CPluginExtension_LibCore
{
public:
	CPluginExtension_LibCore();
	~CPluginExtension_LibCore();
public:
	bool PluginExtension_LibCore_Init();
	bool PluginExtension_LibCore_Push(XNETHANDLE* pxhModule, LPCXSTR lpszPluginFile, XENGINE_PLUGINPARAM* pSt_PluginParameter = NULL);
	bool PluginExtension_LibCore_Exec(XNETHANDLE xhModule, XCHAR*** pppHDRList, int nListCount, int* pInt_HTTPCode, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer = NULL, int nMsgLen = 0);
	bool PluginExtension_LibCore_Get(XNETHANDLE xhModule, XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc);
	bool PluginExtension_LibCore_Destroy();
protected:
	bool PluginExtension_LibCore_Add(XNETHANDLE xhNet, LPCXSTR lpszPluginFile, XENGINE_PLUGINPARAM* pSt_PluginParameter = NULL);
private:
	bool bIsInit;
private:
	shared_mutex st_csStl;
private:
	unordered_map<XNETHANDLE, PLUGINCORE_LIBFRAMEWORK> stl_MapFrameWork;
};
