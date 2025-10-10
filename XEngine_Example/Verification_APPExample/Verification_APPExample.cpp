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
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include "../../XEngine_Module/XEngine_Verification/Verification_Define.h"
#include "../../XEngine_Module/XEngine_Verification/Verification_Error.h"

#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../../x64/Debug/XEngine_Verification.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Debug/XEngine_Verification.lib")
#elif _M_IX86
#pragma comment(lib,"../../Debug/XEngine_Verification.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../../x64/Release/XEngine_Verification.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Release/XEngine_Verification.lib")
#elif _M_IX86
#pragma comment(lib,"../../Release/XEngine_Verification.lib")
#endif
#endif
#endif

//Linux Macos::g++ -std=c++17 -Wall -g Verification_APPExample.cpp -o Verification_APPExample.exe -L ../../XEngine_Module/XEngine_Verification -lXEngine_Verification


int main()
{
	LPCXSTR lpszKeyPass = _X("123123aa");
	LPCXSTR lpszKeyFile = _X("D:\\XEngine_OPenSource\\Debug\\test.key");
	VERIFICATION_XAUTHKEY st_XAuthInfo = {};

	sprintf(st_XAuthInfo.st_AuthUserInfo.tszUserName, _X("qyt"));
	sprintf(st_XAuthInfo.st_AuthUserInfo.tszUserContact, _X("486179@qq.com"));

	Verification_XAuthKey_KeyInit(&st_XAuthInfo);
	Verification_XAuthKey_FileWrite(&st_XAuthInfo, lpszKeyFile, lpszKeyPass);

	memset(&st_XAuthInfo, 0, sizeof(VERIFICATION_XAUTHKEY));
	Verification_XAuthKey_FileRead(&st_XAuthInfo, lpszKeyFile, lpszKeyPass);

	Verification_XAuthKey_UserRegister(&st_XAuthInfo, st_XAuthInfo.st_AuthSerial.st_TimeLimit.tszTimeSerial);
	Verification_XAuthKey_WriteTime(&st_XAuthInfo);
	Verification_XAuthKey_WriteTime(&st_XAuthInfo);

	int nListCount = 0;
	XCHAR** pptszTimeList;
	Verification_XAuthKey_ReadTime(&st_XAuthInfo, &pptszTimeList, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s\n", pptszTimeList[i]);
	}
	BaseLib_Memory_Free((XPPPMEM)&pptszTimeList, nListCount);

	Verification_XAuthKey_FileWrite(&st_XAuthInfo, lpszKeyFile, lpszKeyPass);
	return 0;
}