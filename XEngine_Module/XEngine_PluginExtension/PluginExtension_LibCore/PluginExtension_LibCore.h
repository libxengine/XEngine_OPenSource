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
typedef int(*FPCall_PluginCore_RegisterType)();
typedef void(*FPCall_PluginCore_GetInfo)(XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc);
typedef bool(*FPCall_PluginCore_Call)(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLen, XCHAR*** pppInputParameters, int nInputPCount, XCHAR*** pppOutputParameters, int* pInt_OutputPCount);
typedef bool(*FPCall_PluginCore_Call2)(XHANDLE*** ppphBuffer);
typedef bool(*FPCall_PluginCore_Call3)(XHANDLE phBuffer, XHANDLE*** ppphBuffer, int *pInt_ListCount);
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
	int nCallCount;

	bool(*fpCall_PluginCore_Init)(XENGINE_PLUGINPARAM* pSt_PluginParameter);
	void(*fpCall_PluginCore_UnInit)();
	int(*fpCall_PluginCore_RegisterType)();
	void(*fpCall_PluginCore_GetInfo)(XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc);
	bool(*fpCall_PluginCore_Call)(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLe, XCHAR*** pppInputParameters, int nInputPCount, XCHAR*** pppOutputParameters, int* pInt_OutputPCount);
	bool(*fpCall_PluginCore_Call2)(XHANDLE*** ppphBuffer);
	bool(*fpCall_PluginCore_Call3)(XHANDLE phBuffer, XHANDLE ***ppphBuffer, int* pInt_ListCount);
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
	int PluginExtension_LibCore_RegisterType(XNETHANDLE xhModule);
	bool PluginExtension_LibCore_Exec(XNETHANDLE xhModule, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer = NULL, int nMsgLen = 0, XCHAR*** pppInputParameters = NULL, int nInputPCount = 0, XCHAR*** pppOutputParameters = NULL, int* pInt_OutputPCount = NULL);
	bool PluginExtension_LibCore_Exec2(XNETHANDLE xhModule, XHANDLE*** ppphBuffer);
	bool PluginExtension_LibCore_Exec3(XNETHANDLE xhModule, XHANDLE phBuffer, XHANDLE*** ppphBuffer, int* pInt_ListCount);
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
