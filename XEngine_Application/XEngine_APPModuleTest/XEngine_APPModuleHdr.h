#pragma once
/********************************************************************
//    Created:     2024/09/09  16:24:15
//    File Name:   D:\XEngine_OPenSource\XEngine_Application\XEngine_APPModuleTest\XEngine_APPModuleHdr.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Application\XEngine_APPModuleTest
//    File Base:   XEngine_APPModuleHdr
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     公用头文件
//    History:
*********************************************************************/
//加载系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
using namespace std;
//加载XEngine头文件
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
//基础库
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_BaseLib/BaseSafe_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseSafe_Error.h>
#include <XEngine_Include/XEngine_BaseLib/Algorithm_Define.h>
#include <XEngine_Include/XEngine_BaseLib/Algorithm_Error.h>
//核心库
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_Core/Protocol_Define.h>
#include <XEngine_Include/XEngine_Core/Protocol_Error.h>
//客户端
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_Client/StreamClient_Define.h>
#include <XEngine_Include/XEngine_Client/StreamClient_Error.h>
//系统组件
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
//网络组件
#include <XEngine_Include/XEngine_NetHelp/APIAddr_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIAddr_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIFlow_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIFlow_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_NetHelp/XSocket_Define.h>
#include <XEngine_Include/XEngine_NetHelp/XSocket_Error.h>
//帮助模块
#include <XEngine_Include/XEngine_HelpComponents/BINPack_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/BINPack_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/HelpCompress_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/HelpCompress_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/WBlackList_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/WBlackList_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
//RFC组件
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/MQTTProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/MQTTProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/NatProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/NatProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/NTPProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/NTPProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/SIPProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/SIPProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/SnmpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/SnmpProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/SSDPProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/SSDPProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Error.h>
//流媒体协议组件
#include <XEngine_Include/XEngine_StreamMedia/FLVProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/FLVProtocol_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/HLSProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/HLSProtocol_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/MP4Protocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/MP4Protocol_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTCPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTCPProtocol_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTMPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTMPProtocol_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTPProtocol_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTSPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTSPProtocol_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Error.h>
//音视频编解码组件
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFilter_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFilter_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Error.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>

//链接库
#ifdef _MSC_BUILD
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseSafe")
#pragma comment(lib,"XEngine_BaseLib/XEngine_Algorithm")

#pragma comment(lib,"XEngine_Core/XEngine_Core")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption")
#pragma comment(lib,"XEngine_Core/XEngine_Protocol")

#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#pragma comment(lib,"XEngine_Client/XClient_Stream")

#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi")

#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIAddr")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIFlow")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_XSocket")

#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_BINPack")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Compress")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_DataBase")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_WBlackList")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog")

#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpProtocol")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_MQTTProtocol")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_NatProtocol")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_NTPProtocol")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_ProxyProtocol")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_SIPPorotocol")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_SnmpProtocol")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_SSDPProtocol")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_WSProtocol")

#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_FLVProtocol")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_HLSProtocol")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_MP4Protocol")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTCPProtocol")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTMPProtocol")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTPProtocol")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTSPProtocol")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_SDPProtocol")

#pragma comment(lib,"XEngine_AVCodec/XEngine_AudioCodec")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVCollect")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFilter")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFormat")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFrame")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp")
#pragma comment(lib,"XEngine_AVCodec/XEngine_VideoCodec")
#endif