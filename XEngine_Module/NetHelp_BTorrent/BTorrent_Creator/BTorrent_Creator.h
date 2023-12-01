﻿#pragma once
/********************************************************************
//    Created:     2023/02/02  16:25:43
//    File Name:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Creator\BTorrent_Creator.h
//    File Path:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Creator
//    File Base:   BTorrent_Creator
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     BT创建器
//    History:
*********************************************************************/
typedef struct  
{
	XCHAR tszComment[4096];
	XCHAR tszPEMCert[4096];
	XCHAR tszBTPath[MAX_PATH];
	XCHAR tszCreate[MAX_PATH];
	int m_nPieceSize;

	list<BTORRENT_PARSEMAP>* pStl_ListNode;
	list<BTORRENT_PARSEMAP>* pStl_ListTracker;
	list<BTORRENT_PARSEMAP>* pStl_ListSeeds;
}BTORRENT_CREATORINFO;

class CBTorrent_Creator
{
public:
	CBTorrent_Creator();
	~CBTorrent_Creator();
public:
	XBOOL BTorrent_Creator_Init(XNETHANDLE* pxhToken, LPCXSTR lpszBTPath, int nPieceSize = 0);
	XBOOL BTorrent_Creator_AddNode(XNETHANDLE xhToken, LPCXSTR lpszAddr, int nIndex);
	XBOOL BTorrent_Creator_AddTracker(XNETHANDLE xhToken, LPCXSTR lpszAddr, int nIndex = 0);
	XBOOL BTorrent_Creator_AddSeeds(XNETHANDLE xhToken, LPCXSTR lpszAddr, XBOOL bSingle = FALSE);
	XBOOL BTorrent_Creator_SetInfo(XNETHANDLE xhToken, LPCXSTR lpszCreator, LPCXSTR lpszComment = NULL, LPCXSTR lpszCertBuffer = NULL);
	XBOOL BTorrent_Creator_GetTorrent(XNETHANDLE xhToken, LPCXSTR lpszBTFile = NULL, XCHAR* ptszMsgBuffer = NULL, int* pInt_MsgLen = NULL);
	XBOOL BTorrent_Creator_Destory(XNETHANDLE xhToken);
protected:
private:
	shared_mutex st_Locker;
private:
	unordered_map<XNETHANDLE, BTORRENT_CREATORINFO> stl_MapBTCreator;
};