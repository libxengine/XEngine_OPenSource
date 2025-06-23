#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include "../../XEngine_Module/XEngine_AIApi/AIApi_Define.h"
#include "../../XEngine_Module/XEngine_AIApi/AIApi_Error.h"

#ifdef _MSC_BUILD
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../../x64/Debug/XEngine_AIApi.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Debug/XEngine_AIApi.lib")
#elif _M_IX86
#pragma comment(lib,"../../Debug/XEngine_AIApi.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../../x64/Release/XEngine_AIApi.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Release/XEngine_AIApi.lib")
#elif _M_IX86
#pragma comment(lib,"../../Release/XEngine_AIApi.lib")
#endif
#endif
#endif

//Linux Macos::g++ -std=c++17 -Wall -g AIApi_APPExample.cpp -o AIApi_APPExample.exe -L ../../XEngine_Module/XEngine_AIApi -lXEngine_AIApi

void XCALLBACK XEngine_AIApi_CBRecv(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	printf("%lld,Name:%s:%d=%s\n", xhToken, lpszModelName, nMsgLen, lpszMsgBuffer);
}
int main()
{
	XNETHANDLE xhToken = 0;

	LPCXSTR lpszAPIUrl = _X("https://api.zhizengzeng.com/v1/chat/completions");
	LPCXSTR lpszAPIKey = _X("sk-zk2e0065dd7c8411828ce612337c2f9d1086841436f63c48");
	LPCXSTR lpszAPIModel = _X("gpt-4o");

	//LPCXSTR lpszAPIUrl = _X("https://api.hunyuan.cloud.tencent.com/v1/chat/completions");
	//LPCXSTR lpszAPIKey = _X("sk-dXVprOeQSWrodQcTYuHoGytHaifId7QlwaBebeWxfyPUipFk");
	//LPCXSTR lpszAPIModel = _X("hunyuan-turbos-latest");
	
	if (!AIApi_Chat_Create(&xhToken, lpszAPIUrl, lpszAPIKey, XEngine_AIApi_CBRecv))
	{
		printf("AIApi_Chat_Create:%lX\n", AIApi_GetLastError());
		return 0;
	}
	AIApi_Chat_SetRole(xhToken, _X("You are a helpful assistant."));
	LPCXSTR lpszMSGBuffer = _X("hello");
	int nMSGLen = strlen(lpszMSGBuffer);
	if (!AIApi_Chat_Excute(xhToken, lpszAPIModel, lpszMSGBuffer, nMSGLen, false))
	{
		printf("AIApi_Chat_Excute:%lX\n", AIApi_GetLastError());
		return 0;
	}

	bool bCompleted = false;
	AIApi_Chat_GetStatus(xhToken, &bCompleted);
	lpszMSGBuffer = _X("what can you do for me?");
	nMSGLen = strlen(lpszMSGBuffer);
	if (!AIApi_Chat_Excute(xhToken, lpszAPIModel, lpszMSGBuffer, nMSGLen, true))
	{
		printf("AIApi_Chat_Excute:%lX\n", AIApi_GetLastError());
		return 0;
	}
	bCompleted = false;
	AIApi_Chat_GetStatus(xhToken, &bCompleted);

	lpszMSGBuffer = _X("李白是谁?");
	nMSGLen = strlen(lpszMSGBuffer);
	if (!AIApi_Chat_Excute(xhToken, lpszAPIModel, lpszMSGBuffer, nMSGLen, true))
	{
		printf("AIApi_Chat_Excute:%lX\n", AIApi_GetLastError());
		return 0;
	}
	bCompleted = false;
	AIApi_Chat_GetStatus(xhToken, &bCompleted);
	AIApi_Chat_Destory(xhToken);
	return 1;
}