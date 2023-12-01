#pragma once
/********************************************************************
//	Created:	2018/12/12   09:14
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer\AVPlayer_Audio\AVPlayer_Audio.h
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer\AVPlayer_Audio
//	File Base:	AVPlayer_Audio
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	音频播放器
//	History:
*********************************************************************/
typedef struct tag_AVPlayer_Audio
{
	SDL_AudioSpec st_AudioSource;        //指定的音频参数
	SDL_AudioSpec st_AudioDest;          //输出的音频参数
	unsigned int hAudio;
}AVPLAYER_AUDIO, * LPAVPLAYER_AUDIO;

class CAVPlayer_Audio
{
public:
	CAVPlayer_Audio();
	~CAVPlayer_Audio();
public:
	XHANDLE AVPlayer_Audio_Create(int nSampleRate, int nSampleFmt, int nChannel = 2, int* pInt_ASize = NULL);
	bool AVPlayer_Audio_Push(XHANDLE xhNet, LPCXSTR lpszPCMData, int nPCMSize);
	bool AVPlayer_Audio_Close(XHANDLE xhNet);
	bool AVPlayer_Audio_Pause(XHANDLE xhNet, bool bIsPlayer);
	bool AVPlayer_Audio_GetSize(XHANDLE xhNet, int* pInt_Len);
	bool AVPlayer_Audio_Clear(XHANDLE xhNet);
private:
};
