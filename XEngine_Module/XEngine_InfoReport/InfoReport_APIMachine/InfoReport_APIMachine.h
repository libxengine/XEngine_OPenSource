﻿#pragma once
/********************************************************************
//    Created:     2024/04/28  17:21:30
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_InfoReport\InfoReport_APIMachine\InfoReport_APIMachine.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_InfoReport\InfoReport_APIMachine
//    File Base:   InfoReport_APIMachine
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     信息收集报告类
//    History:
*********************************************************************/

class CInfoReport_APIMachine
{
public:
    CInfoReport_APIMachine();
    ~CInfoReport_APIMachine();
public:
    bool InfoReport_APIMachine_Send(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName, __int64x* pInt_TimeNumber = NULL);
    bool InfoReport_APIMachine_GetTime(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName, __int64x* pInt_TimeNumber);
    bool InfoReport_APIMachine_Delete(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName);
public:
    bool InfoReport_APIMachine_Hardware(XCHAR* ptszSWInfo, int* pInt_Len = NULL);
    bool InfoReport_APIMachine_Software(XCHAR* ptszSWInfo, int* pInt_Len = NULL);
protected:
};
