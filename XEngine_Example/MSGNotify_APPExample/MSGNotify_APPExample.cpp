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
#include "../../XEngine_Module/XEngine_MSGNotify/MSGNotify_Define.h"
#include "../../XEngine_Module/XEngine_MSGNotify/MSGNotify_Error.h"

#ifdef _MSC_BUILD
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../../x64/Debug/XEngine_MSGNotify.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Debug/XEngine_MSGNotify.lib")
#elif _M_IX86
#pragma comment(lib,"../../Debug/XEngine_MSGNotify.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../../x64/Release/XEngine_MSGNotify.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Release/XEngine_MSGNotify.lib")
#elif _M_IX86
#pragma comment(lib,"../../Release/XEngine_MSGNotify.lib")
#endif
#endif
#endif

//Linux Macos::g++ -std=c++17 -Wall -g MSGNotify_APPExample.cpp -o MSGNotify_APPExample.exe -L ../../XEngine_Module/XEngine_MSGNotify -lXEngine_MSGNotify


int main()
{
	LPCXSTR lpszServiceAddr = _X("smtp://smtp.163.com");
	LPCXSTR lpszUser = _X("ggqytgodlove@163.com");
	LPCXSTR lpszPass = _X("STZsBi3uuJvWsWL3");
	LPCXSTR lpszSendAddr = _X("486179@qq.com");
	LPCXSTR lpszSubject = _X("this is test");
	LPCXSTR lpszPayload = _X("email from test\r\nand test to 4444");
	if (!MSGNotify_EMail_Send(lpszServiceAddr, lpszUser, lpszPass, lpszSendAddr, lpszSubject, lpszPayload))
	{
		printf("MSGNotify_EMail_Send:%lX\n", MSGNotify_GetLastError());
		return -1;
	}
	printf("MSGNotify_EMail_Send:ok\n");
	return 1;
}