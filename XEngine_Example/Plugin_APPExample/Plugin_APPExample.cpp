#ifdef _MSC_BUILD
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include "../../XEngine_Module/XEngine_PluginExtension/PluginExtension_Define.h"
#include "../../XEngine_Module/XEngine_PluginExtension/PluginExtension_Error.h"

#ifdef _MSC_BUILD
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../../x64/Debug/XEngine_PluginExtension.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Debug/XEngine_PluginExtension.lib")
#elif _M_IX86
#pragma comment(lib,"../../Debug/XEngine_PluginExtension.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../../x64/Release/XEngine_PluginExtension.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Release/XEngine_PluginExtension.lib")
#elif _M_IX86
#pragma comment(lib,"../../Release/XEngine_PluginExtension.lib")
#endif
#endif
#endif

//Linux Macos::g++ -std=c++17 -Wall -g Plugin_APPExample.cpp -o Plugin_APPExample.exe -L ../../XEngine_Module/XEngine_PluginExtension -lXEngine_PluginExtension
typedef struct
{
	XCHAR tszUserData[128];
	int nUserNumber;
}XENGINE_PLUGINBUFFER;

int main()
{
	LPCXSTR lpszModuleName = _X("D:\\XEngine_OPenSource\\Debug\\PluginModule_VideoHook.dll");
	if (!PluginExtension_Loader_Init())
	{
		printf("PluginExtension_Loader_Init failed\n");
		return -1;
	}
	if (!PluginExtension_Loader_Insert(lpszModuleName, 0))
	{
		printf("PluginExtension_Loader_Insert failed\n");
		return -1;
	}
	XCHAR tszModuleName[128] = { 0 };
	XCHAR tszModuleVer[64] = { 0 };
	XCHAR tszModuleAuthor[128] = { 0 };
	PluginExtension_Loader_GetForModule(lpszModuleName, tszModuleName, tszModuleVer, tszModuleAuthor);
	printf("PluginExtension_Loader_GetForModule:ModuleName=%s,ModuleVer=%s,ModuleAuthor=%s\n", tszModuleName, tszModuleVer, tszModuleAuthor);

	XENGINE_PLUGINBUFFER st_PluginBuffer = {};
	st_PluginBuffer.nUserNumber = 100;
	_xstprintf(st_PluginBuffer.tszUserData, _X("videohook"));

	//lua
	LPCXSTR lpszLuaName = _X("D:\\XEngine_OPenSource\\Debug\\PluginModule_VideoHook.lua");
	if (!PluginExtension_Loader_Insert(lpszLuaName, 1))
	{
		printf("PluginExtension_Loader_Insert failed\n");
		return -1;
	}
	memset(tszModuleName, 0, sizeof(tszModuleName));
	memset(tszModuleVer, 0, sizeof(tszModuleVer));
	memset(tszModuleAuthor, 0, sizeof(tszModuleAuthor));

	PluginExtension_Loader_GetForModule(lpszLuaName, tszModuleName, tszModuleVer, tszModuleAuthor);
	printf("PluginExtension_Loader_GetForModule:ModuleName=%s,ModuleVer=%s,ModuleAuthor=%s\n", tszModuleName, tszModuleVer, tszModuleAuthor);

	st_PluginBuffer.nUserNumber = 100;
	_xstprintf(st_PluginBuffer.tszUserData, _X("audio"));
	

	PluginExtension_Loader_Destory();
	return 1;
}