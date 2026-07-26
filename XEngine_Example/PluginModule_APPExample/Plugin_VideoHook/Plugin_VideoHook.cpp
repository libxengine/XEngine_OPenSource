#include "pch.h"
#include "Plugin_VideoHook.h"
/********************************************************************
//    Created:     2022/04/21  15:52:19
//    File Name:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook\Plugin_VideoHook\Plugin_VideoHook.cpp
//    File Path:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook\Plugin_VideoHook
//    File Base:   Plugin_VideoHook
//    File Ext:    cpp
//    Project:     XEngine_AVCodecApp (AV 编解码插件示例)
//    Author:      qyt
//    Purpose:     视频钩子插件实现
//    History:
*********************************************************************/
CPlugin_VideoHook::CPlugin_VideoHook()
{
}
CPlugin_VideoHook::~CPlugin_VideoHook()
{
}
//////////////////////////////////////////////////////////////////////////
//                       公有函数
//////////////////////////////////////////////////////////////////////////
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
bool CPlugin_VideoHook::PluginCore_Init(XENGINE_PLUGINPARAM *pSt_PluginParameter)
{
	VideoHook_IsErrorOccur = false;

    return true;
}
/********************************************************************
函数名称：PluginCore_UnInit
函数功能：卸载插件
返回值
  类型：无
  意思：
备注：
*********************************************************************/
void CPlugin_VideoHook::PluginCore_UnInit()
{
	VideoHook_IsErrorOccur = false;
}
/********************************************************************
函数名称：PluginCore_RegisterType
函数功能：注册插件类型
返回值
  类型：整数
  意思：插件类型,0无效,1视频,2音频
备注：
*********************************************************************/
int CPlugin_VideoHook::PluginCore_RegisterType()
{
	VideoHook_IsErrorOccur = false;
	return 1;
}
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
void CPlugin_VideoHook::PluginCore_GetInfo(XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc)
{
	VideoHook_IsErrorOccur = false;
	
	_tcsxcpy(ptszPluginName, "videohook");
	_tcsxcpy(ptszPluginVersion, "1.0.0.1001");
	_tcsxcpy(ptszPluginAuthor, "xengine");
	_tcsxcpy(ptszPluginDesc, "Video Hook Plugin for XEngine API Service");
}
/********************************************************************
函数名称：PluginCore_Call
函数功能：调用插件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CPlugin_VideoHook::PluginCore_Call(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLen, XCHAR*** pppInputParameters, int nInputPCount, XCHAR*** pppOutputParameters, int* pInt_OutputPCount)
{
	VideoHook_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		VideoHook_IsErrorOccur = true;
		VideoHook_dwErrorCode = ERROR_AVCODEC_PLUGIN_MODULE_VIDEOHOOK_PARAMENT;
		return false;
	}
	for (int i = 0; i < nInputPCount; i++)
	{
		_xtprintf(_X("Input Parameter %d: %s\r\n"), i + 1, (*pppInputParameters)[i]);
	}
	_xtprintf(_X("%d\r\n"), *pInt_MsgLen);
	return true;
}
bool CPlugin_VideoHook::PluginCore_Call2(XHANDLE phBuffer)
{
	VideoHook_IsErrorOccur = false;

	if (NULL == phBuffer)
	{
		VideoHook_IsErrorOccur = true;
		VideoHook_dwErrorCode = ERROR_AVCODEC_PLUGIN_MODULE_VIDEOHOOK_PARAMENT;
		return false;
	}
	XENGINE_PLUGINBUFFER* pSt_PluginBuffer = (XENGINE_PLUGINBUFFER*)phBuffer;

	pSt_PluginBuffer->nUserNumber = _tcsxlen(_X("VideoHook Plugin Call2 Success"));
	_xstprintf(pSt_PluginBuffer->tszUserData, _X("%s"), _X("VideoHook Plugin Call2 Success"));
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                       保护函数
//////////////////////////////////////////////////////////////////////////