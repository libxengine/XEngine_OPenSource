#include "pch.h"
#include "AVPlayer_Audio.h"
/********************************************************************
//	Created:	2018/12/12   09:14
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer\AVPlayer_Audio\AVPlayer_Audio.cpp
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer\AVPlayer_Audio
//	File Base:	AVPlayer_Audio
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	音频播放器
//	History:
*********************************************************************/
CAVPlayer_Audio::CAVPlayer_Audio()
{
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
}
CAVPlayer_Audio::~CAVPlayer_Audio()
{
    SDL_Quit();
}
//////////////////////////////////////////////////////////////////////////
//                           公用函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AVPlayer_Audio_Create
函数功能：创建一个音频播放器
 参数.一：nSampleRate
  In/Out：In
  类型：整数型
  可空：N
  意思：音频数据的采样率。常用的有48000,44100等。
 参数.二：nSampleFmt
  In/Out：In
  类型：整数型
  可空：Y
  意思：音频数据的采样格式，参考AVCOLLECT_AUDIOSAMPLEFORMAT（目前支持S16和S32）
 参数.三：nChannel
  In/Out：In
  类型：整数型
  可空：Y
  意思：音频通道数量2为双通道
 参数.四：pInt_ASize
  In/Out：In
  类型：整数型指针
  可空：Y
  意思：输出音频缓冲缓冲区大小,可以根据这个值投递数据
返回值
  类型：句柄型
  意思：返回创建成功的句柄,错误返回NULL
