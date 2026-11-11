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
    bool MSGNotify_SMS_SendTencent(LPCXSTR lpszAPIUrl, LPCXSTR lpszSecretID, LPCXSTR lpszSecretKey, LPCXSTR lpszRegionStr, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, XCHAR*** ppptszPhoneNumber, int nListCount);
    bool MSGNotify_SMS_SendAliyun(LPCXSTR lpszAPIUrl, LPCXSTR lpszSecretID, LPCXSTR lpszSecretKey, LPCXSTR lpszRegionStr, LPCXSTR lpszAPPID, LPCXSTR lpszSignName, LPCXSTR lpszTemplateID, XCHAR*** ppptszPhoneNumber, int nListCount);
protected:
    bool MSGNotify_SMS_UTCTime(time_t nTime, XCHAR* ptszTimeStr);
    bool MSGNotify_SMS_Sha256Hex(LPCXSTR lpszMSGBuffer, XCHAR* ptszMSGBuffer);
    bool MSGNotify_SMS_Sha1Hex(LPCXSTR lpszMSGBuffer, LPCXSTR lpszKeyStr, XCHAR* ptszMSGBuffer);
};
