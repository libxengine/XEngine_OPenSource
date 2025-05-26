#pragma once
/********************************************************************
//    Created:     2025/05/20  14:17:37
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\MSGNotify_EMail\MSGNotify_EMail.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_MSGNotify\MSGNotify_EMail
//    File Base:   MSGNotify_EMail
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     电子邮件通知
//    History:
*********************************************************************/

class CMSGNotify_EMail
{
public:
    CMSGNotify_EMail();
    ~CMSGNotify_EMail();
public:
    bool MSGNotify_EMail_Send(LPCXSTR lpszServiceAddr, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszSendAddr, LPCXSTR lpszSubJect, LPCXSTR lpszPayLoad);
protected:
};
