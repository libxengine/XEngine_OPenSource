#pragma once
/********************************************************************
//    Created:     2026/06/01  00:00:00
//    File Name:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook\PluginVideo_Define.h
//    File Path:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook
//    File Base:   PluginVideo_Define
//    File Ext:    h
//    Project:     XEngine_AVCodecApp
//    Author:      qyt
//    Purpose:     视频钩子插件导出定义
//    History:
*********************************************************************/
typedef struct
{
	XCHAR tszAPIVersion[64];                                              //API服务版本
	XCHAR tszXEngineVer[64];                                              //XEngine版本
}XENGINE_PLUGINPARAM;

typedef struct
{
	XCHAR tszUserData[128];
	int nUserNumber;
}XENGINE_PLUGINBUFFER;
//////////////////////////////////////////////////////////////////////////
//                       导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG PluginCore_GetLastError();
/*********************************************************************************
*                                导出函数定义                                    *
*********************************************************************************/
/********************************************************************
函数名称：PluginCore_Init
函数功能：初始化插件模块
 参数.一：pSt_PluginParameter
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入插件初始化信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool PluginCore_Init(XENGINE_PLUGINPARAM* pSt_PluginParameter = NULL);
/********************************************************************
函数名称：PluginCore_UnInit
函数功能：卸载插件
返回值
  类型：无
  意思：
备注：
*********************************************************************/
extern "C" void PluginCore_UnInit();
/********************************************************************
函数名称：PluginCore_RegisterType
函数功能：注册插件类型
返回值
  类型：整数
  意思：插件类型,0无效,1视频,2音频
备注：
*********************************************************************/
extern "C" int PluginCore_RegisterType();
/********************************************************************
函数名称：PluginCore_GetInfo
函数功能：获取插件基础信息函数
 参数.一：ptszPluginName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：处理名称
 参数.二：ptszPluginVersion
  In/Out：Out
  类型：字符指针
  可空：N
  意思：版本号.使用x.x.x.x 格式
 参数.三：ptszPluginAuthor
  In/Out：Out
  类型：字符指针
  可空：N
  意思：作者
 参数.四：ptszPluginDesc
  In/Out：Out
  类型：字符指针
  可空：N
  意思：插件描述
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" void PluginCore_GetInfo(XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc);
/********************************************************************
函数名称：PluginCore_Call
函数功能：调用插件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool PluginCore_Call(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer = NULL, int nMsgLen = 0, XCHAR*** pppInputParameters = NULL, int nInputPCount = 0, XCHAR*** pppOutputParameters = NULL, int* pInt_OutputPCount = NULL);
extern "C" bool PluginCore_Call2(XHANDLE phBuffer);