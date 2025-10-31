#include "pch.h"
#include "Verification_XAuthKey.h"
/********************************************************************
//    Created:     2025/09/30  16:47:21
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth\Verification_XAuthKey.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth
//    File Base:   Verification_XAuthKey
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     XAuth 文件KEY验证
//    History:
*********************************************************************/
CVerification_XAuthKey::CVerification_XAuthKey()
{
}
CVerification_XAuthKey::~CVerification_XAuthKey()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
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
bool CVerification_XAuthKey::Verification_XAuthKey_FileRead(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszKeyFile, LPCXSTR lpszKeyPass /* = NULL */)
{
	Verification_IsErrorOccur = false;

	XCHAR tszDECodecBuffer[4096] = {};
	XCHAR tszENCodecBuffer[4096] = {};

	FILE* pSt_File = _xtfopen(lpszKeyFile, _X("rb"));
	if (NULL == pSt_File)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_OPENFILE;
		return false;
	}
	int nRet = fread(tszENCodecBuffer, 1, sizeof(tszENCodecBuffer), pSt_File);
	fclose(pSt_File);

	if (NULL == lpszKeyPass)
	{
		//读取
		if (!Verification_XAuthKey_ReadMemory(tszENCodecBuffer, nRet, pSt_XAuthInfo))
		{
			return false;
		}
	}
	else
	{
		//解密
		if (!Cryption_XCrypto_Decoder(tszENCodecBuffer, &nRet, tszDECodecBuffer, lpszKeyPass))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = Cryption_GetLastError();
			return false;
		}
		//读取
		if (!Verification_XAuthKey_ReadMemory(tszDECodecBuffer, nRet, pSt_XAuthInfo))
		{
			return false;
		}
	}

	if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_DAY == pSt_XAuthInfo->st_AuthRegInfo.enSerialType)
	{
		XENGINE_LIBTIME st_SysTime = {};
		XENGINE_LIBTIME st_EndTime = {};
		BaseLib_Time_GetSysTime(&st_SysTime);
		BaseLib_Time_StrToTime(pSt_XAuthInfo->st_AuthRegInfo.tszStartTime, &st_EndTime);
		if ((st_EndTime.wYear != st_SysTime.wYear) || (st_EndTime.wMonth != st_SysTime.wMonth) || (st_EndTime.wDay != st_SysTime.wDay))
		{
			pSt_XAuthInfo->st_AuthRegInfo.nHasTime--;
			_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszLeftTime, _X("%lld"), pSt_XAuthInfo->st_AuthRegInfo.nHasTime);
			BaseLib_Time_TimeToStr(pSt_XAuthInfo->st_AuthRegInfo.tszStartTime);
		}
	}
	else
	{
		BaseLib_Time_TimeToStr(pSt_XAuthInfo->st_AuthRegInfo.tszStartTime);
	}
	pSt_XAuthInfo->st_AuthAppInfo.nExecTime++;
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_FileWrite(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszKeyFile, LPCXSTR lpszKeyPass /* = NULL */)
{
	Verification_IsErrorOccur = false;

	int nSize = 0;
	XCHAR tszDECodecBuffer[4096] = {};
	XCHAR tszENCodecBuffer[4096] = {};

	//更新使用时间
	if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_SECOND == pSt_XAuthInfo->st_AuthRegInfo.enSerialType)
	{
		XCHAR tszTimeEnd[64] = {};
		__int64x nUsedTime = 0;

		BaseLib_Time_TimeToStr(tszTimeEnd);
		BaseLib_TimeSpan_GetForStr(pSt_XAuthInfo->st_AuthRegInfo.tszStartTime, tszTimeEnd, &nUsedTime, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
		pSt_XAuthInfo->st_AuthRegInfo.nHasTime -= nUsedTime;
		_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszLeftTime, _X("%lld"), pSt_XAuthInfo->st_AuthRegInfo.nHasTime);
	}
	//准备数据
	if (!Verification_XAuthKey_WriteMemory(tszDECodecBuffer, &nSize, pSt_XAuthInfo))
	{
		return false;
	}
	//打开文件
	FILE* pSt_File = _xtfopen(lpszKeyFile, _X("wb"));
	if (NULL == pSt_File)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_OPENFILE;
		return false;
	}
	//写数据
	if (NULL == lpszKeyPass)
	{
		fwrite(tszDECodecBuffer, 1, nSize, pSt_File);
	}
	else
	{
		if (!Cryption_XCrypto_Encoder(tszDECodecBuffer, &nSize, (XBYTE*)tszENCodecBuffer, lpszKeyPass))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = Cryption_GetLastError();
			return false;
		}
		fwrite(tszENCodecBuffer, 1, nSize, pSt_File);
	}
	fclose(pSt_File);

	return true;
}
bool CVerification_XAuthKey::Verification_XAuthKey_MemoryRead(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszMSGBuffer, int nMSGLen, LPCXSTR lpszKeyPass /* = NULL */)
{
	Verification_IsErrorOccur = false;

	int nRet = nMSGLen;
	XCHAR tszDECodecBuffer[4096] = {};
	XCHAR tszENCodecBuffer[4096] = {};

	memcpy(tszENCodecBuffer, lpszMSGBuffer, nMSGLen);

	if (NULL == lpszKeyPass)
	{
		//读取
		if (!Verification_XAuthKey_ReadMemory(tszENCodecBuffer, nRet, pSt_XAuthInfo))
		{
			return false;
		}
	}
	else
	{
		//解密
		if (!Cryption_XCrypto_Decoder(tszENCodecBuffer, &nRet, tszDECodecBuffer, lpszKeyPass))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = Cryption_GetLastError();
			return false;
		}
		//读取
		if (!Verification_XAuthKey_ReadMemory(tszDECodecBuffer, nRet, pSt_XAuthInfo))
		{
			return false;
		}
	}

	if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_DAY == pSt_XAuthInfo->st_AuthRegInfo.enSerialType)
	{
		XENGINE_LIBTIME st_SysTime = {};
		XENGINE_LIBTIME st_EndTime = {};
		BaseLib_Time_GetSysTime(&st_SysTime);
		BaseLib_Time_StrToTime(pSt_XAuthInfo->st_AuthRegInfo.tszStartTime, &st_EndTime);
		if ((st_EndTime.wYear != st_SysTime.wYear) || (st_EndTime.wMonth != st_SysTime.wMonth) || (st_EndTime.wDay != st_SysTime.wDay))
		{
			pSt_XAuthInfo->st_AuthRegInfo.nHasTime--;
			_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszLeftTime, _X("%lld"), pSt_XAuthInfo->st_AuthRegInfo.nHasTime);
			BaseLib_Time_TimeToStr(pSt_XAuthInfo->st_AuthRegInfo.tszStartTime);
		}
	}
	else
	{
		BaseLib_Time_TimeToStr(pSt_XAuthInfo->st_AuthRegInfo.tszStartTime);
	}
	pSt_XAuthInfo->st_AuthAppInfo.nExecTime++;
	return true;
}
bool CVerification_XAuthKey::Verification_XAuthKey_MemoryWrite(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszKeyPass /* = NULL */)
{
	Verification_IsErrorOccur = false;

	int nSize = 0;
	XCHAR tszDECodecBuffer[4096] = {};
	//更新使用时间
	if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_SECOND == pSt_XAuthInfo->st_AuthRegInfo.enSerialType)
	{
		XCHAR tszTimeEnd[64] = {};
		__int64x nUsedTime = 0;

		BaseLib_Time_TimeToStr(tszTimeEnd);
		BaseLib_TimeSpan_GetForStr(pSt_XAuthInfo->st_AuthRegInfo.tszStartTime, tszTimeEnd, &nUsedTime, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
		pSt_XAuthInfo->st_AuthRegInfo.nHasTime -= nUsedTime;
		_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszLeftTime, _X("%lld"), pSt_XAuthInfo->st_AuthRegInfo.nHasTime);
	}
	//准备数据
	if (!Verification_XAuthKey_WriteMemory(tszDECodecBuffer, &nSize, pSt_XAuthInfo))
	{
		return false;
	}
	//写数据
	if (NULL == lpszKeyPass)
	{
		memcpy(ptszMSGBuffer, tszDECodecBuffer, nSize);
	}
	else
	{
		if (!Cryption_XCrypto_Encoder(tszDECodecBuffer, &nSize, (XBYTE*)ptszMSGBuffer, lpszKeyPass))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = Cryption_GetLastError();
			return false;
		}
	}
	*pInt_MSGLen = nSize;
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_KeyParse(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, ENUM_VERIFICATION_MODULE_HW_TYPE enHWType /* = ENUM_VERIFICATION_MODULE_HW_TYPE_UNKNOW */, LPCXSTR lpszHWCode /* = NULL */)
{
	Verification_IsErrorOccur = false;

	if (NULL == pSt_XAuthInfo)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	//////////////////////////////////////////////////////////////////////////验证
	//验证CDKEY本身情况
	XENGINE_LIBTIME st_EndTimer = {};
	XENGINE_LIBTIME st_SysTimer = {};
	//处理注册类型
	if (ENUM_VERIFICATION_MODULE_CDKEY_TYPE_UNLIMIT == pSt_XAuthInfo->st_AuthRegInfo.enRegType)
	{
		return true;
	}
	else if (ENUM_VERIFICATION_MODULE_CDKEY_TYPE_UNKNOW == pSt_XAuthInfo->st_AuthRegInfo.enRegType)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_UNKNOW;
		return false;
	}
	else if (ENUM_VERIFICATION_MODULE_CDKEY_TYPE_TEMP == pSt_XAuthInfo->st_AuthRegInfo.enRegType)
	{
		pSt_XAuthInfo->st_AuthRegInfo.enRegType = ENUM_VERIFICATION_MODULE_CDKEY_TYPE_EXPIRED;
		_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszExpiryTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), st_SysTimer.wYear, st_SysTimer.wMonth, st_SysTimer.wDay, st_SysTimer.wHour, st_SysTimer.wMinute, st_SysTimer.wSecond);
	}
	else if (ENUM_VERIFICATION_MODULE_CDKEY_TYPE_EXPIRED == pSt_XAuthInfo->st_AuthRegInfo.enRegType)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_EXPIRED;
		return false;
	}
	else
	{
		//其他类型的注册机制
		BaseLib_Time_GetSysTime(&st_SysTimer);
		if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_SECOND == pSt_XAuthInfo->st_AuthRegInfo.enSerialType)
		{
			XCHAR tszTimeEnd[64] = {};
			__int64x nUsedTime = 0;

			BaseLib_Time_TimeToStr(tszTimeEnd);
			BaseLib_TimeSpan_GetForStr(pSt_XAuthInfo->st_AuthRegInfo.tszStartTime, tszTimeEnd, &nUsedTime, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
			if (nUsedTime <= 0)
			{
				pSt_XAuthInfo->st_AuthRegInfo.nHasTime = nUsedTime;
			}
		}
		else if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_TIME == pSt_XAuthInfo->st_AuthRegInfo.enSerialType)
		{
			pSt_XAuthInfo->st_AuthRegInfo.nHasTime--;
		}
		else if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_CUSTOM == pSt_XAuthInfo->st_AuthRegInfo.enSerialType)
		{
			_stxscanf(pSt_XAuthInfo->st_AuthRegInfo.tszLeftTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), &st_EndTimer.wYear, &st_EndTimer.wMonth, &st_EndTimer.wDay, &st_EndTimer.wHour, &st_EndTimer.wMinute, &st_EndTimer.wSecond);
			BaseLib_TimeSpan_GetForStu(&st_SysTimer, &st_EndTimer, &pSt_XAuthInfo->st_AuthRegInfo.nHasTime, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
		}
		else
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_NOTSUPPORT;
			return false;
		}
		if (pSt_XAuthInfo->st_AuthRegInfo.nHasTime < 0)
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_EXPIRED;
			pSt_XAuthInfo->st_AuthRegInfo.enRegType = ENUM_VERIFICATION_MODULE_CDKEY_TYPE_EXPIRED;
			_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszExpiryTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), st_SysTimer.wYear, st_SysTimer.wMonth, st_SysTimer.wDay, st_SysTimer.wHour, st_SysTimer.wMinute, st_SysTimer.wSecond);
			return false;
		}
	}
	//是否有自定义硬件码,如果有,那么可以不用关心硬件类型
	if (NULL == lpszHWCode)
	{
		SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial = {};
		SystemApi_HardWare_GetSerial(&st_SDKSerial);

		if (ENUM_VERIFICATION_MODULE_HW_TYPE_UNKNOW == enHWType)
		{
			//采用默认主板序列验证
			if (ENUM_VERIFICATION_MODULE_HW_TYPE_BOARD != pSt_XAuthInfo->st_AuthRegInfo.enHWType)
			{
				Verification_IsErrorOccur = true;
				Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_HWTYPE;
				return false;
			}
			if (0 != _tcsxnicmp(st_SDKSerial.tszBoardSerial, pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _tcsxlen(st_SDKSerial.tszBoardSerial)))
			{
				Verification_IsErrorOccur = true;
				Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_SERIAL;
				return false;
			}
		}
		else
		{
			//验证硬件类型是否匹配
			if (enHWType == pSt_XAuthInfo->st_AuthRegInfo.enHWType)
			{
				if (ENUM_VERIFICATION_MODULE_HW_TYPE_CPU == enHWType)
				{
					if (0 != _tcsxnicmp(st_SDKSerial.tszCPUSerial, pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _tcsxlen(st_SDKSerial.tszCPUSerial)))
					{
						Verification_IsErrorOccur = true;
						Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_SERIAL;
						return false;
					}
				}
				else if (ENUM_VERIFICATION_MODULE_HW_TYPE_DISK == enHWType)
				{
					if (0 != _tcsxnicmp(st_SDKSerial.tszDiskSerial, pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _tcsxlen(st_SDKSerial.tszDiskSerial)))
					{
						Verification_IsErrorOccur = true;
						Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_SERIAL;
						return false;
					}
				}
				else if (ENUM_VERIFICATION_MODULE_HW_TYPE_BOARD == enHWType)
				{
					if (0 != _tcsxnicmp(st_SDKSerial.tszBoardSerial, pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _tcsxlen(st_SDKSerial.tszBoardSerial)))
					{
						Verification_IsErrorOccur = true;
						Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_SERIAL;
						return false;
					}
				}
				else if (ENUM_VERIFICATION_MODULE_HW_TYPE_SYSTEM == enHWType)
				{
					if (0 != _tcsxnicmp(st_SDKSerial.tszSystemSerial, pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _tcsxlen(st_SDKSerial.tszSystemSerial)))
					{
						Verification_IsErrorOccur = true;
						Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_SERIAL;
						return false;
					}
				}
				else
				{
					Verification_IsErrorOccur = true;
					Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_NOTSUPPORT;
					return false;
				}
			}
			else
			{
				Verification_IsErrorOccur = true;
				Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_HWTYPE;
				return false;
			}
		}
	}
	else
	{
		if (0 != _tcsxnicmp(lpszHWCode, pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _tcsxlen(lpszHWCode)))
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_SERIAL;
			return false;
		}
	}
	
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_KeyInit(VERIFICATION_XAUTHKEY* pSt_XAuthInfo)
{
	Verification_IsErrorOccur = false;

	if (NULL == pSt_XAuthInfo)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	if (_tcsxlen(pSt_XAuthInfo->tszAddr) <= 0)
	{
		_xstprintf(pSt_XAuthInfo->tszAddr, _X("http://app.libxengine.com"));
	}
	if (0 == pSt_XAuthInfo->nPort)
	{
		pSt_XAuthInfo->nPort = 5302;
	}
	//应用信息
	pSt_XAuthInfo->st_AuthAppInfo.bInit = true;
	if (_tcsxlen(pSt_XAuthInfo->st_AuthAppInfo.tszAppName) <= 0)
	{
		_xstprintf(pSt_XAuthInfo->st_AuthAppInfo.tszAppName, _X("XEngine_AuthorzeApp"));
	}
	if (_tcsxlen(pSt_XAuthInfo->st_AuthAppInfo.tszAppVer) <= 0)
	{
		_xstprintf(pSt_XAuthInfo->st_AuthAppInfo.tszAppVer, _X("V1.0.0.1001"));
	}
	//注册信息
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial = {};
	SystemApi_HardWare_GetSerial(&st_SDKSerial);

	BaseLib_Time_TimeToStr(pSt_XAuthInfo->st_AuthRegInfo.tszCreateTime);
	if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_UNKNOW == pSt_XAuthInfo->st_AuthRegInfo.enSerialType)
	{
		pSt_XAuthInfo->st_AuthRegInfo.enSerialType = ENUM_VERIFICATION_MODULE_SERIAL_TYPE_TIME;
		pSt_XAuthInfo->st_AuthRegInfo.nHasTime = 3;
	}
	_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszLeftTime, _X("%lld"), pSt_XAuthInfo->st_AuthRegInfo.nHasTime);

	if (ENUM_VERIFICATION_MODULE_CDKEY_TYPE_UNKNOW == pSt_XAuthInfo->st_AuthRegInfo.enRegType)
	{
		pSt_XAuthInfo->st_AuthRegInfo.enRegType = ENUM_VERIFICATION_MODULE_CDKEY_TYPE_TRY;
	}
	if (ENUM_VERIFICATION_MODULE_HW_TYPE_UNKNOW == pSt_XAuthInfo->st_AuthRegInfo.enHWType)
	{
		pSt_XAuthInfo->st_AuthRegInfo.enHWType = ENUM_VERIFICATION_MODULE_HW_TYPE_BOARD;
		_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _X("%s"), st_SDKSerial.tszBoardSerial);
	}
	else
	{
		if (ENUM_VERIFICATION_MODULE_HW_TYPE_CPU == pSt_XAuthInfo->st_AuthRegInfo.enHWType)
		{
			if (_tcsxlen(pSt_XAuthInfo->st_AuthRegInfo.tszHardware) == 0)
			{
				_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _X("%s"), st_SDKSerial.tszCPUSerial);
			}
		}
		else if (ENUM_VERIFICATION_MODULE_HW_TYPE_DISK == pSt_XAuthInfo->st_AuthRegInfo.enHWType)
		{
			if (_tcsxlen(pSt_XAuthInfo->st_AuthRegInfo.tszHardware) == 0)
			{
				_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _X("%s"), st_SDKSerial.tszDiskSerial);
			}
		}
		else if (ENUM_VERIFICATION_MODULE_HW_TYPE_BOARD == pSt_XAuthInfo->st_AuthRegInfo.enHWType)
		{
			if (_tcsxlen(pSt_XAuthInfo->st_AuthRegInfo.tszHardware) == 0)
			{
				_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _X("%s"), st_SDKSerial.tszBoardSerial);
			}
		}
		else if (ENUM_VERIFICATION_MODULE_HW_TYPE_SYSTEM == pSt_XAuthInfo->st_AuthRegInfo.enHWType)
		{
			if (_tcsxlen(pSt_XAuthInfo->st_AuthRegInfo.tszHardware) == 0)
			{
				_xstprintf(pSt_XAuthInfo->st_AuthRegInfo.tszHardware, _X("%s"), st_SDKSerial.tszSystemSerial);
			}
		}
		else
		{
			Verification_IsErrorOccur = true;
			Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_NOTSUPPORT;
			return false;
		}
	}
	if (ENUM_VERIFICATION_MODULE_VERMODE_TYPE_UNKNOW == pSt_XAuthInfo->st_AuthRegInfo.enVModeType)
	{
		pSt_XAuthInfo->st_AuthRegInfo.enVModeType = ENUM_VERIFICATION_MODULE_VERMODE_TYPE_LOCAL;
	}
	//临时序列号
	if (0 == pSt_XAuthInfo->st_AuthSerial.st_TimeLimit.nTimeCount)
	{
		pSt_XAuthInfo->st_AuthSerial.st_TimeLimit.nTimeCount = 9999;
	}
	if (_tcsxlen(pSt_XAuthInfo->st_AuthSerial.st_TimeLimit.tszTimeSerial) <= 0)
	{
		Verification_XAuthKey_KeySerial(pSt_XAuthInfo->st_AuthSerial.st_TimeLimit.tszTimeSerial, 7, 0);
	}
	pSt_XAuthInfo->st_AuthSerial.st_DataLimit.bTimeAdd = true;
	if (_tcsxlen(pSt_XAuthInfo->st_AuthSerial.st_DataLimit.tszDataSerial) <= 0)
	{
		XENGINE_LIBTIME st_LibTime = {};
		BaseLib_Time_GetSysTime(&st_LibTime);
		st_LibTime.wYear += 1;
		BaseLib_Time_TimeToStr(pSt_XAuthInfo->st_AuthSerial.st_DataLimit.tszDataTime, NULL, true, &st_LibTime);
		Verification_XAuthKey_KeySerial(pSt_XAuthInfo->st_AuthSerial.st_DataLimit.tszDataSerial, 7, 0);
	}
	if (_tcsxlen(pSt_XAuthInfo->st_AuthSerial.st_UNLimit.tszUNLimitSerial) <= 0)
	{
		Verification_XAuthKey_KeySerial(pSt_XAuthInfo->st_AuthSerial.st_UNLimit.tszUNLimitSerial, 7, 0);
	}
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_KeySerial(XCHAR* ptszSerialStr, int nCount, int nType)
{
	Verification_IsErrorOccur = false;

	for (int i = 0; i < nCount; i++)
	{
		XCHAR tszRandomStr[6] = {};

		if (0 != i)
		{
			_tcsxcat(ptszSerialStr, _X("-"));
		}
		
		if (0 == nType)
		{
			BaseLib_Handle_CreateStr(tszRandomStr, 5, nType, 1);
			_tcsxcat(ptszSerialStr, tszRandomStr);
		}
		else if (1 == nType)
		{
			BaseLib_Handle_CreateStr(tszRandomStr, 5, nType, 1);
			_tcsxcat(ptszSerialStr, tszRandomStr);
		}
		else
		{
			BaseLib_Handle_CreateStr(tszRandomStr, 5, nType, 1);
			_tcsxcat(ptszSerialStr, tszRandomStr);
		}
	}
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_UserRegister(VERIFICATION_XAUTHKEY* pSt_AuthLocal, LPCXSTR lpszSerialStr)
{
	Verification_IsErrorOccur = false;

	if (NULL == pSt_AuthLocal || NULL == lpszSerialStr)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	bool bSerial = false;

	if (0 == _tcsxncmp(pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial, lpszSerialStr, _tcsxlen(pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial)))
	{
		//次数验证
		bSerial = true;
		pSt_AuthLocal->st_AuthRegInfo.enSerialType = ENUM_VERIFICATION_MODULE_SERIAL_TYPE_TIME;
		pSt_AuthLocal->st_AuthRegInfo.enRegType = ENUM_VERIFICATION_MODULE_CDKEY_TYPE_OFFICIAL;
		Verification_XAuthKey_BuildKeyTime(pSt_AuthLocal, pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount);
	}
	else if (0 == _tcsxncmp(pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial, lpszSerialStr, _tcsxlen(pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial)))
	{
		//日期序列
		bSerial = true;
		pSt_AuthLocal->st_AuthRegInfo.enSerialType = ENUM_VERIFICATION_MODULE_SERIAL_TYPE_CUSTOM;
		pSt_AuthLocal->st_AuthRegInfo.enRegType = ENUM_VERIFICATION_MODULE_CDKEY_TYPE_OFFICIAL;
		XENGINE_LIBTIME st_LibTime = {};
		BaseLib_Time_GetSysTime(&st_LibTime);
		if (pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd)
		{
			__int64x nTimeCount = 0;
			XENGINE_LIBTIME st_AddTime = {};

			BaseLib_TimeSpan_GetForStr(pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime, pSt_AuthLocal->st_AuthRegInfo.tszCreateTime, &nTimeCount);
			st_AddTime.wDay = (int)nTimeCount;
			BaseLib_TimeSpan_CalForStu(&st_LibTime, &st_AddTime);

			Verification_XAuthKey_BuildKeyTime(pSt_AuthLocal, 0, &st_AddTime);
		}
		else
		{
			__int64x nTimeCount = 0;
			XCHAR tszTimeStr[128] = {};
			XENGINE_LIBTIME st_LibTime = {};
			BaseLib_Time_TimeToStr(tszTimeStr);

			BaseLib_TimeSpan_GetForStr(tszTimeStr, pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime, &nTimeCount);
			if (nTimeCount <= 0)
			{
				Verification_IsErrorOccur = true;
				Verification_dwErrorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_EXPIRED;
				return false;
			}
			BaseLib_Time_StrToTime(pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime, &st_LibTime);
			Verification_XAuthKey_BuildKeyTime(pSt_AuthLocal, 0, &st_LibTime);
		}
	}
	else if (0 == _tcsxncmp(pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial, lpszSerialStr, _tcsxlen(pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial)))
	{
		//无限制
		bSerial = true;
		pSt_AuthLocal->st_AuthRegInfo.enRegType = ENUM_VERIFICATION_MODULE_CDKEY_TYPE_UNLIMIT;
	}

	if (!bSerial)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_FAILED;
		return false;
	}
	BaseLib_Time_TimeToStr(pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime);
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_WriteTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, int nCount /* = 10 */)
{
	Verification_IsErrorOccur = false;

	if (NULL == pSt_AuthLocal)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	//如果设置了最大个数
	std::list<xstring> stl_ListTime;
	//分割字符串
	XCHAR tszMSGBuffer[2048] = {};
	_tcsxcpy(tszMSGBuffer, pSt_AuthLocal->tszTimeList);
	//申请内存
	XCHAR* ptszTokStr = _tcsxtok(tszMSGBuffer, _X("|"));
	while (NULL != ptszTokStr)
	{
		if (_tcsxlen(ptszTokStr) > 4)
		{
			stl_ListTime.push_back(ptszTokStr);
		}
		ptszTokStr = _tcsxtok(NULL, _X("|"));
	}
	//移除多余的
	if (nCount > 0)
	{
		while ((int)stl_ListTime.size() >= nCount)
		{
			stl_ListTime.pop_front();
		}
	}
	XCHAR tszTimeStr[64] = {};
#ifdef _MSC_BUILD
	_xstprintf(tszTimeStr, _X("%lld|"), time(NULL));
#else
	_xstprintf(tszTimeStr, _X("%ld|"), time(NULL));
#endif
	//追加
	_tcsxcat(pSt_AuthLocal->tszTimeList, tszTimeStr);
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_ReadTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, XCHAR*** ppptszTimeList, int* pInt_ListCount)
{
	Verification_IsErrorOccur = false;

	if ((NULL == pSt_AuthLocal) || (NULL == pInt_ListCount))
	{
		return false;
	}
	std::list<xstring> stl_ListTime;
	//分割字符串
	XCHAR tszMSGBuffer[2048] = {};
	_tcsxcpy(tszMSGBuffer, pSt_AuthLocal->tszTimeList);
	//申请内存
	XCHAR* ptszTokStr = _tcsxtok(tszMSGBuffer, _X("|"));
	while (NULL != ptszTokStr)
	{
		if (_tcsxlen(ptszTokStr) > 4)
		{
			stl_ListTime.push_back(ptszTokStr);
		}
		ptszTokStr = _tcsxtok(NULL, _X("|"));
	}
	//导出
	*pInt_ListCount = (int)stl_ListTime.size();
	BaseLib_Memory_Malloc((XPPPMEM)ppptszTimeList, *pInt_ListCount, 64);
	std::list<xstring>::const_iterator stl_ListIterator = stl_ListTime.begin();
	for (int i = 0; stl_ListIterator != stl_ListTime.end(); stl_ListIterator++, i++)
	{
		_tcsxcpy((*ppptszTimeList)[i], stl_ListIterator->c_str());
	}
	stl_ListTime.clear();
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_WriteKey(LPCXSTR lpszFileKey, VERIFICATION_XAUTHKEY* pSt_AuthLocal)
{
	Verification_IsErrorOccur = false;

	if ((NULL == lpszFileKey) || (NULL == pSt_AuthLocal))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	//添加连接信息文本
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("Connection"), _X("tszAddr"), pSt_AuthLocal->tszAddr))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("Connection"), _X("nPort"), pSt_AuthLocal->nPort))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//添加程序名称文本
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AppInfo"), _X("tszAppName"), pSt_AuthLocal->st_AuthAppInfo.tszAppName))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AppInfo"), _X("tszAppVer"), pSt_AuthLocal->st_AuthAppInfo.tszAppVer))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AppInfo"), _X("nExecTime"), pSt_AuthLocal->st_AuthAppInfo.nExecTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AppInfo"), _X("bInit"), pSt_AuthLocal->st_AuthAppInfo.bInit))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//添加注册信息
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AuthReg"), _X("nHasTime"), pSt_AuthLocal->st_AuthRegInfo.nHasTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AuthReg"), _X("enHWType"), pSt_AuthLocal->st_AuthRegInfo.enHWType))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AuthReg"), _X("enRegType"), pSt_AuthLocal->st_AuthRegInfo.enRegType))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AuthReg"), _X("enSerialType"), pSt_AuthLocal->st_AuthRegInfo.enSerialType))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AuthReg"), _X("enVModeType"), pSt_AuthLocal->st_AuthRegInfo.enVModeType))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszHardware"), pSt_AuthLocal->st_AuthRegInfo.tszHardware))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszCreateTime"), pSt_AuthLocal->st_AuthRegInfo.tszCreateTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszRegisterTime"), pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszLeftTime"), pSt_AuthLocal->st_AuthRegInfo.tszLeftTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszStartTime"), pSt_AuthLocal->st_AuthRegInfo.tszStartTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszExpiryTime"), pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//临时序列号
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszTimeSerial"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AuthSerial"), _X("nTimeCount"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszDataSerial"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszDataTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AuthSerial"), _X("bTimeAdd"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszUNLimitSerial"), pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//自定义用户信息
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszUserName"), pSt_AuthLocal->st_AuthUserInfo.tszUserName))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszUserContact"), pSt_AuthLocal->st_AuthUserInfo.tszUserContact))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszCustom"), pSt_AuthLocal->st_AuthUserInfo.tszCustom))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("TimeList"), _X("TimeNumber"), pSt_AuthLocal->tszTimeList))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_ReadKey(LPCXSTR lpszFileKey, VERIFICATION_XAUTHKEY* pSt_AuthLocal)
{
	Verification_IsErrorOccur = false;

	if ((NULL == lpszFileKey) || (NULL == pSt_AuthLocal))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	//添加连接信息文本
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("Connection"), _X("tszAddr"), pSt_AuthLocal->tszAddr) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	pSt_AuthLocal->nPort = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("Connection"), _X("nPort"));
	//添加程序名称文本
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AppInfo"), _X("tszAppName"), pSt_AuthLocal->st_AuthAppInfo.tszAppName) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AppInfo"), _X("tszAppVer"), pSt_AuthLocal->st_AuthAppInfo.tszAppVer) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	pSt_AuthLocal->st_AuthAppInfo.nExecTime = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AppInfo"), _X("nExecTime"));
	pSt_AuthLocal->st_AuthAppInfo.bInit = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AppInfo"), _X("bInit"));
	//添加注册信息
	pSt_AuthLocal->st_AuthRegInfo.enHWType = (ENUM_VERIFICATION_MODULE_HW_TYPE)SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthReg"), _X("enHWType"));
	pSt_AuthLocal->st_AuthRegInfo.enRegType = (ENUM_VERIFICATION_MODULE_CDKEY_TYPE)SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthReg"), _X("enRegType"));
	pSt_AuthLocal->st_AuthRegInfo.enSerialType = (ENUM_VERIFICATION_MODULE_SERIAL_TYPE)SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthReg"), _X("enSerialType"));
	pSt_AuthLocal->st_AuthRegInfo.enVModeType = (ENUM_VERIFICATION_MODULE_VERMODE_TYPE)SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthReg"), _X("enVModeType"));
	pSt_AuthLocal->st_AuthRegInfo.nHasTime = SystemConfig_File_ReadInt64FromFile(lpszFileKey, _X("AuthReg"), _X("nHasTime"));
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszHardware"), pSt_AuthLocal->st_AuthRegInfo.tszHardware) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszCreateTime"), pSt_AuthLocal->st_AuthRegInfo.tszCreateTime) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszRegisterTime"), pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime) < 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszLeftTime"), pSt_AuthLocal->st_AuthRegInfo.tszLeftTime) < 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszStartTime"), pSt_AuthLocal->st_AuthRegInfo.tszStartTime) < 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszExpiryTime"), pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime) < 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//临时序列号
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszTimeSerial"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthSerial"), _X("nTimeCount"));
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszDataSerial"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszDataTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthSerial"), _X("bAddTime"));
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszUNLimitSerial"), pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//用户信息
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszUserName"), pSt_AuthLocal->st_AuthUserInfo.tszUserName) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszUserContact"), pSt_AuthLocal->st_AuthUserInfo.tszUserContact) <= 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszCustom"), pSt_AuthLocal->st_AuthUserInfo.tszCustom) < 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("TimeList"), _X("TimeNumber"), pSt_AuthLocal->tszTimeList) < 0)
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_WriteMemory(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, VERIFICATION_XAUTHKEY* pSt_AuthLocal)
{
	Verification_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen) || (NULL == pSt_AuthLocal))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	int nMsgLen = 0;
	//添加连接信息文本
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("Connection"), _X("tszAddr"), pSt_AuthLocal->tszAddr, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("Connection"), _X("nPort"), pSt_AuthLocal->nPort, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//添加程序名称文本
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AppInfo"), _X("tszAppName"), pSt_AuthLocal->st_AuthAppInfo.tszAppName, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AppInfo"), _X("tszAppVer"), pSt_AuthLocal->st_AuthAppInfo.tszAppVer, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AppInfo"), _X("nExecTime"), pSt_AuthLocal->st_AuthAppInfo.nExecTime, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AppInfo"), _X("bInit"), pSt_AuthLocal->st_AuthAppInfo.bInit, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//添加注册信息
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("nHasTime"), pSt_AuthLocal->st_AuthRegInfo.nHasTime, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enHWType"), pSt_AuthLocal->st_AuthRegInfo.enHWType, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enRegType"), pSt_AuthLocal->st_AuthRegInfo.enRegType, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enSerialType"), pSt_AuthLocal->st_AuthRegInfo.enSerialType, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enVModeType"), pSt_AuthLocal->st_AuthRegInfo.enVModeType, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszHardware"), pSt_AuthLocal->st_AuthRegInfo.tszHardware, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszCreateTime"), pSt_AuthLocal->st_AuthRegInfo.tszCreateTime, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszRegisterTime"), pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszLeftTime"), pSt_AuthLocal->st_AuthRegInfo.tszLeftTime, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszStartTime"), pSt_AuthLocal->st_AuthRegInfo.tszStartTime, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszExpiryTime"), pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//临时序列号
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszTimeSerial"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("nTimeCount"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszDataSerial"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszDataTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("bAddTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszUNLimitSerial"), pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//用户信息
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszUserName"), pSt_AuthLocal->st_AuthUserInfo.tszUserName, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszUserContact"), pSt_AuthLocal->st_AuthUserInfo.tszUserContact, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszCustom"), pSt_AuthLocal->st_AuthUserInfo.tszCustom, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("TimeList"), _X("TimeNumber"), pSt_AuthLocal->tszTimeList, ptszMsgBuffer, &nMsgLen))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	*pInt_MsgLen = nMsgLen;
	return true;
}
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
bool CVerification_XAuthKey::Verification_XAuthKey_ReadMemory(LPCXSTR lpszMsgBuffer, int nMsgLen, VERIFICATION_XAUTHKEY* pSt_AuthLocal)
{
	Verification_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_AuthLocal))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = ERROR_XENGINE_MODULE_VERIFICATION_XAUTH_PARAMENT;
		return false;
	}
	//添加连接信息文本
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("Connection"), _X("tszAddr"), pSt_AuthLocal->tszAddr))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("Connection"), _X("nPort"), &pSt_AuthLocal->nPort))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//添加程序名称文本
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AppInfo"), _X("tszAppName"), pSt_AuthLocal->st_AuthAppInfo.tszAppName))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AppInfo"), _X("tszAppVer"), pSt_AuthLocal->st_AuthAppInfo.tszAppVer))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AppInfo"), _X("bInit"), (int*)&pSt_AuthLocal->st_AuthAppInfo.bInit);
	SystemConfig_File_ReadInt64FromMemory(lpszMsgBuffer, nMsgLen, _X("AppInfo"), _X("nExecTime"), &pSt_AuthLocal->st_AuthAppInfo.nExecTime);
	//添加注册信息
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enHWType"), (int*)&pSt_AuthLocal->st_AuthRegInfo.enHWType);
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enRegType"), (int*)&pSt_AuthLocal->st_AuthRegInfo.enRegType);
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enSerialType"), (int*)&pSt_AuthLocal->st_AuthRegInfo.enSerialType);
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enVModeType"), (int*)&pSt_AuthLocal->st_AuthRegInfo.enVModeType);
	SystemConfig_File_ReadInt64FromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("nHasTime"), &pSt_AuthLocal->st_AuthRegInfo.nHasTime);

	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszHardware"), pSt_AuthLocal->st_AuthRegInfo.tszHardware))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszCreateTime"), pSt_AuthLocal->st_AuthRegInfo.tszCreateTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszRegisterTime"), pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszLeftTime"), pSt_AuthLocal->st_AuthRegInfo.tszLeftTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszStartTime"), pSt_AuthLocal->st_AuthRegInfo.tszStartTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszExpiryTime"), pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//序列号信息
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszTimeSerial"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("nTimeCount"), &pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszDataSerial"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszDataTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("bAddTime"), (int*)&pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd);
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszUNLimitSerial"), pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	//用户信息
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszUserName"), pSt_AuthLocal->st_AuthUserInfo.tszUserName))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszUserContact"), pSt_AuthLocal->st_AuthUserInfo.tszUserContact))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszCustom"), pSt_AuthLocal->st_AuthUserInfo.tszCustom))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("TimeList"), _X("TimeNumber"), pSt_AuthLocal->tszTimeList))
	{
		Verification_IsErrorOccur = true;
		Verification_dwErrorCode = SystemConfig_GetLastError();
		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Verification_XAuthKey_BuildKeyTime
函数功能：构造注册的时间结构体信息
 参数.一：pSt_AuthLocal
  In/Out：In
  类型：结构体指针
  可空：N
  意思：要构造的结构体
 参数.二：nDayTimer
  In/Out：In
  类型：整数型
  可空：Y
  意思：可用时间或者次数.非自定义时间需要设置此值
 参数.三：pSt_DayTimer
  In/Out：In
  类型：结构体指针
  可空：Y
  意思：如果是自定义时间,这个参数需要设置,其他类型请设置参数二
返回值
  类型：逻辑型
  意思：是否构造成功
备注：此函数会修改st_AuthRegInfo的时间信息成员,必须重写CDKEY
*********************************************************************/
bool CVerification_XAuthKey::Verification_XAuthKey_BuildKeyTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, __int64x nDayTimer /* = 0 */, XENGINE_LIBTIME* pSt_DayTimer /* = NULL */)
{
	Verification_IsErrorOccur = false;

	//判断注册时间方式
	if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_CUSTOM == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
	{
		//按照到期时间计算
		_xstprintf(pSt_AuthLocal->st_AuthRegInfo.tszLeftTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), pSt_DayTimer->wYear, pSt_DayTimer->wMonth, pSt_DayTimer->wDay, pSt_DayTimer->wHour, pSt_DayTimer->wMinute, pSt_DayTimer->wSecond);
	}
	else
	{
		pSt_AuthLocal->st_AuthRegInfo.nHasTime = nDayTimer;
		_xstprintf(pSt_AuthLocal->st_AuthRegInfo.tszLeftTime, _X("%lld"), nDayTimer);
	}
	return true;
}