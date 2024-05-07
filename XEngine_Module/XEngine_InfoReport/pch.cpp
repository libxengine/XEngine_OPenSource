#include "pch.h"
#include "InfoReport_APIMachine/InfoReport_APIMachine.h"
/********************************************************************
//    Created:     2024/04/28  17:21:51
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_InfoReport\pch.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_InfoReport
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     信息报告导出实现
//    History:
*********************************************************************/
bool InfoReport_IsErrorOccur = false;
XLONG InfoReport_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CInfoReport_APIMachine m_APIMachine;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG InfoReport_GetLastError(int* pInt_ErrorCode)
{
	if (NULL != pInt_ErrorCode)
	{
		*pInt_ErrorCode = errno;
	}
	return InfoReport_dwErrorCode;
}
/************************************************************************/
/*                        APIMachine报告                                */
/************************************************************************/
extern "C" bool InfoReport_APIMachine_Send(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName)
{
	return m_APIMachine.InfoReport_APIMachine_Send(lpszAPIUrl, lpszServiceName);
}
extern "C" bool InfoReport_APIMachine_GetTime(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName, __int64x * pInt_TimeNumber)
{
	return m_APIMachine.InfoReport_APIMachine_GetTime(lpszAPIUrl, lpszServiceName, pInt_TimeNumber);
}