#pragma once
/********************************************************************
//    Created:     2025/08/18  16:56:40
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_Error.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification
//    File Base:   Verification_Error
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     通用验证模块
//    History:
*********************************************************************/
/************************************************************************/
/*                         HTTP验证错误                                */
/************************************************************************/
#define ERROR_XENGINE_MODULE_VERIFICATION_HTTP_PARAMENT 0x01E0001         //参数错误，无法继续
#define ERROR_XENGINE_MODULE_VERIFICATION_HTTP_NOTSUPPORT 0x01E0002       //不支持
#define ERROR_XENGINE_MODULE_VERIFICATION_HTTP_NONCE 0x01E0003            //Nonce错误
#define ERROR_XENGINE_MODULE_VERIFICATION_HTTP_OPAQUE 0x01E0004           //Opaque错误
#define ERROR_XENGINE_MODULE_VERIFICATION_HTTP_RESULT 0x01E0005           //验证结果错误
/************************************************************************/
/*                         OAUTH验证错误                                */
/************************************************************************/
#define ERROR_XENGINE_MODULE_VERIFICATION_OAUTH_PARAMENT 0x01E1001        //参数错误，无法继续
/************************************************************************/
/*                         XAUTH验证错误                                */
/************************************************************************/
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT 0x01E2001        //参数错误，无法继续
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_NOTSUPPORT 0x01E2002      //不支持此生成类型
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_UNKNOW 0x01E2003          //获取类型失败，这个类型未知
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_SERIAL 0x01E2004          //硬件序列号验证失败
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_MODETYPE 0x01E2005        //不支持的验证模式
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_HWTYPE 0x01E2006          //不支持的硬件类型
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_OPENFILE 0x01E2007        //文件不存在
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_TIMELIST 0x01E2008        //不存在时间列表
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_MALLOC 0x01E2009          //申请内存失败
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_READ 0x01E2010            //读取文件失败
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_FAILED 0x01E2011          //序列号验证失败,输入的序列号错误
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE 0x01E2012           //解析数据失败
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE 0x01E2013            //服务器返回的CODE错误
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_THREAD 0x01E2014          //创建线程失败
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_NOTAUTH 0x01E2015         //没有验证无法继续
#define ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_NOLOGIN 0x01E2016         //没有登录无法继续