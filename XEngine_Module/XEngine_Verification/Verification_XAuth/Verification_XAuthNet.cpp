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
 参数.三：pInt_Type
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出验证类型
 参数.四：enHWType
  In/Out：Out
  类型：枚举型
  可空：Y
  意思：输入序列号类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_TryRequest(LPCXSTR lpszURLAddr, LPCXSTR lpszPass /* = NULL */, int* pInt_Type /* = NULL */, ENUM_VERIFICATION_MODULE_HW_TYPE enHWType /* = ENUM_VERIFICATION_MODULE_HW_TYPE_BOARD */)
{
	Verification_IsErrorOccur = true;

	if (NULL == lpszURLAddr)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	XCHAR tszJsonStr[XPATH_MAX] = {};
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial = {};

	SystemApi_HardWare_GetSerial(&st_SDKSerial);

	if (ENUM_VERIFICATION_MODULE_HW_TYPE_BOARD == enHWType)
	{
		_xstprintf(tszJsonStr, _X("%s"), st_SDKSerial.tszBoardSerial);
	}
	else if (ENUM_VERIFICATION_MODULE_HW_TYPE_CPU == enHWType)
	{
		_xstprintf(tszJsonStr, _X("%s"), st_SDKSerial.tszCPUSerial);
	}
	else if (ENUM_VERIFICATION_MODULE_HW_TYPE_DISK == enHWType)
	{
		_xstprintf(tszJsonStr, _X("%s"), st_SDKSerial.tszDiskSerial);
	}
	else if (ENUM_VERIFICATION_MODULE_HW_TYPE_SYSTEM == enHWType)
	{
		_xstprintf(tszJsonStr, _X("%s"), st_SDKSerial.tszSystemSerial);
	}
	else
	{
		_xstprintf(tszJsonStr, _X("%s"), st_SDKSerial.tszBoardSerial);
	}

	if (_tcsxlen(tszJsonStr) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_GETSERIAL;
		return false;
	}

	st_JsonObject["tszVSerial"] = tszJsonStr;
	st_JsonRoot["st_VERTemp"] = st_JsonObject;
	//请求
	XCHAR tszMSGBuffer[XPATH_MAX] = {};
	int nMsgLen = st_JsonRoot.toStyledString().length();
	if (!Verification_XAuthNet_HTTPRequest(lpszURLAddr, tszMSGBuffer, &nMsgLen, st_JsonRoot.toStyledString().c_str()))
	{
		return false;
	}
	//解析回复
	st_JsonObject.clear();
	st_JsonRoot.clear();
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMSGBuffer, tszMSGBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}

	if (st_JsonRoot["code"].isNull())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE;
		return false;
	}
	if (NULL != pInt_Type)
	{
		if (!st_JsonRoot["type"].isNull())
		{
			*pInt_Type = st_JsonRoot["type"].asInt();
		}
	}
	m_bLogin = true;
	m_bAuth = true;
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
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	//请求
	int nMsgLen = 0;
	XCHAR tszMSGBuffer[XPATH_MAX] = {};
	if (!Verification_XAuthNet_HTTPRequest(lpszURLAddr, tszMSGBuffer, &nMsgLen))
	{
		return false;
	}
	//解析回复
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMSGBuffer, tszMSGBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}

	if (0 != st_JsonRoot["code"].asInt())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE;
		return false;
	}

	*pInt_DYCode = st_JsonRoot["nDynamicCode"].asInt();
	*pxhToken = st_JsonRoot["xhToken"].asUInt64();
	if (NULL != pInt_Timeout)
	{
		*pInt_Timeout = st_JsonRoot["nTimeout"].asInt();
	}
	return true;
}
/********************************************************************
函数名称：Verification_XAuthNet_Register
函数功能：用户注册请求
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器地址,完成的API地址,比如:http://127.0.0.1:5302/auth/user/register
 参数.二：pSt_UserInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入注册信息
 参数.三：lpszHWCode
  In/Out：Out
  类型：常量字符指针
  可空：N
  意思：输入要绑定的硬件序列号
 参数.四：lpszPassword
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_Register(LPCXSTR lpszURLAddr, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo, LPCXSTR lpszHWCode /* = NULL */, LPCXSTR lpszPassword /* = NULL */)
{
	Verification_IsErrorOccur = false;

	if (NULL == pSt_UserInfo)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	Json::Value st_JsonUserInfo;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	st_JsonUserInfo["tszUserName"] = pSt_UserInfo->tszUserName;
	st_JsonUserInfo["tszUserPass"] = pSt_UserInfo->tszUserPass;
	st_JsonUserInfo["tszEMailAddr"] = pSt_UserInfo->tszEMailAddr;
	st_JsonUserInfo["nPhoneNumber"] = (Json::Value::Int64)pSt_UserInfo->nPhoneNumber;
	st_JsonUserInfo["nIDNumber"] = (Json::Value::Int64)pSt_UserInfo->nIDNumber;
	st_JsonUserInfo["nUserLevel"] = pSt_UserInfo->nUserLevel;

	st_JsonObject["st_UserInfo"] = st_JsonUserInfo;
	st_JsonObject["tszHardCode"] = lpszHWCode;

	st_JsonRoot["st_UserTable"] = st_JsonObject;

	int nMsgLen = 0;
	XCHAR tszMSGBuffer[XPATH_MAX] = {};
	if (!Verification_XAuthNet_HTTPRequest(lpszURLAddr, tszMSGBuffer, &nMsgLen, st_JsonRoot.toStyledString().c_str()))
	{
		return false;
	}
	st_JsonRoot.clear();
	st_JsonObject.clear();
	//解析回复
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMSGBuffer, tszMSGBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}

	if (st_JsonRoot["code"].isNull())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：Verification_XAuthNet_GetTime
