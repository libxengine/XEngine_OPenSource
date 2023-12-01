#include "pch.h"
#include "AVPlayer_Video.h"
/********************************************************************
//	Created:	2018/10/8   14:55
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer\AVPlayer_Video\AVPlayer_Video.cpp
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer\AVPlayer_Video
//	File Base:	AVPlayer_Video
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	视频播放器
//	History:
*********************************************************************/
CAVPlayer_Video::CAVPlayer_Video()
{
}
CAVPlayer_Video::~CAVPlayer_Video()
{
}
//////////////////////////////////////////////////////////////////////////
//                           公用函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AVPlayer_Video_Create
函数功能：创建一个播放器窗口
 参数.一：lPWindowFrom
  In/Out：In
  类型：无类型指针
  可空：N
  意思：输入窗口句柄
 参数.二：lpszWindowName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入窗口名称
 参数.三：nWidth
  In/Out：In
  类型：整数型
  可空：Y
  意思：指定显示窗口宽度
 参数.四：nHeight
  In/Out：In
  类型：整数型
  可空：Y
  意思：指定显示窗口高度
 参数.五：nXPos
  In/Out：In
  类型：整数型
  可空：Y
  意思：窗口所在屏幕X的坐标
 参数.六：nYPos
  In/Out：In
  类型：整数型
  可空：Y
  意思：窗口所在屏幕Y的坐标
返回值
  类型：句柄型
  意思：成功返回创建的句柄,失败返回NULL
备注：lPWindowFrom参数可用的时候,最后两个参数将无效.lPWindowFrom可以附在指定窗口句柄
      lpszWindowName创建单独窗口的时候,你的父进程如果是窗口,那么将崩溃
