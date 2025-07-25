﻿#ifdef _MSC_BUILD
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

void XCALLBACK XEngine_AIApi_CBRecv(XNETHANDLE xhToken, LPCXSTR lpszModelName, LPCXSTR lpszMsgBuffer, int nMsgLen, bool bThink, XPVOID lParam)
{
	if (bThink)
	{
		printf("think:%lld,Name:%s:%d=%s\n", xhToken, lpszModelName, nMsgLen, lpszMsgBuffer);
	}
	else
	{
		printf("chat:%lld,Name:%s:%d=%s\n", xhToken, lpszModelName, nMsgLen, lpszMsgBuffer);
	}
}

int Test_CreateImage()
{
	XNETHANDLE xhToken = 0;

	LPCXSTR lpszAPIUrl = _X("https://ark.cn-beijing.volces.com/api/v3/images/generations");
	LPCXSTR lpszAPIKey = _X("d68056c1-1faa-438e-8476-1");
	LPCXSTR lpszAPIModel = _X("doubao-seedream-3-0-t2i-250415");

	if (!AIApi_Vision_Create(&xhToken, lpszAPIUrl, lpszAPIKey, XEngine_AIApi_CBRecv))
	{
		printf("AIApi_Vision_Create:%lX\n", AIApi_GetLastError());
		return 0;
	}

	LPCXSTR lpszMSGBuffer = _X("生成一张图片:鱼眼镜头，一只猫咪的头部，画面呈现出猫咪的五官因为拍摄方式扭曲的效果。");
	int nMSGLen = strlen(lpszMSGBuffer);

	int nImageSize = 0;
	XCHAR* ptszMSGBuffer = NULL;
	if (!AIApi_Vision_ExcuteCrete(xhToken, lpszAPIModel, _X("1024x1024"), lpszMSGBuffer, nMSGLen, &ptszMSGBuffer, &nImageSize, false))
	{
		printf("AIApi_Vision_ExcuteCrete:%lX\n", AIApi_GetLastError());
		return 0;
	}
	AIApi_Help_Base64DecodecFile(ptszMSGBuffer, nImageSize, _X("D:\\ai_create.png"));
	AIApi_Vision_Destory(xhToken);
	return 1;
}
int Test_ParseImage()
{
	XNETHANDLE xhToken = 0;

	LPCXSTR lpszAPIUrl = _X("https://ark.cn-beijing.volces.com/api/v3/chat/completions");
	LPCXSTR lpszAPIKey = _X("d68056c1-1faa-438e-8476-1");
	LPCXSTR lpszAPIModel = _X("doubao-1-5-vision-pro-32k-250115");

	if (!AIApi_Vision_Create(&xhToken, lpszAPIUrl, lpszAPIKey, XEngine_AIApi_CBRecv))
	{
		printf("AIApi_Vision_Create:%lX\n", AIApi_GetLastError());
		return 0;
	}

	LPCXSTR lpszMSGBuffer = _X("请描述下这个图片内容?");
	int nMSGLen = strlen(lpszMSGBuffer);

	if (true)
	{
		LPCXSTR lpszFileName = _X("D:\\20240419102308.png");

		int nMSGLen = 0;
		XCHAR* ptszMSGBuffer = NULL;
		AIApi_Help_Base64EncodecFile(lpszFileName, &ptszMSGBuffer, &nMSGLen);
		if (!AIApi_Vision_ExcuteParse(xhToken, lpszAPIModel, ptszMSGBuffer, lpszMSGBuffer, nMSGLen, true))
		{
			printf("AIApi_Vision_ExcuteParse:%lX\n", AIApi_GetLastError());
			return 0;
		}
		free(ptszMSGBuffer);
	}
	else
	{
		LPCXSTR lpszUrlBuffer = _X("https://www.xyry.org/XEngine_StructPic/EngineFrameWork.png");
		if (!AIApi_Vision_ExcuteParse(xhToken, lpszAPIModel, lpszUrlBuffer, lpszMSGBuffer, nMSGLen, true))
		{
			printf("AIApi_Vision_ExcuteParse:%lX\n", AIApi_GetLastError());
			return 0;
		}
	}
	
	bool bCompleted = false;
	AIApi_Vision_GetStatus(xhToken, &bCompleted);
	AIApi_Vision_Destory(xhToken);
	return 1;
}
int Test_Think()
{
	XNETHANDLE xhToken = 0;

	LPCXSTR lpszAPIUrl = _X("https://ark.cn-beijing.volces.com/api/v3/chat/completions");
	LPCXSTR lpszAPIKey = _X("d68056c1-1faa-438e-8476-1");
	LPCXSTR lpszAPIModel = _X("doubao-seed-1-6-thinking-250615");

	if (!AIApi_Chat_Create(&xhToken, lpszAPIUrl, lpszAPIKey, XEngine_AIApi_CBRecv))
	{
		printf("AIApi_Chat_Create:%lX\n", AIApi_GetLastError());
		return 0;
	}
	AIApi_Chat_SetRole(xhToken, _X("You are a helpful assistant."));
	LPCXSTR lpszMSGBuffer = _X("李白是谁?");
	int nMSGLen = strlen(lpszMSGBuffer);
	if (!AIApi_Chat_Excute(xhToken, lpszAPIModel, lpszMSGBuffer, nMSGLen, true))
	{
		printf("AIApi_Chat_Excute:%lX\n", AIApi_GetLastError());
		return 0;
	}
	bool bCompleted = false;
	AIApi_Chat_GetStatus(xhToken, &bCompleted);
	AIApi_Chat_Destory(xhToken);
	return 1;
}
int Test_Chat()
{
	XNETHANDLE xhToken = 0;

	//LPCXSTR lpszAPIUrl = _X("https://api.zhizengzeng.com/v1/chat/completions");
	//LPCXSTR lpszAPIKey = _X("sk-1");
	//LPCXSTR lpszAPIModel = _X("gpt-4o");

	LPCXSTR lpszAPIUrl = _X("https://api.hunyuan.cloud.tencent.com/v1/chat/completions");
	LPCXSTR lpszAPIKey = _X("sk-1");
	LPCXSTR lpszAPIModel = _X("hunyuan-turbos-latest");

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
int main()
{
	Test_CreateImage();
	Test_ParseImage();
	Test_Think();
	Test_Chat();
	return 0;
}