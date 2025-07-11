#pragma once
/********************************************************************
//    Created:     2025/05/29  14:02:53
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Error.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi
//    File Base:   AIApi_Error
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     AI接口导出错误
//    History:
*********************************************************************/
/************************************************************************/
/*                         AI对话模型接口错误                           */
/************************************************************************/
#define ERROR_XENGINE_MODULE_AIAPI_CHAT_PARAMENT 0x01D0001               //参数错误，无法继续
#define ERROR_XENGINE_MODULE_AIAPI_CHAT_MALLOC 0x01D0002                 //申请内存失败
#define ERROR_XENGINE_MODULE_AIAPI_CHAT_NOTFOUND 0x01D0003               //没有找到
/************************************************************************/
/*                         AI图像模型接口错误                           */
/************************************************************************/
#define ERROR_XENGINE_MODULE_AIAPI_IMAGE_PARAMENT 0x01D1001               //参数错误，无法继续
#define ERROR_XENGINE_MODULE_AIAPI_IMAGE_MALLOC 0x01D1002                 //申请内存失败
#define ERROR_XENGINE_MODULE_AIAPI_IMAGE_NOTFOUND 0x01D1003               //没有找到
#define ERROR_XENGINE_MODULE_AIAPI_IMAGE_PARSE 0x01D1004                  //JSON解析错误
#define ERROR_XENGINE_MODULE_AIAPI_IMAGE_FORMAT 0x01D1005                 //JSON格式错误
/************************************************************************/
/*                         AI帮助接口错误                               */
/************************************************************************/
#define ERROR_XENGINE_MODULE_AIAPI_HELP_PARAMENT 0x01D2001                //参数错误，无法继续
#define ERROR_XENGINE_MODULE_AIAPI_HELP_MALLOC 0x01D2002                  //申请内存失败
#define ERROR_XENGINE_MODULE_AIAPI_HELP_OPENFILE 0x01D2003                //打开文件失败
#define ERROR_XENGINE_MODULE_AIAPI_HELP_READFILE 0x01D2004                //读取文件失败