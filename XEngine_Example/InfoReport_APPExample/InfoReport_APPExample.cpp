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
#include "../../XEngine_Module/XEngine_InfoReport/InfoReport_Define.h"
#include "../../XEngine_Module/XEngine_InfoReport/InfoReport_Error.h"

#ifdef _MSC_BUILD
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../../x64/Debug/XEngine_InfoReport.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Debug/XEngine_InfoReport.lib")
#elif _M_IX86
#pragma comment(lib,"../../Debug/XEngine_InfoReport.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../../x64/Release/XEngine_InfoReport.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Release/XEngine_InfoReport.lib")
#elif _M_IX86
#pragma comment(lib,"../../Release/XEngine_InfoReport.lib")
#endif
#endif
#endif

//Linux Macos::g++ -std=c++17 -Wall -g InfoReport_APPExample.cpp -o InfoReport_APPExample.exe -L ../../XEngine_Module/XEngine_InfoReport -lXEngine_InfoReport


int main()
{
	LPCXSTR lpszAPIUrl = _X("http://127.0.0.1:5501/api?function=machine");
	LPCXSTR lpszAPPName = _X("InfoReport_APPExample");
	
	if (!InfoReport_APIMachine_Send(lpszAPIUrl, lpszAPPName))
	{
		printf("InfoReport_APIMachine_Send:%lX\n", InfoReport_GetLastError());
		return -1;
	}
	
	__int64x nTimeNumber = 0;
	if (!InfoReport_APIMachine_GetTime(lpszAPIUrl, lpszAPPName, &nTimeNumber))
	{
		printf("InfoReport_APIMachine_GetTime:%lX\n", InfoReport_GetLastError());
		return -1;
	}
	printf("Time:%lld\n", nTimeNumber);
	return 1;
}