函数功能：获取用户时间信息
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入API地址
 参数.二：penSerialType
  In/Out：Out
  类型：枚举型指针
  可空：N
  意思：输出用户序列号类型
 参数.三：pSt_UserInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出用户序列号类型
 参数.四：pInt_OnlineTime
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出在线时间
 参数.五：ptszLeftTime
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出剩余时间字符串
 参数.六：lpszPassword
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_GetTime(LPCXSTR lpszURLAddr, XENGINE_PROTOCOL_USERAUTH* pSt_UserInfo, ENUM_VERIFICATION_MODULE_SERIAL_TYPE* penSerialType, __int64x* pInt_OnlineTime, XCHAR* ptszLeftTime, LPCXSTR lpszPassword /* = NULL */)
{
	Verification_IsErrorOccur = false;

	if (NULL == lpszURLAddr)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	st_JsonObject["tszUserName"] = pSt_UserInfo->tszUserName;
	st_JsonObject["tszUserPass"] = pSt_UserInfo->tszUserPass;

	st_JsonRoot["st_UserAuth"] = st_JsonObject;

	int nMsgLen = 0;
	XCHAR tszMSGBuffer[XPATH_MAX] = {};
	if (!Verification_XAuthNet_HTTPRequest(lpszURLAddr, tszMSGBuffer, &nMsgLen, st_JsonRoot.toStyledString().c_str(), lpszPassword))
	{
		return false;
	}
	st_JsonRoot.clear();
	st_JsonObject.clear();
	//解析回复
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMSGBuffer, tszMSGBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}

	if (st_JsonRoot["code"].isNull() || st_JsonRoot["st_UserTime"].isNull())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE;
		return false;
	}
	st_JsonObject = st_JsonRoot["st_UserTime"][0];

	if (!st_JsonObject["enSerialType"].isNull())
	{
		*penSerialType = (ENUM_VERIFICATION_MODULE_SERIAL_TYPE)st_JsonObject["enSerialType"].asInt();
	}
	if (!st_JsonObject["nTimeONLine"].isNull())
	{
		*pInt_OnlineTime = (__int64x)st_JsonObject["nTimeONLine"].asInt64();
	}
	if (!st_JsonObject["tszLeftTime"].isNull())
	{
		_tcsxcpy(ptszLeftTime, st_JsonObject["tszLeftTime"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：Verification_XAuthNet_GetPass
函数功能：密码找回
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入API地址
 参数.二：pSt_UserInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入用户注册信息,身份证,邮箱,手机号
 参数.三：pSt_UserAuth
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出用户密码
 参数.四：lpszPassword
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_GetPass(LPCXSTR lpszURLAddr, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth, LPCXSTR lpszPassword /* = NULL */)
{
	Verification_IsErrorOccur = false;

	if (NULL == pSt_UserInfo)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	st_JsonObject["tszUserName"] = pSt_UserInfo->tszUserName;
	if (_tcsxlen(pSt_UserInfo->tszUserPass) > 0)
	{
		st_JsonObject["tszUserPass"] = pSt_UserInfo->tszUserPass;
	}
	st_JsonObject["tszEMailAddr"] = pSt_UserInfo->tszEMailAddr;
	st_JsonObject["nPhoneNumber"] = (Json::Value::Int64)pSt_UserInfo->nPhoneNumber;
	st_JsonObject["nIDNumber"] = (Json::Value::Int64)pSt_UserInfo->nIDNumber;

	st_JsonRoot["st_UserInfo"] = st_JsonObject;

	int nMsgLen = 0;
	XCHAR tszMSGBuffer[XPATH_MAX] = {};
	if (!Verification_XAuthNet_HTTPRequest(lpszURLAddr, tszMSGBuffer, &nMsgLen, st_JsonRoot.toStyledString().c_str()))
	{
		return false;
	}
	st_JsonRoot.clear();
	st_JsonObject.clear();
	//解析回复
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMSGBuffer, tszMSGBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}

	if (st_JsonRoot["code"].isNull() || st_JsonRoot["st_UserAuth"].isNull())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE;
		return false;
	}
	st_JsonObject = st_JsonRoot["st_UserAuth"];
	if (!st_JsonObject["enClientType"].isNull())
	{
		pSt_UserAuth->enClientType = (ENUM_PROTOCOLCLIENT_TYPE)st_JsonObject["enClientType"].asInt();
	}
	if (!st_JsonObject["enDeviceType"].isNull())
	{
		pSt_UserAuth->enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)st_JsonObject["enDeviceType"].asInt();
	}
	if (!st_JsonObject["tszUserName"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszUserName, st_JsonObject["tszUserName"].asCString());
	}
	if (!st_JsonObject["tszUserPass"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszUserPass, st_JsonObject["tszUserPass"].asCString());
	}

	return true;
}
/********************************************************************
函数名称：Verification_XAuthNet_Pay
函数功能：充值
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入API地址
 参数.二：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：充值的用户
 参数.三：lpszSerial
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入充值的序列号
 参数.四：lpszPassword
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_Pay(LPCXSTR lpszURLAddr, LPCXSTR lpszUser, LPCXSTR lpszSerial, LPCXSTR lpszPassword /* = NULL */)
{
	Verification_IsErrorOccur = false;

	if (NULL == lpszURLAddr)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	st_JsonObject["tszSerialNumber"] = lpszSerial;
	st_JsonObject["tszUserName"] = lpszUser;

	st_JsonRoot["st_UserPay"] = st_JsonObject;

	int nMsgLen = 0;
	XCHAR tszMSGBuffer[XPATH_MAX] = {};
	if (!Verification_XAuthNet_HTTPRequest(lpszURLAddr, tszMSGBuffer, &nMsgLen, st_JsonRoot.toStyledString().c_str()))
	{
		return false;
	}
	st_JsonRoot.clear();
	st_JsonObject.clear();
	//解析回复
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMSGBuffer, tszMSGBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}

	if (st_JsonRoot["code"].isNull())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARSE;
		return false;
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_CODE;
		return false;
	}

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
	if (!m_bAuth)
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
 参数.七：pSt_UserInfo
  In/Out：In
  类型：数据结构指针
  可空：Y
  意思：输出用户信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszHWCode /* = NULL */, XSHOT nDYCode /* = 0 */, XNETHANDLE xhToken /* = 0 */, XLONG dwCryption /* = 0 */, VERIFICATION_USERINFO* pSt_UserInfo /* = NULL */)
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
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN;
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
	XCHAR* ptszMsgBuffer = NULL;
	st_ProtocolHdr = {};
	//接受数据
	if (!XClient_TCPSelect_RecvPkt(m_hSocket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = XClient_GetLastError();
		return false;
	}
	if (st_ProtocolHdr.unPacketSize == sizeof(VERIFICATION_USERINFO))
	{
		if (ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT == st_ProtocolHdr.wCrypto)
		{
			XCHAR tszCodecBuffer[2048] = {};
			st_ProtocolHdr.wCrypto = ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT;
			Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszCodecBuffer, tszPassStr);
			memcpy(&st_UserInfo, tszCodecBuffer, sizeof(VERIFICATION_USERINFO));
		}
		else
		{
			memcpy(&st_UserInfo, ptszMsgBuffer, sizeof(VERIFICATION_USERINFO));
		}
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
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
	if (NULL != pSt_UserInfo)
	{
		*pSt_UserInfo = st_UserInfo;
	}
	xhToken = st_ProtocolHdr.xhToken;
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
/********************************************************************
函数名称：Verification_XAuthNet_GetToken
函数功能：获取当前的令牌句柄
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出令牌句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CVerification_XAuthNet::Verification_XAuthNet_GetToken(XNETHANDLE* pxhToken)
{
	Verification_IsErrorOccur = false;

	if (NULL == pxhToken)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	if (!Verification_XAuthNet_GetAuth())
	{
		return false;
	}
	*pxhToken = xhToken;
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
bool CVerification_XAuthNet::Verification_XAuthNet_HTTPRequest(LPCXSTR lpszURLAddr, XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszMSGBuffer /* = NULL */, LPCXSTR lpszPassword /* = NULL */)
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
	LPCXSTR lpszCustomHdr = _X("Content-Type: application/json\r\n");
	//请求
	int nHTTPLen = *pInt_MSGLen;
	XCHAR* ptszHTTPBuffer = NULL;
	if (NULL == lpszMSGBuffer)
	{
		if (!APIClient_Http_Request(_X("GET"), lpszURLAddr, NULL, &nHTTPCode, &ptszHTTPBuffer, &nHTTPLen))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = APIClient_GetLastError();
			return false;
		}
	}
	else
	{
		if (NULL == lpszPassword)
		{
			if (!APIClient_Http_Request(_X("POST"), lpszURLAddr, lpszMSGBuffer, &nHTTPCode, &ptszHTTPBuffer, &nHTTPLen, lpszCustomHdr))
			{
				Verification_IsErrorOccur = true;
				Verification_dwErrorCode = APIClient_GetLastError();
				return false;
			}
		}
		else
		{
			XCHAR tszENCodec[2048] = {};
			if (!Cryption_XCrypto_Encoder(lpszMSGBuffer, &nHTTPLen, (XBYTE*)tszENCodec, lpszPassword))
			{
				Verification_IsErrorOccur = true;
				Verification_dwErrorCode = Cryption_GetLastError();
				BaseLib_Memory_FreeCStyle((XPPMEM)&ptszHTTPBuffer);
				return false;
			}
			if (!APIClient_Http_Request(_X("POST"), lpszURLAddr, tszENCodec, &nHTTPCode, &ptszHTTPBuffer, &nHTTPLen, lpszCustomHdr))
			{
				Verification_IsErrorOccur = true;
				Verification_dwErrorCode = APIClient_GetLastError();
				return false;
			}
		}
	}
	//解析回复
	if (NULL == lpszPassword)
	{
		*pInt_MSGLen = nHTTPLen;
		_tcsxcpy(ptszMSGBuffer, ptszHTTPBuffer);
	}
	else
	{
		XCHAR tszDECodec[2048] = {};
		if (!Cryption_XCrypto_Decoder(ptszHTTPBuffer, &nHTTPLen, tszDECodec, lpszPassword))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = Cryption_GetLastError();
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszHTTPBuffer);
			return false;
		}

		*pInt_MSGLen = nHTTPLen;
		_tcsxcpy(ptszMSGBuffer, tszDECodec);
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszHTTPBuffer);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD XCALLBACK CVerification_XAuthNet::Verification_XAuthNet_Thread(XPVOID lParam)
{
	CVerification_XAuthNet* pClass_This = (CVerification_XAuthNet*)lParam;
	XENGINE_PROTOCOLHDR st_ProtocolHeart = {};

	st_ProtocolHeart.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHeart.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHeart.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_HB_SYN;
	st_ProtocolHeart.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	while (pClass_This->m_bRun)
	{
		//发送心跳
		if (!XClient_TCPSelect_SendMsg(pClass_This->m_hSocket, (LPCXSTR)&st_ProtocolHeart, sizeof(XENGINE_PROTOCOLHDR)))
		{
			//心跳失败
			pClass_This->m_bRun = false;
			pClass_This->m_bLogin = false;
			pClass_This->m_bAuth = false;
			XClient_TCPSelect_Close(pClass_This->m_hSocket);
			break;
		}
		int nMsgLen = 0;
		XCHAR* ptszMsgBuffer = NULL;
		XENGINE_PROTOCOLHDR st_ProtocolHdr = {};
		if (!XClient_TCPSelect_RecvPkt(pClass_This->m_hSocket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			XLONG dwRet = XClient_GetLastError();
			if (ERROR_XCLIENT_SOCKET_TCPSOCKET_SELECT_RECVPKT_TIMEOUT == dwRet)
			{
				continue;
			}
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