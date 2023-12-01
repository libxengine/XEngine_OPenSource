#include "pch.h"
#include "AVPlayer_Video/AVPlayer_Video.h"
#include "AVPlayer_Audio/AVPlayer_Audio.h"
/********************************************************************
//	Created:	2018/10/8   15:56
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer\AVPlayer_DllMain.cpp
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_AvCoder\NetEngine_AVPlayer
//	File Base:	AVPlayer_DllMain
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	播放器导出函数实现
//	History:
*********************************************************************/
bool AVPlayer_IsErrorOccur = false;
XLONG AVPlayer_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAVPlayer_Video m_PlayerVideo;
CAVPlayer_Audio m_PlayerAudio;
//////////////////////////////////////////////////////////////////////////
//                     导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG AVPlayer_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return AVPlayer_dwErrorCode;
}
/************************************************************************/
/*                     视频播放器导出函数                               */
/************************************************************************/
extern "C" XHANDLE AVPlayer_Video_Create(XPVOID lPWindowFrom, LPCXSTR lpszWindowName, int nWidth, int nHeight, int nXPos, int nYPos)
{
	return m_PlayerVideo.AVPlayer_Video_Create(lPWindowFrom, lpszWindowName, nWidth, nHeight, nXPos, nYPos);
}
extern "C" bool AVPlayer_Video_Push(XHANDLE xhNet, uint8_t * pszYUVData, int nLineSize)
{
	return m_PlayerVideo.AVPlayer_Video_Push(xhNet, pszYUVData, nLineSize);
}
extern "C" bool AVPlayer_Video_PushData(XHANDLE xhNet, uint8_t * pszYData, int nYSize, uint8_t * pszUData, int nUSize, uint8_t * pszVData, int nVSize)
{
	return m_PlayerVideo.AVPlayer_Video_PushData(xhNet, pszYData, nYSize, pszUData, nUSize, pszVData, nVSize);
}
extern "C" bool AVPlayer_Video_SetWindows(XHANDLE xhNet, int nWidth, int nHeight)
{
	return m_PlayerVideo.AVPlayer_Video_SetWindows(xhNet, nWidth, nHeight);
}
extern "C" bool AVPlayer_Video_Close(XHANDLE xhNet)
{
	return m_PlayerVideo.AVPlayer_Video_Close(xhNet);
}
/************************************************************************/
/*                     视频播放器导出函数                                  */
/************************************************************************/
extern "C" XHANDLE AVPlayer_Audio_Create(int nSampleRate, int nSampleFmt, int nChannel, int* pInt_ASize)
{
	return m_PlayerAudio.AVPlayer_Audio_Create(nSampleRate, nSampleFmt, nChannel, pInt_ASize);
}
extern "C" bool AVPlayer_Audio_Push(XHANDLE xhNet, LPCXSTR lpszPCMData, int nPCMSize)
{
	return m_PlayerAudio.AVPlayer_Audio_Push(xhNet, lpszPCMData, nPCMSize);
}
extern "C" bool AVPlayer_Audio_Close(XHANDLE xhNet)
{
	return m_PlayerAudio.AVPlayer_Audio_Close(xhNet);
}
extern "C" bool AVPlayer_Audio_Pause(XHANDLE xhNet, bool bIsPlayer)
{
	return m_PlayerAudio.AVPlayer_Audio_Pause(xhNet, bIsPlayer);
}
extern "C" bool AVPlayer_Audio_GetSize(XHANDLE xhNet, int* pInt_Len)
{
	return m_PlayerAudio.AVPlayer_Audio_GetSize(xhNet, pInt_Len);
}
extern "C" bool AVPlayer_Audio_Clear(XHANDLE xhNet)
{
	return m_PlayerAudio.AVPlayer_Audio_Clear(xhNet);
}