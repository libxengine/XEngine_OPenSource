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
#include "../../XEngine_Module/XEngine_AVPlayer/AVPlayer_Define.h"
#include "../../XEngine_Module/XEngine_AVPlayer/AVPlayer_Error.h"

#ifdef _MSC_BUILD
#pragma comment(lib,"../../Debug/XEngine_AVPlayer.lib")
#endif

//Linux::g++ -std=c++17 -Wall -g AVCodec_APPPlayer.cpp -o AVCodec_APPPlayer.exe -L ../../XEngine_Module/XEngine_AVPlayer -lXEngine_AVPlayer
//Macos::g++ -std=c++17 -Wall -g AVCodec_APPPlayer.cpp -o AVCodec_APPPlayer.exe -L ../../XEngine_Module/XEngine_AVPlayer -lXEngine_AVPlayer

int Player_Video()
{
	LPCXSTR lpszName = _X("test");

	XHANDLE xhPlayer = AVPlayer_Video_Create(NULL, lpszName, 720, 480);
	if (NULL == xhPlayer)
	{
		return -1;
	}
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\h264 file\\ds.yuv");
#else
	LPCXSTR lpszFile = _X("./ds.yuv");
#endif
	int nSize = 720 * 480 * 3 / 2;
	XCHAR* ptszBuffer = new XCHAR[nSize];
	FILE* pSt_File = fopen(lpszFile, "rb");
	while (1)
	{
		memset(ptszBuffer, '\0', nSize);
		int nRet = fread(ptszBuffer, 1, nSize, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		AVPlayer_Video_Push(xhPlayer, (uint8_t*)ptszBuffer, 720);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	AVPlayer_Video_Close(xhPlayer);
	return -2;
}
int Player_Audio()
{
	LPCXSTR lpszName = _X("test");

	XHANDLE xhPlayer = AVPlayer_Audio_Create(44100, 1, 2);
	if (NULL == xhPlayer)
	{
		return -1;
	}
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\audio\\44.1k_2_16.aac.pcm");
#else
	LPCXSTR lpszFile = _X("44.1k_2_16.aac.pcm");
#endif
	int nSize = 8192;
	XCHAR* ptszBuffer = new XCHAR[nSize];
	FILE* pSt_File = fopen(lpszFile, "rb");
	while (1)
	{
		memset(ptszBuffer, '\0', nSize);
		int nRet = fread(ptszBuffer, 1, nSize, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		AVPlayer_Audio_Push(xhPlayer, ptszBuffer, nRet);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	AVPlayer_Audio_Close(xhPlayer);
	return -2;
}

int main()
{
	Player_Audio();
	Player_Video();

	return 1;
}