*********************************************************************/
XHANDLE CAVPlayer_Video::AVPlayer_Video_Create(XPVOID lPWindowFrom, LPCXSTR lpszWindowName /* = NULL */, int nWidth /* = 0 */, int nHeight /* = 0 */, int nXPos /* = 0 */, int nYPos /* = 0 */)
{
    AVPlayer_IsErrorOccur = false;

    if ((NULL != lpszWindowName) && (NULL != lPWindowFrom))
    {
        AVPlayer_IsErrorOccur = true;
        AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_CREATE_PARAMENT;
        return NULL;
    }
    //申请独立内存
    AVPLAYER_VIDEO* pSt_AVPlayer = new AVPLAYER_VIDEO;
    if (NULL == pSt_AVPlayer)
    {
        AVPlayer_IsErrorOccur = true;
        AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_CREATE_MALLOC;
        return NULL;
    }
    memset(pSt_AVPlayer, '\0', sizeof(AVPLAYER_VIDEO));

	SDL_EventState(SDL_WINDOWEVENT, SDL_IGNORE);
	//SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, nullptr);
    //创建一个窗口
    if (NULL == lpszWindowName)
    {
        pSt_AVPlayer->pSt_SDLScreen = SDL_CreateWindowFrom(lPWindowFrom);
    }
    else
    {
        pSt_AVPlayer->pSt_SDLScreen = SDL_CreateWindow(lpszWindowName, nXPos, nYPos, nWidth, nHeight, SDL_WINDOW_OPENGL);
    }
    if (NULL == pSt_AVPlayer->pSt_SDLScreen)
    {
        AVPlayer_IsErrorOccur = true;
        AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_CREATE_WINDOW;
        return NULL;
    }
    //为窗口创建一个2D渲染上下文
    pSt_AVPlayer->pSt_SDLRender = SDL_CreateRenderer(pSt_AVPlayer->pSt_SDLScreen, -1, 0);
    if (NULL == pSt_AVPlayer->pSt_SDLRender)
    {
        AVPlayer_IsErrorOccur = true;
        AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_CREATE_RENDER;
        return NULL;
    }
    //简要的为渲染上下文创建纹理
    pSt_AVPlayer->pSt_SDLTexture = SDL_CreateTexture(pSt_AVPlayer->pSt_SDLRender, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, nWidth, nHeight);
    if (NULL == pSt_AVPlayer->pSt_SDLTexture)
    {
        AVPlayer_IsErrorOccur = true;
        AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_CREATE_TEXTURE;
        return NULL;
    }
    pSt_AVPlayer->st_SDLRect.x = nXPos;
    pSt_AVPlayer->st_SDLRect.y = nYPos;
    pSt_AVPlayer->st_SDLRect.w = nWidth;
    pSt_AVPlayer->st_SDLRect.h = nHeight;
    return pSt_AVPlayer;
}
/********************************************************************
函数名称：AVPlayer_Video_Push
函数功能：压入一段YUV数据给播放器并且显示它
 参数.一：xhNet
  In/Out：In
  类型：句柄
  可空：N
  意思：要插入的到的播放器句柄
 参数.二：pszYUVData
  In/Out：In
  类型：无符号整数型指针
  可空：N
  意思：YUV缓冲区
 参数.三：nLineSize
  In/Out：In
  类型：整数型
  可空：N
  意思：输入视频宽
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAVPlayer_Video::AVPlayer_Video_Push(XHANDLE xhNet, uint8_t* pszYUVData, int nLineSize)
{
    AVPlayer_IsErrorOccur = false;

    AVPLAYER_VIDEO* pSt_PlayVideo = (AVPLAYER_VIDEO*)xhNet;
    if (NULL == pSt_PlayVideo)
    {
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_PUSH_NOTFOUND;
		return false;
    }
	//准备更新的数据
	if (-1 == SDL_UpdateTexture(pSt_PlayVideo->pSt_SDLTexture, &pSt_PlayVideo->st_SDLRect, pszYUVData, nLineSize))
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_PUSH_UPDATA;
		return false;
	}
	//清理数据
	if (-1 == SDL_RenderClear(pSt_PlayVideo->pSt_SDLRender))
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_PUSH_CLEAR;
		return false;
	}
	if (-1 == SDL_RenderCopy(pSt_PlayVideo->pSt_SDLRender, pSt_PlayVideo->pSt_SDLTexture, NULL, &pSt_PlayVideo->st_SDLRect))
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_PUSH_COPY;
		return false;
	}
	//简要地显示屏幕进行渲染
	SDL_RenderPresent(pSt_PlayVideo->pSt_SDLRender);
    return true;
}
/********************************************************************
函数名称：AVPlayer_Video_PushData
函数功能：压入一段YUV数据给播放器并且显示它
 参数.一：xhNet
  In/Out：In
  类型：句柄
  可空：N
  意思：要插入的到的播放器句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：功能同AVPlayer_Video_Push,只是YUV数据分开压入
*********************************************************************/
bool CAVPlayer_Video::AVPlayer_Video_PushData(XHANDLE xhNet, uint8_t* pszYData, int nYSize, uint8_t* pszUData, int nUSize, uint8_t* pszVData, int nVSize)
{
	AVPlayer_IsErrorOccur = false;

	AVPLAYER_VIDEO* pSt_PlayVideo = (AVPLAYER_VIDEO*)xhNet;
	if (NULL == pSt_PlayVideo)
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_PUSH_NOTFOUND;
		return false;
	}
	//准备更新的数据
	if (-1 == SDL_UpdateYUVTexture(pSt_PlayVideo->pSt_SDLTexture, NULL, pszYData, nYSize, pszUData, nUSize, pszVData, nVSize))
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_PUSH_UPDATA;
		return false;
	}
	//清理数据
	if (-1 == SDL_RenderClear(pSt_PlayVideo->pSt_SDLRender))
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_PUSH_CLEAR;
		return false;
	}
	if (-1 == SDL_RenderCopy(pSt_PlayVideo->pSt_SDLRender, pSt_PlayVideo->pSt_SDLTexture, NULL, &pSt_PlayVideo->st_SDLRect))
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_PUSH_COPY;
		return false;
	}
	//简要地显示屏幕进行渲染
	SDL_RenderPresent(pSt_PlayVideo->pSt_SDLRender);
	return true;
}
/********************************************************************
函数名称：AVPlayer_Video_SetWindows
函数功能：设置窗口大小
 参数.一：xhNet
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的窗口
 参数.二：nWidth
  In/Out：In
  类型：整数型
  可空：N
  意思：输入调整的宽
 参数.三：nHeight
  In/Out：In
  类型：整数型
  可空：N
  意思：输入调整的高
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAVPlayer_Video::AVPlayer_Video_SetWindows(XHANDLE xhNet, int nWidth, int nHeight)
{
	AVPlayer_IsErrorOccur = false;

	AVPLAYER_VIDEO* pSt_PlayVideo = (AVPLAYER_VIDEO*)xhNet;
	if (NULL == pSt_PlayVideo)
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_VIDEO_PUSH_NOTFOUND;
		return false;
	}
	SDL_SetWindowSize(pSt_PlayVideo->pSt_SDLScreen, nWidth, nHeight);
	return true;
}
/********************************************************************
函数名称：AVPlayer_Video_Close
函数功能：关闭一个视频播放器
 参数.一：xhNet
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的播放器句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAVPlayer_Video::AVPlayer_Video_Close(XHANDLE xhNet)
{
    AVPlayer_IsErrorOccur = false;

	AVPLAYER_VIDEO* pSt_PlayVideo = (AVPLAYER_VIDEO*)xhNet;
	if (NULL != pSt_PlayVideo)
	{
		SDL_DestroyTexture(pSt_PlayVideo->pSt_SDLTexture);
		SDL_DestroyRenderer(pSt_PlayVideo->pSt_SDLRender);
		SDL_DestroyWindow(pSt_PlayVideo->pSt_SDLScreen);

		delete pSt_PlayVideo;
		pSt_PlayVideo = NULL;
	}
    return true;
}