备注：
*********************************************************************/
XHANDLE CAVPlayer_Audio::AVPlayer_Audio_Create(int nSampleRate, int nSampleFmt, int nChannel /* = 2 */, int* pInt_ASize /* = NULL */)
{
    AVPlayer_IsErrorOccur = false;

    //申请独立内存
    AVPLAYER_AUDIO* pSt_AVPlayer = new AVPLAYER_AUDIO;
    if (NULL == pSt_AVPlayer)
    {
        AVPlayer_IsErrorOccur = true;
        AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_AUDIO_CREATE_MALLOC;
        return NULL;
    }
    memset(pSt_AVPlayer, '\0', sizeof(AVPLAYER_AUDIO));

    pSt_AVPlayer->st_AudioSource.freq = nSampleRate;     //音频数据的采样率。常用的有48000,44100等。
    pSt_AVPlayer->st_AudioSource.channels = nChannel;
    pSt_AVPlayer->st_AudioSource.silence = 0;            //设置静音的值
    pSt_AVPlayer->st_AudioSource.samples = 1024;         //音频缓冲区中的采样个数，要求必须是2的n次方
    //设计采样格式
    if (1 == nSampleFmt)
    {
        pSt_AVPlayer->st_AudioSource.format = AUDIO_S16SYS;
    }
    else if (2 == nSampleFmt)
    {
        pSt_AVPlayer->st_AudioSource.format = AUDIO_S32SYS;
    }
    else
    {
        AVPlayer_IsErrorOccur = true;
        AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_AUDIO_CREATE_FMTNOTSUPPORT;
        return NULL;
    }
    //打开音频设备
    pSt_AVPlayer->hAudio = SDL_OpenAudioDevice(NULL, 0, &pSt_AVPlayer->st_AudioSource, &pSt_AVPlayer->st_AudioDest, 0);
    if (pSt_AVPlayer->hAudio <= 0)
    {
        AVPlayer_IsErrorOccur = true;
        AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_AUDIO_CREATE_OPENAUDIO;
        return NULL;
    }
    if (NULL != pInt_ASize)
    {
        *pInt_ASize = pSt_AVPlayer->st_AudioDest.size;
    }
    SDL_PauseAudioDevice(pSt_AVPlayer->hAudio, 0);   //让播放器播放
    return pSt_AVPlayer;
}
/********************************************************************
函数名称：AVPlayer_Audio_Push
函数功能：压入一段PCM音频数据给播放器并且播放它
 参数.一：xhNet
  In/Out：In
  类型：句柄
  可空：N
  意思：播放器句柄
 参数.二：lpszPCMData
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：PCM音频数据缓冲区
 参数.三：nPCMSize
  In/Out：In
  类型：整数型
  可空：N
  意思：音频数据缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAVPlayer_Audio::AVPlayer_Audio_Push(XHANDLE xhNet, LPCXSTR lpszPCMData, int nPCMSize)
{
    AVPlayer_IsErrorOccur = false;

    AVPLAYER_AUDIO* pSt_PlayAudio = (AVPLAYER_AUDIO*)xhNet;
    if (NULL == pSt_PlayAudio)
    {
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_AUDIO_PUSH_NOTFOUND;
		return false;
    }
    SDL_QueueAudio(pSt_PlayAudio->hAudio, lpszPCMData, nPCMSize);
    return true;
}
/********************************************************************
函数名称：AVPlayer_Audio_Close
函数功能：关闭一个音频播放器
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
bool CAVPlayer_Audio::AVPlayer_Audio_Close(XHANDLE xhNet)
{
    AVPlayer_IsErrorOccur = false;

	AVPLAYER_AUDIO* pSt_PlayAudio = (AVPLAYER_AUDIO*)xhNet;
	if (NULL != pSt_PlayAudio)
	{
		SDL_ClearQueuedAudio(pSt_PlayAudio->hAudio);
		SDL_CloseAudioDevice(pSt_PlayAudio->hAudio);

		delete pSt_PlayAudio;
		pSt_PlayAudio = NULL;
	}
    return true;
}
/********************************************************************
函数名称：AVPlayer_Audio_Pause
函数功能：暂停或者继续播放音频
 参数.一：xhNet
  In/Out：In
  类型：句柄
  可空：N
  意思：播放器句柄
 参数.二：bIsPlayer
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：为真为继续，为假为暂停
返回值
  类型：逻辑型
  意思：是否成功
备注：此函数将操作音频设备播放和暂停
*********************************************************************/
bool CAVPlayer_Audio::AVPlayer_Audio_Pause(XHANDLE xhNet, bool bIsPlayer)
{
    AVPlayer_IsErrorOccur = false;

	AVPLAYER_AUDIO* pSt_PlayAudio = (AVPLAYER_AUDIO*)xhNet;
	if (NULL == pSt_PlayAudio)
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_AUDIO_PAUSE_NOTFOUND;
		return false;
	}
    if (bIsPlayer)
    {
        SDL_PauseAudioDevice(pSt_PlayAudio->hAudio, 0);
    }
    else
    {
        SDL_PauseAudioDevice(pSt_PlayAudio->hAudio, 1);
    }
    return true;
}
/********************************************************************
函数名称：AVPlayer_Audio_GetSize
函数功能：获取音频设备缓冲区剩余待播放的数据量
 参数.一：xhNet
  In/Out：In
  类型：句柄
  可空：N
  意思：播放器句柄
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出剩余的播放数据大小（字节）
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAVPlayer_Audio::AVPlayer_Audio_GetSize(XHANDLE xhNet, int* pInt_Len)
{
    AVPlayer_IsErrorOccur = false;

	AVPLAYER_AUDIO* pSt_PlayAudio = (AVPLAYER_AUDIO*)xhNet;
	if (NULL == pSt_PlayAudio)
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_AUDIO_GETSIZE_NOTFOUND;
		return false;
	}
    if (NULL == pInt_Len)
    {
        AVPlayer_IsErrorOccur = true;
        AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_AUDIO_GETSIZE_PARAMENT;
        return false;
    }
    *pInt_Len = SDL_GetQueuedAudioSize(pSt_PlayAudio->hAudio);
    return true;
}
/********************************************************************
函数名称：AVPlayer_Audio_Clear
函数功能：清空播放缓冲区剩余数据
 参数.一：xhNet
  In/Out：In
  类型：句柄
  可空：N
  意思：播放器句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAVPlayer_Audio::AVPlayer_Audio_Clear(XHANDLE xhNet)
{
    AVPlayer_IsErrorOccur = false;

	AVPLAYER_AUDIO* pSt_PlayAudio = (AVPLAYER_AUDIO*)xhNet;
	if (NULL == pSt_PlayAudio)
	{
		AVPlayer_IsErrorOccur = true;
		AVPlayer_dwErrorCode = ERROR_XENGINE_AVCODEC_AVPLAYER_AUDIO_CLEAR_NOTFOUND;
		return false;
	}
    SDL_ClearQueuedAudio(pSt_PlayAudio->hAudio);
    return true;
}