#pragma once
/********************************************************************
//    Created:     2025/05/20  14:28:33
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\MSGNotify_SMS\MSGNotify_SMS.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\MSGNotify_SMS
//    File Base:   MSGNotify_SMS
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     短信通知
//    History:
*********************************************************************/

class CMSGNotify_SMS
{
public:
    CMSGNotify_SMS();
    ~CMSGNotify_SMS();
public:
    bool MSGNotify_SMS_SendTencent(LPCXSTR lpszServiceAddr, LPCXSTR lpszRegion, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, LPCXSTR lpszPhoneNumber, LPCXSTR lpszMSGBuffer = NULL);
    bool MSGNotify_SMS_SendAliyun(LPCXSTR lpszServiceAddr, LPCXSTR lpszRegion, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, LPCXSTR lpszPhoneNumber, LPCXSTR lpszMSGBuffer = NULL);
};
