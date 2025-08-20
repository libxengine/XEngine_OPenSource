#pragma once
/********************************************************************
//    Created:     2025/05/21  13:42:30
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_TokenSession\TokenSession_Error.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_TokenSession
//    File Base:   TokenSession_Error
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     TOKEN错误码
//    History:
*********************************************************************/
/************************************************************************/
/*                         TOKEN会话错误                                */
/************************************************************************/
#define ERROR_XENGINE_MODULE_SESSION_TOKEN_PARAMENT 0x01B0001             //参数错误，无法继续
#define ERROR_XENGINE_MODULE_SESSION_TOKEN_THREAD 0x0020001               //创建线程失败
#define ERROR_XENGINE_MODULE_SESSION_TOKEN_NOTFOUND 0x0020002             //没有找到客户端
#define ERROR_XENGINE_MODULE_SESSION_TOKEN_EXIST 0x0020003                //没有找到客户端
#define ERROR_XENGINE_MODULE_SESSION_TOKEN_TOKEN 0x0020004                //TOKEN错误,令牌不一致
/************************************************************************/
/*                         TOKEN会话错误                                */
/************************************************************************/
#define ERROR_XENGINE_MODULE_SESSION_DYNAMIC_THREAD 0x01B1001             //创建线程失败
#define ERROR_XENGINE_MODULE_SESSION_DYNAMIC_PARAMENT 0x01B1002           //参数错误
#define ERROR_XENGINE_MODULE_SESSION_DYNAMIC_NOTFOUND 0x01B1003           //没有找到
#define ERROR_XENGINE_MODULE_SESSION_DYNAMIC_EXIST 0x01B1004              //TOKEN已存在