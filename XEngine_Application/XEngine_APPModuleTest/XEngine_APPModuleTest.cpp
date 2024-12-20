#include "XEngine_APPModuleHdr.h"

int main()
{
#ifdef _MSC_BUILD
    WSADATA st_WSAData;
    WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
    XLONG nMSGLen = 0;
    XSOCKET hSocket = 0;
    XHANDLE xhToken = NULL;
    XNETHANDLE xhClient = 0;
    XCHAR tszMSGBuffer[2048] = {};

    _xtprintf(_X("XEngine_Client Test:\n"));
    _xtprintf(_X("XEngine_BaseLib-BaseLib_Version_XNumberStr:%s\n"), BaseLib_Version_XNumberStr());
    _xtprintf(_X("XEngine_BaseSafe-BaseSafe_String_Strcopy:%ld\n"), BaseSafe_String_Strcopy(tszMSGBuffer, _X("123"), 3));
    _xtprintf(_X("XEngine_Algorithm-Algorithm_String_HEXToInt:%d\n"), Algorithm_String_HEXToInt(tszMSGBuffer));

    _xtprintf(_X("XEngine_Core Test:\n"));
    _xtprintf(_X("XEngine_Core-NetCore_Socket_Create:%d\n"), NetCore_Socket_Create(&hSocket));
    NetCore_Socket_Close(hSocket);
    _xtprintf(_X("XEngine_ManagePool-ManagePool_Memory_Create:%p\n"), xhToken = ManagePool_Memory_Create());
    ManagePool_Memory_Destory(xhToken);
    _xtprintf(_X("XEngine_Cryption-Cryption_Codec_CRC32Codec:%X\n"), Cryption_Codec_CRC32Codec(tszMSGBuffer, 3));
    _xtprintf(_X("XEngine_Protocol-Protocol_ICMPRaw_Init:%d\n"), Protocol_ICMPRaw_Init(&hSocket));
    NetCore_Socket_Close(hSocket);
    
    _xtprintf(_X("XEngine_Client Test:\n"));
    _xtprintf(_X("XClient_Socket-XClient_UDPSelect_Create:%d\n"), XClient_UDPSelect_Create(&hSocket));
    XClient_UDPSelect_Close(hSocket);
    _xtprintf(_X("XClient_APIHelp-APIClient_Http_Create:%d\n"), APIClient_Http_Create(&xhClient));
    APIClient_Http_Close(xhClient);
	_xtprintf(_X("XClient_Stream-XClient_StreamPush_FileInit:%p\n"), xhToken = XClient_StreamPush_FileInit());
    XClient_StreamPush_FileClose(xhToken);

	_xtprintf(_X("XEngine_SystemSdk Test:\n"));
	_xtprintf(_X("XEngine_SystemApi-SystemApi_Process_IsAdmin:%d\n"), SystemApi_Process_IsAdmin());

	_xtprintf(_X("XEngine_HelpComponents Test:\n"));
	_xtprintf(_X("HelpComponents_BINPack-BINPack_UnPack_Close:%d\n"), BINPack_UnPack_Close(xhClient));
    _xtprintf(_X("HelpComponents_Compress-HelpComponents_Compress:%d\n"), HelpCompress_Memory_Compress(tszMSGBuffer, 3, tszMSGBuffer, &nMSGLen, 9));
    _xtprintf(_X("HelpComponents_DataBase-DataBase_SQLite_Open:%d\n"), DataBase_SQLite_Close(0));
    _xtprintf(_X("HelpComponents_Packets-HelpComponents_Help_CheckSum:%X\n"), HelpComponents_Help_CheckSum(tszMSGBuffer, 3));
    _xtprintf(_X("HelpComponents_WBlackList-WBlackList_Domain_Destory:%d\n"), WBlackList_Domain_Destory(xhClient));
	_xtprintf(_X("HelpComponents_XLog-HelpComponents_XLog_Destroy:%d\n"), HelpComponents_XLog_Destroy(NULL));

	_xtprintf(_X("XEngine_NetHelp Test:\n"));
	_xtprintf(_X("NetHelp_APIFlow-APIFlow_NetCtrl_Destory:%d\n"), APIFlow_NetCtrl_Destory(0));
    _xtprintf(_X("NetHelp_APIAddr-APIAddr_EMail_IsEMailAddr:%d\n"), APIAddr_EMail_IsEMailAddr(_X("486179@qq.com")));
    _xtprintf(_X("NetHelp_XSocket-APIFlow_NetCtrl_Destory:%d\n"), XSocket_Api_IsPortOccupation(80, NETHELP_XSOCKET_API_NETSTATE_PROTOCOL_TCP));

	_xtprintf(_X("XEngine_RfcComponents Test:\n"));
	_xtprintf(_X("RfcComponents_HttpProtocol-APIFlow_NetCtrl_Destory:%d\n"), HttpProtocol_Server_DestroyEx(NULL));
    _xtprintf(_X("RfcComponents_MQTTProtocol-MQTTProtocol_Parse_Init:%d\n"), MQTTProtocol_Parse_Init(4));
    MQTTProtocol_Parse_Destory();
    _xtprintf(_X("RfcComponents_NatProtocol-NatProtocol_TurnNat_Create:%d\n"), NatProtocol_TurnNat_Create(_X("1"), _X("2")));
    NatProtocol_TurnNat_Close(_X("1"));
    NTPPROTOCOL_TIMEINFO st_TimeInfo = {};
	_xtprintf(_X("RfcComponents_NTPProtocol-NTPProtocol_Packet_TimeConvert:%d\n"), NTPProtocol_Packet_TimeConvert(&st_TimeInfo));
    _xtprintf(_X("RfcComponents_ProxyProtocol-ProxyProtocol_SocksClient_LoginParse:%d\n"), ProxyProtocol_SocksClient_LoginParse(tszMSGBuffer));
    _xtprintf(_X("RfcComponents_SIPPorotocol-RfcComponents_SIPClient_Delete:%d\n"), RfcComponents_SIPClient_Delete(0));
    _xtprintf(_X("RfcComponents_SnmpProtocol-RfcComponents_SnmpHelp_StrToOID:%d\n"), RfcComponents_SnmpHelp_StrToOID(NULL, NULL, NULL));
    _xtprintf(_X("RfcComponents_SSDPProtocol-SSDPProtocol_Packet_Bye:%d\n"), SSDPProtocol_Packet_Bye(NULL, NULL, NULL));
    _xtprintf(_X("RfcComponents_WSProtocol-RfcComponents_WSPacket_InitEx:%p\n"), xhToken = RfcComponents_WSPacket_InitEx(1));
    RfcComponents_WSPacket_DestoryEx(xhToken);

	_xtprintf(_X("XEngine_StreamMedia Test:\n"));
	_xtprintf(_X("StreamMedia_FLVProtocol-FLVProtocol_Packet_Insert:%d\n"), FLVProtocol_Packet_Insert(tszMSGBuffer));
    FLVProtocol_Packet_Delete(tszMSGBuffer);
	_xtprintf(_X("StreamMedia_HLSProtocol-HLSProtocol_TSParse_Init:%d\n"), HLSProtocol_TSParse_Init(4));
    HLSProtocol_TSParse_Destory();
	_xtprintf(_X("StreamMedia_MP4Protocol-MP4Protocol_Parse_Init:%d\n"), MP4Protocol_Parse_Init(4));
    MP4Protocol_Parse_Destory();
    _xtprintf(_X("StreamMedia_RTCPProtocol-RTCPProtocol_Packet_Sender:%d\n"), RTCPProtocol_Packet_Sender(NULL, NULL, 0, NULL));
	_xtprintf(_X("StreamMedia_RTMPProtocol-RTMPProtocol_Packet_Insert:%d\n"), RTMPProtocol_Packet_Insert(tszMSGBuffer));
    RTMPProtocol_Packet_Delete(tszMSGBuffer);
    _xtprintf(_X("StreamMedia_RTSPProtocol-RTSPProtocol_REPPacket_Response:%d\n"), RTSPProtocol_REPPacket_Response(NULL, NULL, NULL));
	_xtprintf(_X("StreamMedia_SDPProtocol-SDPProtocol_Packet_Create:%d\n"), SDPProtocol_Packet_Create(&xhClient));
    SDPProtocol_Packet_Destory(xhClient);

    _xtprintf(_X("XEngine_AVCodec Test:\n"));
    _xtprintf(_X("XEngine_AudioCodec-AudioCodec_Stream_EnInit:%d\n"), AudioCodec_Stream_EnInit(NULL, NULL));
    _xtprintf(_X("XEngine_AVCollect-AVCollect_Video_Start:%d\n"), AVCollect_Video_Start(NULL));
    _xtprintf(_X("XEngine_AVFilter-AVFilter_Audio_Destroy:%d\n"), AVFilter_Audio_Destroy(0));
    _xtprintf(_X("XEngine_AVFormat-AVFormat_Convert_Stop:%d\n"), AVFormat_Convert_Stop(0));
    _xtprintf(_X("XEngine_AVFrame-AVFrame_Frame_ParseClose:%d\n"), AVFrame_Frame_ParseClose(0));
    _xtprintf(_X("XEngine_AVHelp-AVHelp_Parse_NaluHdr:%d\n"), AVHelp_Parse_NaluHdr(NULL, 0, NULL, NULL));
    _xtprintf(_X("XEngine_VideoCodec-VideoCodec_Stream_EnInit:%d\n"), VideoCodec_Stream_EnInit(NULL, NULL));
#ifdef _MSC_BUILD
    WSACleanup();
#endif
    return 0;
}
