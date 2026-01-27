#pragma once
/********************************************************************
//    Created:     2026/01/27  15:38:17
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\PluginExtension_Error.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension
//    File Base:   PluginExtension_Error
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     插件核心错误导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       错误定义
//////////////////////////////////////////////////////////////////////////
#define ERROR_XENGINE_THIRDPART_PLUGIN_PARAMENT 0x01E0001          //参数错误
#define ERROR_XENGINE_THIRDPART_PLUGIN_OPENDL 0x01E0002            //打开模块插件失败
#define ERROR_XENGINE_THIRDPART_PLUGIN_FPINIT 0x01E0003            //查找初始化函数失败
#define ERROR_XENGINE_THIRDPART_PLUGIN_FPUNINIT 0x01E0004          //查找卸载函数失败
#define ERROR_XENGINE_THIRDPART_PLUGIN_FPCALL 0x01E0005            //查找调用执行功能函数失败
#define ERROR_XENGINE_THIRDPART_PLUGIN_FPERROR 0x01E0006           //获取错误函数失败
#define ERROR_XENGINE_THIRDPART_PLUGIN_INIT 0x01E0007              //初始化内部模块失败
#define ERROR_XENGINE_THIRDPART_PLUGIN_NOTFOUND 0x01E0008          //没有找到句柄
#define ERROR_XENGINE_THIRDPART_PLUGIN_ISINITED 0x01E0009          //已经初始化了，不需要再次初始化
#define ERROR_XENGINE_THIRDPART_PLUGIN_EXECTION 0x01E000A          //插件执行失败
#define ERROR_XENGINE_THIRDPART_PLUGIN_MALLOC 0x01E000B            //申请内存失败
#define ERROR_XENGINE_THIRDPART_PLUGIN_GETINFO 0x01E000C           //获取插件信息失败