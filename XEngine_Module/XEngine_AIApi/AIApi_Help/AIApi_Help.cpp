#include "pch.h"
#include "AIApi_Help.h"
/********************************************************************
//    Created:     2025/07/10  15:46:31
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Help\AIApi_Help.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_AIApi\AIApi_Help
//    File Base:   AIApi_Help
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     AI帮助类
//    History:
*********************************************************************/
CAIApi_Help::CAIApi_Help()
{
}
CAIApi_Help::~CAIApi_Help()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AIApi_Help_Base64EncodecFile
函数功能：转换文件到BASE64请求格式
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取的文件
 参数.二：pptszMSGBuffer
  In/Out：Out
  类型：二级指针
  可空：N
  意思：输出转换后的缓冲区,此内存需要用户手动释放
 参数.三：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Help::AIApi_Help_Base64EncodecFile(LPCXSTR lpszFileName, XCHAR** pptszMSGBuffer, int* pInt_MSGLen)
{
	AIApi_IsErrorOccur = false;

	if ((NULL == lpszFileName))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_HELP_PARAMENT;
		return false;
	}
	//申请内存
	XCHAR* ptszSrcBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_LARGE);
	XCHAR* ptszDstBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_LARGE);
	if (NULL == ptszSrcBuffer || NULL == ptszDstBuffer)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_HELP_MALLOC;
		return false;
	}
	memset(ptszSrcBuffer, 0, XENGINE_MEMORY_SIZE_LARGE);
	memset(ptszDstBuffer, 0, XENGINE_MEMORY_SIZE_LARGE);
	//获取文件扩展名
	XCHAR tszFileExt[32] = {};
	BaseLib_String_GetFileAndPath(lpszFileName, NULL, NULL, NULL, tszFileExt);

	size_t nRet = _xsntprintf(ptszDstBuffer, XENGINE_MEMORY_SIZE_LARGE, _X("data:image/%s;base64,"), tszFileExt);
	//读取文件内容
	FILE* pSt_File = _xtfopen(lpszFileName, _X("rb"));
	if (NULL == pSt_File)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_HELP_OPENFILE;
		return false;
	}
	size_t nSize = fread(ptszSrcBuffer, 1, XENGINE_MEMORY_SIZE_LARGE, pSt_File);
	if (nSize <= 0)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_HELP_READFILE;
		return false;
	}
	fclose(pSt_File);

	if (!Cryption_Codec_Base64(ptszSrcBuffer, ptszDstBuffer + nRet, (int *)&nSize))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	free(ptszSrcBuffer);
	ptszSrcBuffer = NULL;
	*pptszMSGBuffer = ptszDstBuffer;
	*pInt_MSGLen = nRet + nSize;
	return true;
}
/********************************************************************
函数名称：AIApi_Help_Base64EncodecMemory
函数功能：转换内存数据到BASE64请求格式
 参数.一：lpszFileExt
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：文件的后缀名格式
 参数.二：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入内存缓冲区
 参数.三：nMSGLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.四：pptszMSGBuffer
  In/Out：Out
  类型：二级指针
  可空：N
  意思：输出转换后的缓冲区,此内存需要用户手动释放
 参数.五：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Help::AIApi_Help_Base64EncodecMemory(LPCXSTR lpszFileExt, LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR** pptszMSGBuffer, int* pInt_MSGLen)
{
	AIApi_IsErrorOccur = false;

	if ((NULL == lpszMSGBuffer))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_HELP_PARAMENT;
		return false;
	}
	//申请内存
	XCHAR* ptszDstBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_LARGE);
	if (NULL == ptszDstBuffer)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_HELP_MALLOC;
		return false;
	}
	memset(ptszDstBuffer, 0, XENGINE_MEMORY_SIZE_LARGE);

	size_t nRet = _xsntprintf(ptszDstBuffer, XENGINE_MEMORY_SIZE_LARGE, _X("data:image/%s;base64,"), lpszFileExt);

	if (!Cryption_Codec_Base64(lpszMSGBuffer, ptszDstBuffer + nRet, &nMSGLen))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	*pptszMSGBuffer = ptszDstBuffer;
	*pInt_MSGLen = nRet + nMSGLen;
	return true;
}
/********************************************************************
函数名称：AIApi_Help_Base64DecodecFile
函数功能：BASE64格式内存缓冲区数据转文件
 参数.一：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入内存缓冲区
 参数.二：nMSGLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要保存的文件地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Help::AIApi_Help_Base64DecodecFile(LPCXSTR lpszMSGBuffer, int nMSGLen, LPCXSTR lpszFileName)
{
	AIApi_IsErrorOccur = false;

	XCHAR* ptszMSGBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_LARGE);
	if (NULL == ptszMSGBuffer)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_HELP_MALLOC;
		return false;
	}
	memset(ptszMSGBuffer, 0, XENGINE_MEMORY_SIZE_LARGE);

	if (!Cryption_Codec_Base64(lpszMSGBuffer, ptszMSGBuffer, &nMSGLen, false))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	FILE* pSt_File = _xtfopen(lpszFileName, _X("wb"));
	if (NULL == pSt_File)
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = ERROR_XENGINE_MODULE_AIAPI_HELP_OPENFILE;
		return false;
	}
	fwrite(ptszMSGBuffer, 1, nMSGLen, pSt_File);
	fclose(pSt_File);

	free(ptszMSGBuffer);
	ptszMSGBuffer = NULL;
	return true;
}
/********************************************************************
函数名称：AIApi_Help_Base64DecodecMemory
函数功能：BASE64格式内存缓冲区转为内存数据
 参数.一：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入内存缓冲区
 参数.二：nMSGLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出转换后的缓冲区
 参数.四：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAIApi_Help::AIApi_Help_Base64DecodecMemory(LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	AIApi_IsErrorOccur = false;

	if (!Cryption_Codec_Base64(lpszMSGBuffer, ptszMSGBuffer, &nMSGLen, false))
	{
		AIApi_IsErrorOccur = true;
		AIApi_dwErrorCode = Cryption_GetLastError();
		return false;
	}
	*pInt_MSGLen = nMSGLen;
	return true;
}