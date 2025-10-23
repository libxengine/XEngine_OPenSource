#pragma once
/********************************************************************
//    Created:     2025/09/30  16:44:42
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth\Verification_XAuthKey.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth
//    File Base:   Verification_XAuthKey
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     XAuth 文件KEY验证
//    History:
*********************************************************************/

class CVerification_XAuthKey
{
public:
	CVerification_XAuthKey();
	~CVerification_XAuthKey();
public:
	bool Verification_XAuthKey_FileRead(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszKeyFile, LPCXSTR lpszKeyPass = NULL);
	bool Verification_XAuthKey_FileWrite(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszKeyFile, LPCXSTR lpszKeyPass = NULL);
	bool Verification_XAuthKey_MemoryRead(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, LPCXSTR lpszMSGBuffer, int nMSGLen, LPCXSTR lpszKeyPass = NULL);
	bool Verification_XAuthKey_MemoryWrite(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, XCHAR* ptszMSGBuffer, int* pInt_MSGLen, LPCXSTR lpszKeyPass = NULL);
public:
	bool Verification_XAuthKey_KeyParse(VERIFICATION_XAUTHKEY* pSt_XAuthInfo, ENUM_VERIFICATION_MODULE_HW_TYPE enHWType = ENUM_VERIFICATION_MODULE_HW_TYPE_UNKNOW, LPCXSTR lpszHWCode = NULL);
	bool Verification_XAuthKey_KeyInit(VERIFICATION_XAUTHKEY* pSt_XAuthInfo);
	bool Verification_XAuthKey_KeySerial(XCHAR* ptszSerialStr, int nCount, int nType);
public:
	bool Verification_XAuthKey_UserRegister(VERIFICATION_XAUTHKEY* pSt_AuthLocal, LPCXSTR lpszSerialStr);
	bool Verification_XAuthKey_WriteTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, int nCount = 10);
	bool Verification_XAuthKey_ReadTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, XCHAR*** ppptszTimeList, int* pInt_ListCount);
public:
	bool Verification_XAuthKey_WriteKey(LPCXSTR lpszFileKey, VERIFICATION_XAUTHKEY* pSt_AuthLocal);
	bool Verification_XAuthKey_ReadKey(LPCXSTR lpszFileKey, VERIFICATION_XAUTHKEY* pSt_AuthLocal);
	bool Verification_XAuthKey_WriteMemory(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, VERIFICATION_XAUTHKEY* pSt_AuthLocal);
	bool Verification_XAuthKey_ReadMemory(LPCXSTR lpszMsgBuffer, int nMsgLen, VERIFICATION_XAUTHKEY* pSt_AuthLocal);
protected:
	bool Verification_XAuthKey_BuildKeyTime(VERIFICATION_XAUTHKEY* pSt_AuthLocal, __int64x nDayTimer = 0, XENGINE_LIBTIME* pSt_DayTimer = NULL);
};