#pragma once
/********************************************************************
//	Created:	2018/10/8   14:49
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer\AVPlayer_Video\AVPlayer_Video.h
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer\AVPlayer_Video
//	File Base:	AVPlayer_Video
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	视频播放器
//	History:
*********************************************************************/
typedef struct tag_AVPlayer_Video
{
	SDL_Window* pSt_SDLScreen;
	SDL_Renderer* pSt_SDLRender;
	SDL_Texture* pSt_SDLTexture;
	SDL_Rect st_SDLRect;
}AVPLAYER_VIDEO, * LPAVPLAYER_VIDEO;

class CAVPlayer_Video
{
public:
	CAVPlayer_Video();
	~CAVPlayer_Video();
public:
	XHANDLE AVPlayer_Video_Create(XPVOID lPWindowFrom, LPCXSTR lpszWindowName = NULL, int nWidth = 0, int nHeight = 0, int nXPos = 0, int nYPos = 0);
	bool AVPlayer_Video_Push(XHANDLE xhNet, uint8_t* pszYUVData, int nLineSize);
	bool AVPlayer_Video_PushData(XHANDLE xhNet, uint8_t* pszYData, int nYSize, uint8_t* pszUData, int nUSize, uint8_t* pszVData, int nVSize);
	bool AVPlayer_Video_SetWindows(XHANDLE xhNet, int nWidth, int nHeight);
	bool AVPlayer_Video_Close(XHANDLE xhNet);
protected:
private:
};
