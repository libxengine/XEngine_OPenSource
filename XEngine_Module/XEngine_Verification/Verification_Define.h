#pragma once
/********************************************************************
//    Created:     2025/08/19  10:52:59
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_Define.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification
//    File Base:   Verification_Define
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     验证模块导出定义
//    History:
*********************************************************************/
typedef enum
{
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_UNKNOW = 0,                 //无法识别的充值卡
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND = 1,                 //秒钟,本地使用在read和write的时候更新
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY = 2,                    //天数,本地使用天数卡,不使用不减天数.
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_TIME = 3,                   //次数卡
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_CUSTOM = 4                  //自定义过期日期
}ENUM_AUTHORIZE_MODULE_SERIAL_TYPE, * LPENUM_AUTHORIZE_MODULE_SERIAL_TYPE;
typedef enum
{
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_UNKNOW = 0,                    //未注册,Authorize_CDKey_GetLeftTimer将返回失败
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_TEMP = 1,                      //临时,Authorize_CDKey_GetLeftTimer一次后过期,需要Write
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_TRY = 2,                       //试用
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_OFFICIAL = 3,                  //正式版
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_UNLIMIT = 4,                   //无限制版,永不过期.CDKEY不做任何验证
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_EXPIRED = 5                    //已过期的版本,Authorize_CDKey_GetLeftTimer将返回失败
}ENUM_AUTHORIZE_MODULE_CDKEY_TYPE, * LPENUM_AUTHORIZE_MODULE_CDKEY_TYPE;
typedef enum
{
	ENUM_AUTHORIZE_MODULE_HW_TYPE_UNKNOW = 0,                     //未指定
	ENUM_AUTHORIZE_MODULE_HW_TYPE_CPU = 1,                        //CPU序列号
	ENUM_AUTHORIZE_MODULE_HW_TYPE_DISK = 2,                       //硬盘序列号
	ENUM_AUTHORIZE_MODULE_HW_TYPE_BOARD = 3,                      //主板序列号
	ENUM_AUTHORIZE_MODULE_HW_TYPE_MAC = 4,                        //网卡MAC地址
	ENUM_AUTHORIZE_MODULE_HW_TYPE_BIOS = 5                        //BIOS序列号
}ENUM_AUTHORIZE_MODULE_HW_TYPE, * LPENUM_AUTHORIZE_MODULE_HW_TYPE;
typedef enum
{
	ENUM_AUTHORIZE_MODULE_VERMODE_TYPE_UNKNOW = 0,                 //未知
	ENUM_AUTHORIZE_MODULE_VERMODE_TYPE_LOCAL = 0x01,               //本地
	ENUM_AUTHORIZE_MODULE_VERMODE_TYPE_NETWORK = 0x02,             //网络
}ENUM_AUTHORIZE_MODULE_VERMODE_TYPE, * LPENUM_AUTHORIZE_MODULE_VERMODE_TYPE;
//////////////////////////////////////////////////////////////////////////
//                        导出定义
//////////////////////////////////////////////////////////////////////////
//OAUTH2验证类型
#define XENGINE_VERIFICATION_OAUTH_GRANDTYPE_AUTHORIZATION_CODE _X("authorization_code") //授权码
#define XENGINE_VERIFICATION_OAUTH_GRANDTYPE_CLIENT_CREDENTIALS _X("client_credentials") //客户端凭据
#define XENGINE_VERIFICATION_OAUTH_GRANDTYPE_PASSWORD _X("password")                     //密码
#define XENGINE_VERIFICATION_OAUTH_GRANDTYPE_REFRESH_TOKEN _X("refresh_token")           //刷新令牌
//OAUTH2错误类型
#define ERROR_XENGINE_VERIFICATION_OAUTH_INVALID_REQUEST _X("invalid_request")           //请求不完整或格式错误（缺少参数、无效参数、重定向 URI 不匹配等
#define ERROR_XENGINE_VERIFICATION_OAUTH_INVALID_CLIENT _X("invalid_client")             //无效的客户端
#define ERROR_XENGINE_VERIFICATION_OAUTH_INVALID_GRANT _X("invalid_grant")               //无效的授权
#define ERROR_XENGINE_VERIFICATION_OAUTH_UNAUTHORIZED_CLIENT _X("unauthorized_client")   //客户端未授权
#define ERROR_XENGINE_VERIFICATION_OAUTH_UNSUPPORTED_GRANT_TYPE _X("unsupported_grant_type")   //不支持的授权类型
#define ERROR_XENGINE_VERIFICATION_OAUTH_UNSUPPORTED_TOKEN_TYPE _X("unsupported_token_type") //不支持的令牌类型
#define ERROR_XENGINE_VERIFICATION_OAUTH_INVALID_SCOPE _X("invalid_scope")               //无效的范围
#define ERROR_XENGINE_VERIFICATION_OAUTH_ACCESS_DENIED _X("access_denied")               //用户拒绝授权
#define ERROR_XENGINE_VERIFICATION_OAUTH_SERVER_ERROR _X("server_error")                 //服务器错误
#define ERROR_XENGINE_VERIFICATION_OAUTH_UNSUPPORTED_RESPONSE_TYPE _X("unsupported_response_type") //不支持的响应类型
#define ERROR_XENGINE_VERIFICATION_OAUTH_TEMPORARILY_UNAVAILABLE _X("temporarily_unavailable") //临时不可用
#define ERROR_XENGINE_VERIFICATION_OAUTH_INVALID_TOKEN _X("invalid_token")             //无效的令牌
#define ERROR_XENGINE_VERIFICATION_OAUTH_EXPIRED_TOKEN _X("expired_token")             //令牌已过期
#define ERROR_XENGINE_VERIFICATION_OAUTH_INSUFFICIENT_SCOPE _X("insufficient_scope")   //范围不足
//////////////////////////////////////////////////////////////////////////
//                        导出结构体
//////////////////////////////////////////////////////////////////////////
typedef struct  
{
	XCHAR tszClientID[128];                 // 客户端ID
	XCHAR tszClientCode[128];               // 客户端代码,authorization_code有效
	XCHAR tszClientSecert[128];             // 客户端秘钥
	XCHAR tszClientUser[128];               // 客户端用户名,仅PASSWORD授权类型使用
	XCHAR tszClientPass[128];               // 客户端密码,仅PASSWORD授权类型使用
	XCHAR tszClientRefresh[128];            // 客户端刷新令牌,仅REFRESH_TOKEN授权类型使用
	XCHAR tszGrandType[64];                 // 授权类型
}VERIFICATION_OAUTHINFO;
typedef struct
{
	XCHAR tszTokenAccess[XPATH_MID];                // 访问的TOKEN
	XCHAR tszTokenRefresh[XPATH_MID];               // 刷新的TOKEN
	XCHAR tszTokenType[128];                        // 授权类型
	int nExpiredTime;                               // 过期时间
}VERIFICATION_TOKENINFO;
typedef struct
{
	XCHAR tszAddr[XPATH_MAX];                                             //服务器或者域名地址
	int nPort;                                                           //端口号码,如果>0表示CDKEY验证失败后改为网络验证
	//版本信息
	struct
	{
		XCHAR tszAppName[128];                                            //应用程序名称
		XCHAR tszAppVer[128];                                             //应用程序版本号
		__int64x nExecTime;                                              //程序已经执行次数,调用Authorize_CDKey_GetLeftTimer会更新
		bool bInit;                                                      //是否初始化,由用户控制
	}st_AuthAppInfo;
	//CDKEY信息
	struct
	{
		XCHAR tszHardware[1024];                                          //硬件码
		XCHAR tszCreateTime[64];                                          //CDKEY创建日期，年/月/日-小时：分钟：秒
		XCHAR tszRegisterTime[64];                                        //注册时间，年/月/日-小时：分钟：秒
		XCHAR tszLeftTime[64];                                            //总的剩余时间,过期日期，根据nLeftType决定此值的意义
		XCHAR tszStartTime[64];                                           //当前启动时间,由系统读取CDKEY的时候自动更新,天数和分钟有效
		XCHAR tszExpiryTime[64];                                          //过期的时间,需要调用Authorize_CDKey_GetLeftTimer并且Write才生效
		__int64x nHasTime;                                               //当前还拥有时间，根据nLeftType决定此值的意义,调用Authorize_CDKey_GetLeftTimer会更新
		ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType;          //过期类型，参考:ENUM_AUTHORIZE_MODULE_SERIAL_TYPE
		ENUM_AUTHORIZE_MODULE_CDKEY_TYPE enRegType;                //注册类型，参考:ENUM_AUTHORIZE_MODULE_CDKEY_TYPE
		ENUM_AUTHORIZE_MODULE_HW_TYPE enHWType;                  //硬件类型，参考:ENUM_AUTHORIZE_MODULE_HW_TYPE
		ENUM_AUTHORIZE_MODULE_VERMODE_TYPE enVModeType;          //验证方式，参考:ENUM_AUTHORIZE_MODULE_VERMODE_TYPE 
	}st_AuthRegInfo;
	//临时序列号
	struct
	{
		//次数限制
		struct
		{
			XCHAR tszTimeSerial[128];
			int nTimeCount;                                              //使用次数
		}st_TimeLimit;
		//时间限制
		struct
		{
			XCHAR tszDataTime[128];                                       //过期时间
			XCHAR tszDataSerial[128];                                     //序列号
			bool bTimeAdd;                                                //真,过期时间是设置的日期-创建日期.假过期日期就是设定的日期
		}st_DataLimit;
		//无限制
		struct
		{
			XCHAR tszUNLimitSerial[128];                                  //无限制序列号
		}st_UNLimit;
	}st_AuthSerial;
	//注册的用户信息，可以不填
	struct
	{
		XCHAR tszUserName[64];                                            //注册的用户
		XCHAR tszUserContact[64];                                         //联系方式，电子邮件或者手机等
		XCHAR tszCustom[1024];                                            //自定义数据
	}st_AuthUserInfo;
}VERIFICATION_XAUTHKEY;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Verification_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                         HTTP验证导出函数                             */
/************************************************************************/
/********************************************************************
函数名称：Verification_HTTP_BasicClientPacket
函数功能：客户端基本验证打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包后的HTTP验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
 参数.三：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.四：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_HTTP_BasicClientPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszUser, LPCXSTR lpszPass);
/********************************************************************
函数名称：Verification_HTTP_DigestClientPacket
函数功能：客户端信息摘要打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包后的HTTP验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
 参数.三：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.四：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.五：lpszRequestMethod
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的方法
 参数.六：lpszRequestUri
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的URL地址
 参数.七：lpszNonceStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务端返回的随机数
 参数.八：lpszOpaqueStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务端返回的不透明值
 参数.九：bQOPBody
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：请求的QOP保护级别,默认为false
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_HTTP_DigestClientPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszRequestMethod, LPCXSTR lpszRequestUri, LPCXSTR lpszNonceStr, LPCXSTR lpszOpaqueStr, bool bQOPBody = false);
/********************************************************************
函数名称：Verification_HTTP_BasicServerPacket
函数功能：服务端请求基本验证打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包后的HTTP验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_HTTP_BasicServerPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen);
/********************************************************************
函数名称：Verification_HTTP_DigestServerPacket
函数功能：服务端请求数据摘要打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包后的HTTP验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_HTTP_DigestServerPacket(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, XCHAR* ptszNonceStr, XCHAR* ptszOpaqueStr, bool bQOPBody = false);
/********************************************************************
函数名称：Verification_HTTP_GetType
函数功能：获得HTTP验证类型
 参数.一：pptszListHdr
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入HTTP头
 参数.二：nHdrCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入HTTP头个数
 参数.三：pInt_Type
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出HTTP验证类型1：BASIC验证 2：摘要验证
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_HTTP_GetType(XCHAR** pptszListHdr, int nHdrCount, int* pInt_Type);
/********************************************************************
函数名称：Verification_HTTP_Basic
函数功能：HTTP基本验证
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要验证的用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要验证的密码
 参数.三：pptszListHdr
  In/Out：In
  类型：指向指针的指针
  可空：N
  意思：输入要解析的HTTP头
 参数.四：nHdrCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的HTTP头列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_HTTP_Basic(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR** pptszListHdr, int nHdrCount);
/********************************************************************
函数名称：Verification_HTTP_Digest
函数功能：HTTP摘要验证
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要验证的用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要验证的密码
 参数.三：lpszMethod
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：客户端请求的方法
 参数.四：pptszListHdr
  In/Out：In
  类型：指向指针的指针
  可空：N
  意思：输入要解析的HTTP头
 参数.五：nHdrCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的HTTP头列表个数
 参数.六：lpszNonceStr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要验证服务端设置的NONCE字符串
 参数.七：lpszOpaqueStr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要验证服务端设置的OPAQUE字符串
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_HTTP_Digest(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszMethod, XCHAR** pptszListHdr, int nHdrCount, LPCXSTR lpszNonceStr = NULL, LPCXSTR lpszOpaqueStr = NULL);
/************************************************************************/
/*                         OAUTH验证导出函数                            */
/************************************************************************/
/********************************************************************
函数名称：Verification_OAuth_Parse
函数功能：解析OAUTH验证请求信息
 参数.一：pSt_OAuthInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出解析到的信息
 参数.二：lpszUrl
  In/Out：Out
  类型：常量字符指针
  可空：Y
  意思：输入请求的URL参数
 参数.三：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入负载数据类型
返回值
  类型：逻辑型
  意思：是否成功
备注：OAuth验证可以是在URL地址或者负载数据.
*********************************************************************/
extern "C" bool Verification_OAuth_Parse(VERIFICATION_OAUTHINFO* pSt_OAuthInfo, LPCXSTR lpszUrl = NULL, LPCXSTR lpszMSGBuffer = NULL);
/********************************************************************
函数名称：Verification_OAuth_PacketToken
函数功能：打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的验证信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包的大小
 参数.三：lpszAccessToken
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要打包的访问token
 参数.四：lpszRefreshToken
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入刷新的TOKEN
 参数.五：lpszTokenType
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入TOKEN类型，默认为Bearer
 参数.六：nExpiredTime
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入token过期时间，默认为3600秒
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_OAuth_PacketToken(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszAccessToken, LPCXSTR lpszRefreshToken = NULL, LPCXSTR lpszTokenType = _X("Bearer"), int nExpiredTime = 3600);
/********************************************************************
函数名称：Verification_OAuth_PacketError
函数功能：错误打包处理函数
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包的大小
 参数.三：lpszOAuthError
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入-OAUTH2错误类型,必须参考头定义的错误信息
 参数.四：lpszErrorDescription
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：错误描述信息
 参数.五：lpszErrorUri
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入错误URI地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_OAuth_PacketError(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszOAuthError, LPCXSTR lpszErrorDescription = NULL, LPCXSTR lpszErrorUri = NULL);