#pragma once
/********************************************************************
//    Created:     2025/07/10  15:45:17
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Help\AIApi_Help.h
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Help
//    File Base:   AIApi_Help
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     AI帮助类
//    History:
*********************************************************************/

class CAIApi_Help
{
public:
	CAIApi_Help();
	~CAIApi_Help();
public:
	bool AIApi_Help_Base64EncodecFile(LPCXSTR lpszFileName, XCHAR** pptszMSGBuffer, int* pInt_MSGLen);
	bool AIApi_Help_Base64EncodecMemory(LPCXSTR lpszFileExt, LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR** pptszMSGBuffer, int* pInt_MSGLen);
	bool AIApi_Help_Base64DecodecFile(LPCXSTR lpszMSGBuffer, int nMSGLen, LPCXSTR lpszFileName);
	bool AIApi_Help_Base64DecodecMemory(LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen);
protected:
private:
};