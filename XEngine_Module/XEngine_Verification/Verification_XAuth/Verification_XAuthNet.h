#pragma once
/********************************************************************
//    Created:     2025/09/30  16:54:59
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth\Verification_XAuthNet.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth
//    File Base:   Verification_XAuthNet
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     XAuth网络验证
//    History:
*********************************************************************/
typedef struct
{
	XNETHANDLE xhToken;
	int nDynamicCode;
	time_t nTimeStart;
}AUTHHELP_DYNAMICCODE;

class CVerification_XAuthNet
{
public:
	CVerification_XAuthNet();
	~CVerification_XAuthNet();
public:
	bool Verification_XAuthNet_TryRequest(LPCXSTR lpszURLAddr, LPCXSTR lpszPass = NULL);
	bool Verification_XAuthNet_GetDCode(LPCXSTR lpszURLAddr, int* pInt_DYCode, XNETHANDLE* pxhToken, int* pInt_Timeout = NULL, LPCXSTR lpszPass = NULL);
public:
	bool Verification_XAuthNet_Connect(LPCXSTR lpszClientAddr, int nPort, LPCXSTR lpszPass = NULL);
	bool Verification_XAuthNet_Close();
	bool Verification_XAuthNet_GetAuth();
	bool Verification_XAuthNet_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszHWCode = NULL, XSHOT nDYCode = 0, XNETHANDLE xhToken = 0, XLONG dwCryption = 0);
protected:
	static XHTHREAD XCALLBACK Verification_XAuthNet_Thread(XPVOID lParam);
private:
	bool m_bRun = false;
	bool m_bLogin = false;
	bool m_bAuth = false;
	XSOCKET m_hSocket = 0;
	XCHAR tszPassStr[128] = {};
private:
	std::unique_ptr<std::thread> pSTDThread;
public:
};