#include "pch.h"
#include "Verification_XAuthNet.h"
/********************************************************************
//    Created:     2025/09/30  16:55:12
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth\Verification_XAuthNet.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth
//    File Base:   Verification_XAuthNet
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     XAuth网络验证
//    History:
*********************************************************************/
CVerification_XAuthNet::CVerification_XAuthNet()
{
}
CVerification_XAuthNet::~CVerification_XAuthNet()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Verification_XAuthNet_TryRequest
函数功能：试用版请求
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：函数的API地址,需要输入完整的URL
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_TryRequest(LPCXSTR lpszURLAddr, LPCXSTR lpszPass /* = NULL */)
{
	Verification_IsErrorOccur = true;

	if (NULL == lpszURLAddr)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	int nHTTPCode = 0;
	XCHAR tszJsonStr[XPATH_MAX] = {};
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial = {};

	SystemApi_HardWare_GetSerial(&st_SDKSerial);
	_xstprintf(tszJsonStr, _X("%s"), st_SDKSerial.tszBoardSerial);

	st_JsonObject["tszVSerial"] = tszJsonStr;
	st_JsonRoot["st_VERTemp"] = st_JsonObject;
	//请求
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;

	if (NULL != lpszPass)
	{
		XCHAR tszENCodec[2048] = {};
		XCHAR tszDECodec[2048] = {};

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (XBYTE*)tszENCodec, lpszPass);
		if (!APIClient_Http_Request(_X("POST"), lpszURLAddr, tszENCodec, &nHTTPCode, &ptszMsgBuffer, &nMsgLen))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = APIClient_GetLastError();
			return false;
		}
		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDECodec, lpszPass);
		st_JsonRoot.clear();
		st_JsonObject.clear();
		//解析回复
		std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
		if (!pSt_JsonReader->parse(tszDECodec, tszDECodec + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			return false;
		}
	}
	else
	{
		if (!APIClient_Http_Request(_X("POST"), lpszURLAddr, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBuffer, &nMsgLen))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = APIClient_GetLastError();
			return false;
		}
		st_JsonRoot.clear();
		st_JsonObject.clear();
		//解析回复
		std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			return false;
		}
	}

	if (0 != st_JsonRoot["code"].asInt())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE;
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		return false;
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return true;
}
/********************************************************************
函数名称：Verification_XAuthNet_GetDCode
函数功能：获取动态码
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器地址,完成的API地址,比如:http://127.0.0.1:5302/api?function=dcode&user=get
 参数.二：pInt_DYCode
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出动态码
 参数.三：pxhToken
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出绑定的句柄
 参数.四：pInt_Timeout
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出动态码超时时间
 参数.五：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_GetDCode(LPCXSTR lpszURLAddr, int* pInt_DYCode, XNETHANDLE* pxhToken, int* pInt_Timeout /* = NULL */, LPCXSTR lpszPass /* = NULL */)
{
	Verification_IsErrorOccur = true;

	if (NULL == lpszURLAddr)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	int nHTTPCode = 0;
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	//请求
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	if (!APIClient_Http_Request(_X("GET"), lpszURLAddr, NULL, &nHTTPCode, &ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	
	if (NULL != lpszPass)
	{
		XCHAR tszDECodec[2048] = {};
		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDECodec, lpszPass);

		std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
		if (!pSt_JsonReader->parse(tszDECodec, tszDECodec + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			return false;
		}
	}
	else
	{
		//解析回复
		std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			return false;
		}
	}

	if (0 != st_JsonRoot["code"].asInt())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE;
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		return false;
	}

	*pInt_DYCode = st_JsonRoot["nDynamicCode"].asInt();
	*pxhToken = st_JsonRoot["xhToken"].asUInt64();
	if (NULL != pInt_Timeout)
	{
		*pInt_Timeout = st_JsonRoot["nTimeout"].asInt();
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return true;
}
/********************************************************************
函数名称：Verification_XAuthNet_Connect
函数功能：链接到服务器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器地址
 参数.二：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：服务器端口
 参数.三：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器密码，如果没有填空
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_Connect(LPCXSTR lpszClientAddr, int nPort, LPCXSTR lpszPass /* = NULL */)
{
	Verification_IsErrorOccur = true;

	if (NULL == lpszClientAddr)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	if (!XClient_TCPSelect_Create(&m_hSocket, lpszClientAddr, nPort, 2))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = XClient_GetLastError();
		return false;
	}
	if (NULL != lpszPass)
	{
		_tcsxcpy(tszPassStr, lpszPass);
	}
	return true;
}
/********************************************************************
函数名称：Verification_XAuthNet_Close
函数功能：销毁关闭客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_Close()
{
	Verification_IsErrorOccur = false;

	if (NULL != pSTDThread)
	{
		m_bRun = false;
		pSTDThread->join();
	}
	m_bLogin = false;
	m_bAuth = false;
	XClient_TCPSelect_Close(m_hSocket);
	return true;
}
/********************************************************************
函数名称：Verification_XAuthNet_GetAuth
函数功能：验证用户是否登录或者超时
返回值
  类型：逻辑型
  意思：是否通过验证,可能是没有登录或者过期
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_GetAuth()
{
	Verification_IsErrorOccur = false;

	if (!m_bLogin)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_NOLOGIN;
		return false;
	}
	if (m_bAuth)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_EXPIRED;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：Verification_XAuthNet_Login
函数功能：登录到服务器
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.三：lpszHWCode
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入硬件码,如果服务器开启了硬件绑定
 参数.四：nDYCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入动态码
 参数.五：xhToken
  In/Out：In
  类型：句柄型
  可空：Y
  意思：输入动态码绑定的句柄
 参数.六：dwCryption
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入密码加密类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszHWCode /* = NULL */, XSHOT nDYCode /* = 0 */, XNETHANDLE xhToken /* = 0 */, XLONG dwCryption /* = 0 */)
{
	Verification_IsErrorOccur = false;

	if ((NULL == lpszUser) || (NULL == lpszPass))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	XCHAR tszMsgBuffer[2048] = {};
	XENGINE_PROTOCOLHDR st_ProtocolHdr = {};
	XENGINE_PROTOCOL_USERAUTHEX st_AuthUser = {};
	//协议头
	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHdr.unOperatorCode = 0x2005;
	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_USERAUTHEX);
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

