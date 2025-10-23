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
	ENUM_VERIFICATION_MODULE_SERIAL_TYPE_UNKNOW = 0,                 //无法识别的充值卡
	ENUM_VERIFICATION_MODULE_SERIAL_TYPE_SECOND = 1,                 //秒钟,本地使用在read和write的时候更新
	ENUM_VERIFICATION_MODULE_SERIAL_TYPE_DAY = 2,                    //天数,本地使用天数卡,不使用不减天数.
	ENUM_VERIFICATION_MODULE_SERIAL_TYPE_TIME = 3,                   //次数卡
	ENUM_VERIFICATION_MODULE_SERIAL_TYPE_CUSTOM = 4                  //自定义过期日期
}ENUM_VERIFICATION_MODULE_SERIAL_TYPE, * LPENUM_VERIFICATION_MODULE_SERIAL_TYPE;
typedef enum
{
	ENUM_VERIFICATION_MODULE_CDKEY_TYPE_UNKNOW = 0,                    //未注册,Authorize_CDKey_GetLeftTimer将返回失败
	ENUM_VERIFICATION_MODULE_CDKEY_TYPE_TEMP = 1,                      //临时,Authorize_CDKey_GetLeftTimer一次后过期,需要Write
	ENUM_VERIFICATION_MODULE_CDKEY_TYPE_TRY = 2,                       //试用
	ENUM_VERIFICATION_MODULE_CDKEY_TYPE_OFFICIAL = 3,                  //正式版
	ENUM_VERIFICATION_MODULE_CDKEY_TYPE_UNLIMIT = 4,                   //无限制版,永不过期.CDKEY不做任何验证
	ENUM_VERIFICATION_MODULE_CDKEY_TYPE_EXPIRED = 5                    //已过期的版本,Authorize_CDKey_GetLeftTimer将返回失败
}ENUM_VERIFICATION_MODULE_CDKEY_TYPE, * LPENUM_VERIFICATION_MODULE_CDKEY_TYPE;
typedef enum
{
	ENUM_VERIFICATION_MODULE_HW_TYPE_UNKNOW = 0,                     //未指定
	ENUM_VERIFICATION_MODULE_HW_TYPE_CPU = 1,                        //CPU序列号
	ENUM_VERIFICATION_MODULE_HW_TYPE_DISK = 2,                       //硬盘序列号
	ENUM_VERIFICATION_MODULE_HW_TYPE_BOARD = 3,                      //主板序列号
	ENUM_VERIFICATION_MODULE_HW_TYPE_SYSTEM = 4,                     //系统序列号,重装系统后改变
	ENUM_VERIFICATION_MODULE_HW_TYPE_MAC = 5                         //网卡MAC地址
}ENUM_VERIFICATION_MODULE_HW_TYPE, * LPENUM_VERIFICATION_MODULE_HW_TYPE;
typedef enum
{
	ENUM_VERIFICATION_MODULE_VERMODE_TYPE_UNKNOW = 0,                 //未知
	ENUM_VERIFICATION_MODULE_VERMODE_TYPE_LOCAL = 0x01,               //本地
	ENUM_VERIFICATION_MODULE_VERMODE_TYPE_NETWORK = 0x02,             //网络
}ENUM_VERIFICATION_MODULE_VERMODE_TYPE, * LPENUM_VERIFICATION_MODULE_VERMODE_TYPE;
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
		ENUM_VERIFICATION_MODULE_SERIAL_TYPE enSerialType;          //过期类型，参考:ENUM_VERIFICATION_MODULE_SERIAL_TYPE
		ENUM_VERIFICATION_MODULE_CDKEY_TYPE enRegType;                //注册类型，参考:ENUM_VERIFICATION_MODULE_CDKEY_TYPE
		ENUM_VERIFICATION_MODULE_HW_TYPE enHWType;                  //硬件类型，参考:ENUM_VERIFICATION_MODULE_HW_TYPE
		ENUM_VERIFICATION_MODULE_VERMODE_TYPE enVModeType;          //验证方式，参考:ENUM_VERIFICATION_MODULE_VERMODE_TYPE 
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

	XCHAR tszTimeList[2048];
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
/************************************************************************/
/*                         XAUTH验证导出函数                            */
/************************************************************************/
/********************************************************************
函数名称：Verification_XAuthKey_FileRead
函数功能：CDKEY帮助读取函数
 参数.一：pSt_XAuthInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的KEY信息
 参数.二：lpszKeyFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要读取的CDKEY文件地址
 参数.三：lpszKeyPass
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：CDKEY的秘钥,如果有
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_XAuthKey_FileRead(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszKeyFile, LPCXSTR lpszKeyPass = NULL);
/********************************************************************
函数名称：Verification_XAuthKey_FileWrite
函数功能：CDKEY帮助写入函数
 参数.一：pSt_XAuthInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要写入的KEY信息
 参数.二：lpszKeyFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要写入的CDKEY文件地址
 参数.三：lpszKeyPass
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：CDKEY的秘钥,如果有
返回值
  类型：逻辑型
  意思：是否成功
备注：无论解析操作是否成功,此函数在结束的时候都需要调用,用来更新CDKEY使用信息.特别是秒数和天数版本
*********************************************************************/
extern "C" bool Verification_XAuthKey_FileWrite(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszKeyFile, LPCXSTR lpszKeyPass = NULL);
extern "C" bool Verification_XAuthKey_MemoryRead(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszMSGBuffer, int nMSGLen, LPCXSTR lpszKeyPass = NULL);
extern "C" bool Verification_XAuthKey_MemoryWrite(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszKeyPass = NULL);
/********************************************************************
函数名称：Verification_XAuthKey_KeyParse
函数功能：解析CDKEY内容,判断是否超时
 参数.一：pSt_OAuthInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出解析到的信息
 参数.二：enHWType
  In/Out：In
  类型：枚举型
  可空：Y
  意思：输入要验证的硬件类型,默认使用主板序列类型
 参数.三：lpszHWCode
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入原始注册的硬件码,此参数传值,参数二将无效
返回值
  类型：逻辑型
  意思：是否成功
备注：无限制版本不做验证
	  其他验证nHasTime将被设置还拥有时间
*********************************************************************/
extern "C" bool Verification_XAuthKey_KeyParse(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, ENUM_VERIFICATION_MODULE_HW_TYPE enHWType = ENUM_VERIFICATION_MODULE_HW_TYPE_UNKNOW, LPCXSTR lpszHWCode = NULL);
/********************************************************************
函数名称：Verification_XAuthKey_KeyInit
函数功能：KEY结构初始化
 参数.一：pSt_XAuthInfo
  In/Out：In/Out
  类型：数据结构指针
  可空：N
  意思：输出初始化后的结构信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_XAuthKey_KeyInit(VERIFICATION_XAUTHKEY* pSt_XAuthInfo);
/********************************************************************
函数名称：Verification_XAuthKey_KeySerial
函数功能：序列号生成函数
 参数.一：ptszSerialStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出生成的序列号
 参数.二：nCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要生成的段数
 参数.三：nType
  In/Out：In
  类型：整数型
  可空：N
  意思：输入生成的类型0随机,1数字,2字母
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_XAuthKey_KeySerial(XCHAR* ptszSerialStr, int nCount, int nType);
/********************************************************************
函数名称：Verification_XAuthKey_UserRegister
函数功能：用户注册CDKEY函数
 参数.一：pSt_AuthLocal
  In/Out：In
  类型：结构体指针
  可空：N
  意思：要构造的结构体
 参数.二：lpszSerialStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入注册验证的序列号
返回值
  类型：逻辑型
  意思：是否构造成功
备注：此函数会修改st_AuthRegInfo的时间信息成员,必须重写CDKEY
*********************************************************************/
extern "C" bool Verification_XAuthKey_UserRegister(VERIFICATION_XAUTHKEY* pSt_AuthLocal, LPCXSTR lpszSerialStr);
/********************************************************************
函数名称：Verification_XAuthKey_WriteTime
函数功能：记录一次执行时间
 参数.一：pSt_AuthLocal
  In/Out：In/Out
  类型：数据结构指针
  可空：N
  意思：输入要操作的结构,输出操作完的结构
 参数.二：nCount
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入最大允许记录个数,0不限制
返回值
  类型：逻辑型
  意思：是否成功
备注：记录次数越多,文件越大.读取需要的内存就越多
*********************************************************************/
extern "C" bool Verification_XAuthKey_WriteTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, int nCount = 10);
/********************************************************************
函数名称：Verification_XAuthKey_ReadTime
函数功能：读取记录的时间列表信息
 参数.一：pSt_AuthLocal
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要操作的结构,输出操作完的结构
 参数.二：ppptszTimeList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出时间信息列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_XAuthKey_ReadTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, XCHAR*** ppptszTimeList, int* pInt_ListCount);
/********************************************************************
函数名称：Verification_XAuthKey_WriteKey
函数功能：写一个CDKey文件
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要操作的文件路径
 参数.二：pSt_AuthLocal
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要写入的文件信息
返回值
  类型：逻辑型
  意思：是否成功
备注：写入读取必须是明文,建议你加密处理CDKEY,通过OPENSSL模块,来加解密,在读写
*********************************************************************/
extern "C" bool Verification_XAuthKey_WriteKey(LPCXSTR lpszFileKey, VERIFICATION_XAUTHKEY* pSt_AuthLocal);
/********************************************************************
函数名称：Verification_XAuthKey_ReadKey
函数功能：读一个数据文件
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要操作的文件路径
 参数.二：pSt_AuthLocal
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出获取到的文件信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_XAuthKey_ReadKey(LPCXSTR lpszFileKey, VERIFICATION_XAUTHKEY* pSt_AuthLocal);
/********************************************************************
函数名称：Verification_XAuthKey_WriteMemory
函数功能：写配置信息到内存
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：写到的内存
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：写到的内存大小
 参数.三：pSt_AuthLocal
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要写的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_XAuthKey_WriteMemory(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, VERIFICATION_XAUTHKEY* pSt_AuthLocal);
/********************************************************************
函数名称：Verification_XAuthKey_ReadMemory
函数功能：内存配置文件读取
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取配置的内存
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入读取内存大小
 参数.三：pSt_AuthLocal
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出读取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_XAuthKey_ReadMemory(LPCXSTR lpszMsgBuffer, int nMsgLen, VERIFICATION_XAUTHKEY* pSt_AuthLocal);
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
extern "C" bool Verification_XAuthNet_TryRequest(LPCXSTR lpszURLAddr, LPCXSTR lpszPass = NULL);
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
extern "C" bool Verification_XAuthNet_GetDCode(LPCXSTR lpszURLAddr, int* pInt_DYCode, XNETHANDLE* pxhToken, int* pInt_Timeout = NULL, LPCXSTR lpszPass = NULL);
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
extern "C" bool Verification_XAuthNet_Connect(LPCXSTR lpszClientAddr, int nPort, LPCXSTR lpszPass = NULL);
/********************************************************************
函数名称：Verification_XAuthNet_Close
函数功能：销毁关闭客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_XAuthNet_Close();
/********************************************************************
函数名称：Verification_XAuthNet_GetAuth
函数功能：验证用户是否登录或者超时
返回值
  类型：逻辑型
  意思：是否通过验证,可能是没有登录或者过期
备注：
*********************************************************************/
extern "C" bool Verification_XAuthNet_GetAuth();
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
extern "C" bool Verification_XAuthNet_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszHWCode = NULL, XSHOT nDYCode = 0, XNETHANDLE xhToken = 0, XLONG dwCryption = 0);
/********************************************************************
函数名称：Verification_XAuthNet_Logout
函数功能：用户登出协议
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
 参数.三：dwCryption
  In/Out：In
  类型：整数型
  可空：Y
  意思：密码加密方法
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Verification_XAuthNet_Logout(LPCXSTR lpszUser, LPCXSTR lpszPass, XLONG dwCryption = 0);