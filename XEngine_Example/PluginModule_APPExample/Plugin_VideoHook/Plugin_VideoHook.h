#pragma once
/********************************************************************
//    Created:     2026/06/01  11:00:00
//    File Name:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook\Plugin_VideoHook\Plugin_VideoHook.h
//    File Path:   D:\XEngine_AVCodecApp\AVCodec_PluginExample\PluginModule_VideoHook\Plugin_VideoHook
//    File Base:   Plugin_VideoHook
//    File Ext:    h
//    Project:     XEngine_AVCodecApp (AV 编解码插件示例)
//    Author:      qyt
//    Purpose:     视频钩子插件头文件
//    History:
*********************************************************************/

class CPlugin_VideoHook
{
public:
	CPlugin_VideoHook();
	~CPlugin_VideoHook();
public:
	bool PluginCore_Init(XENGINE_PLUGINPARAM *pSt_PluginParameter);
	void PluginCore_UnInit();
	int PluginCore_RegisterType();
	void PluginCore_GetInfo(XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc);
	bool PluginCore_Call(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer = NULL, int nMsgLen = 0, XCHAR*** pppInputParameters = NULL, int nInputPCount = 0, XCHAR*** pppOutputParameters = NULL, int* pInt_OutputPCount = NULL);
	bool PluginCore_Call2(XHANDLE phBuffer);
protected:
	
private:
};