#ifdef _MSC_BUILD
	st_AuthUser.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_WINDOWS;
#elif __linux__
	st_AuthUser.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_LINUX;
#else
	st_AuthUser.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_MACOS;
#endif
	_tcsxcpy(st_AuthUser.tszUserName, lpszUser);
	if (NULL != lpszHWCode)
	{
		_tcsxcpy(st_AuthUser.tszHWCode, lpszHWCode);
	}

	if (dwCryption > 0)
	{
		int nPLen = _tcsxlen(lpszPass);
		XBYTE byMD5Buffer[XPATH_MAX] = {};
		Cryption_Api_Digest(lpszPass, byMD5Buffer, &nPLen, false, dwCryption);
		BaseLib_String_StrToHex((LPCXSTR)byMD5Buffer, nPLen, st_AuthUser.tszUserPass);
	}
	else
	{
		_tcsxcpy(st_AuthUser.tszUserPass, lpszPass);
	}
	//是否有动态码
	if (nDYCode > 0)
	{
		st_ProtocolHdr.xhToken = xhToken;
		_xstprintf(st_AuthUser.tszDCode, _X("%d"), nDYCode);
	}
	//是否加密
	int nMsgLen = 0;
	if (_tcsxlen(tszPassStr) > 0)
	{
		XCHAR tszCodecBuffer[2048] = {};

		st_ProtocolHdr.wCrypto = ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT;
		Cryption_XCrypto_Encoder((LPCXSTR)&st_AuthUser, (int*)&st_ProtocolHdr.unPacketSize, (XBYTE*)tszCodecBuffer, tszPassStr);

		memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), tszCodecBuffer, st_ProtocolHdr.unPacketSize);

		nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	}
	else
	{
		memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_AuthUser, st_ProtocolHdr.unPacketSize);

		nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOL_USERAUTHEX);
	}
	//发送数据
	if (!XClient_TCPSelect_SendMsg(m_hSocket, tszMsgBuffer, nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = XClient_GetLastError();
		return false;
	}

	nMsgLen = 0;
	XCHAR* ptszMsgBuffer;
	st_ProtocolHdr = {};
	//接受数据
	if (!XClient_TCPSelect_RecvPkt(m_hSocket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = XClient_GetLastError();
		return false;
	}
	//判断是否登录协议
	if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN != st_ProtocolHdr.unOperatorCode)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE;
		return false;
	}
	//登录失败，错误码
	if (0 != st_ProtocolHdr.wReserve)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = st_ProtocolHdr.wReserve;
		return false;
	}
	m_bRun = true;
	m_bLogin = true;
	m_bAuth = true;
	//登录成功，创建线程
	pSTDThread = std::make_unique<std::thread>(Verification_XAuthNet_Thread, this);
	if (NULL == pSTDThread)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_THREAD;
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD XCALLBACK CVerification_XAuthNet::Verification_XAuthNet_Thread(XPVOID lParam)
{
	CVerification_XAuthNet* pClass_This = (CVerification_XAuthNet*)lParam;

	while (pClass_This->m_bRun)
	{
		int nMsgLen = 0;
		XCHAR* ptszMsgBuffer = NULL;
		XENGINE_PROTOCOLHDR st_ProtocolHdr = {};

		if (!XClient_TCPSelect_RecvPkt(pClass_This->m_hSocket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			pClass_This->m_bRun = false;
			pClass_This->m_bLogin = false;
			pClass_This->m_bAuth = false;
			XClient_TCPSelect_Close(pClass_This->m_hSocket);
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			break;
		}
		XCHAR tszMsgBuffer[4096] = {};
		if (nMsgLen > 0 && _tcsxlen(pClass_This->tszPassStr) > 0)
		{
			//只有有后续数据的情况才需要解密
			Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, pClass_This->tszPassStr);
		}
		else
		{
			memcpy(tszMsgBuffer, ptszMsgBuffer, nMsgLen);
		}
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_EXPIRED == st_ProtocolHdr.wReserve)
		{
			pClass_This->m_bRun = false;
			pClass_This->m_bLogin = false;
			pClass_This->m_bAuth = false;
			XClient_TCPSelect_Close(pClass_This->m_hSocket);
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}