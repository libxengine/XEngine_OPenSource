#include "pch.h"
#include "InfoReport_APIMachine.h"
/********************************************************************
//    Created:     2024/04/28  17:21:45
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_InfoReport\InfoReport_APIMachine\InfoReport_APIMachine.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_InfoReport\InfoReport_APIMachine
//    File Base:   InfoReport_APIMachine
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     信息收集报告类
//    History:
*********************************************************************/
CInfoReport_APIMachine::CInfoReport_APIMachine()
{

}
CInfoReport_APIMachine::~CInfoReport_APIMachine()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：InfoReport_APIMachine_Send
函数功能：发送一条信息报告给API服务器
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszServiceName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入上报的服务名称
 参数.三：pInt_TimeNumber
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出报告次数
返回值
  类型：逻辑型
  意思：是否成功
备注：lpszAPIUrl = _X("http://127.0.0.1:5501/api?function=machine");
*********************************************************************/
bool CInfoReport_APIMachine::InfoReport_APIMachine_Send(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName, __int64x* pInt_TimeNumber /* = NULL */)
{
	InfoReport_IsErrorOccur = false;

    if (NULL == lpszAPIUrl)
    {
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARAMENT;
        return false;
    }
	int nLen = 0;
	int nCode = 0;

	XCHAR tszAPIUrl[XPATH_MAX] = {};
	XCHAR tszOSName[128] = {};
	XCHAR tszOSVersion[128] = {};
	XCHAR tszOSBuild[128] = {};
	XCHAR tszUserName[128] = {};
	XCHAR tszComputerName[128] = {};
	XLONG nOSArch = 0;

	XCLIENT_APIHTTP st_HTTPParam = {};
	
	_xstprintf(tszAPIUrl, _X("%s&params1=0"), lpszAPIUrl);

	SystemApi_System_GetSystemVer(tszOSName, tszOSVersion, tszOSBuild, &nOSArch);
	SystemApi_System_GetSysName(tszUserName, tszComputerName);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::StreamWriterBuilder st_JsonBuilder;
	Json::CharReaderBuilder st_JsonReader;

	XCHAR tszMachineSoftware[8192] = {};
	XCHAR tszMachineHardware[8192] = {};
	InfoReport_APIMachine_Hardware(tszMachineHardware);
	InfoReport_APIMachine_Software(tszMachineSoftware);

	st_JsonRoot["tszServiceName"] = lpszServiceName;
	st_JsonRoot["tszMachineName"] = tszOSName;
	st_JsonRoot["tszMachineUser"] = tszUserName;
	st_JsonRoot["tszMachineSystem"] = tszComputerName;
	st_JsonRoot["tszMachineSoftware"] = tszMachineSoftware;
	st_JsonRoot["tszMachineHardware"] = tszMachineHardware;
	st_JsonBuilder["emitUTF8"] = true;

	XCHAR* ptszMsgBuffer = NULL;
	st_HTTPParam.nTimeConnect = 2000;
	if (!APIClient_Http_Request(_X("POST"), tszAPIUrl, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), &nCode, &ptszMsgBuffer, &nLen, NULL, NULL, &st_HTTPParam))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	st_JsonRoot.clear();
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonReader.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nLen, &st_JsonRoot, &st_JsonError))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARSE;
		return false;
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	if (0 != st_JsonRoot["code"].asInt())
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_CODE;
		return false;
	}
	if (!st_JsonRoot["data"].isNull() && (NULL != pInt_TimeNumber))
	{
		Json::Value st_JsonArray = st_JsonRoot["data"];
		*pInt_TimeNumber = st_JsonArray[0]["nTimeNumber"].asInt64();
	}
	
    return true;
}
/********************************************************************
函数名称：InfoReport_APIMachine_GetTime
函数功能：获取报告次数
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszServiceName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入服务名称
 参数.三：pInt_TimeNumber
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出报告次数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CInfoReport_APIMachine::InfoReport_APIMachine_GetTime(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName, __int64x* pInt_TimeNumber)
{
	InfoReport_IsErrorOccur = false;

	if (NULL == lpszAPIUrl)
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARAMENT;
		return false;
	}
	int nLen = 0;
	int nCode = 0;
	XCHAR tszAPIUrl[XPATH_MAX] = {};
	XCHAR tszComputerName[XPATH_MAX] = {};
	XCLIENT_APIHTTP st_HTTPParam = {};

	_xstprintf(tszAPIUrl, _X("%s&params1=2"), lpszAPIUrl);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::StreamWriterBuilder st_JsonBuilder;
	Json::CharReaderBuilder st_JsonReader;

	SystemApi_System_GetSysName(NULL, tszComputerName);

	st_JsonRoot["tszServiceName"] = lpszServiceName;
	st_JsonRoot["tszMachineSystem"] = tszComputerName;
	st_JsonBuilder["emitUTF8"] = true;

	XCHAR* ptszMsgBuffer = NULL;
	st_HTTPParam.nTimeConnect = 2000;
	if (!APIClient_Http_Request(_X("POST"), tszAPIUrl, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), &nCode, &ptszMsgBuffer, &nLen, NULL, NULL, &st_HTTPParam))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	st_JsonRoot.clear();
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonReader.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nLen, &st_JsonRoot, &st_JsonError))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARSE;
		return false;
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	if (0 != st_JsonRoot["code"].asInt())
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_CODE;
		return false;
	}
	Json::Value st_JsonArray = st_JsonRoot["data"];
	
	*pInt_TimeNumber = st_JsonArray[0]["nTimeNumber"].asInt64();
	return true;
}
/********************************************************************
函数名称：InfoReport_APIMachine_Delete
函数功能：发送一条删除请求给API服务器
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：lpszServiceName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入上报的服务名称
返回值
  类型：逻辑型
  意思：是否成功
备注：lpszAPIUrl = _X("http://127.0.0.1:5501/api?function=machine");
*********************************************************************/
bool CInfoReport_APIMachine::InfoReport_APIMachine_Delete(LPCXSTR lpszAPIUrl, LPCXSTR lpszServiceName)
{
	InfoReport_IsErrorOccur = false;

	if (NULL == lpszAPIUrl)
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARAMENT;
		return false;
	}
	int nLen = 0;
	int nCode = 0;
	XCHAR tszAPIUrl[XPATH_MAX] = {};
	XCHAR tszComputerName[XPATH_MAX] = {};
	XCLIENT_APIHTTP st_HTTPParam = {};

	_xstprintf(tszAPIUrl, _X("%s&params1=1"), lpszAPIUrl);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::StreamWriterBuilder st_JsonBuilder;
	Json::CharReaderBuilder st_JsonReader;

	SystemApi_System_GetSysName(NULL, tszComputerName);

	st_JsonRoot["tszServiceName"] = lpszServiceName;
	st_JsonRoot["tszMachineSystem"] = tszComputerName;
	st_JsonBuilder["emitUTF8"] = true;

	XCHAR* ptszMsgBuffer = NULL;
	st_HTTPParam.nTimeConnect = 2000;
	if (!APIClient_Http_Request(_X("POST"), tszAPIUrl, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), &nCode, &ptszMsgBuffer, &nLen, NULL, NULL, &st_HTTPParam))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	st_JsonRoot.clear();
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonReader.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nLen, &st_JsonRoot, &st_JsonError))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARSE;
		return false;
	}

	if (0 != st_JsonRoot["code"].asInt())
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_CODE;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：InfoReport_APIMachine_Hardware
函数功能：获取硬件信息
 参数.一：ptszHWInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的数据,这个数据是JSON格式
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：导出数据的长度
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CInfoReport_APIMachine::InfoReport_APIMachine_Hardware(XCHAR* ptszSWInfo, int* pInt_Len /* = NULL */)
{
	InfoReport_IsErrorOccur = false;

	if ((NULL == ptszSWInfo))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_CODE;
		return false;
	}
	int nDiskNumber = 0;
	XCHAR** pptszRootName;
	SYSTEMAPI_DISK_INFOMATION st_DiskInfo = {};
	SYSTEMAPI_CPU_INFOMATION st_CPUInfo = {};
	SYSTEMAPI_MEMORY_INFOMATION st_MemoryInfo = {};

	if (!SystemApi_HardWare_GetDiskNumber(&pptszRootName, &nDiskNumber))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = SystemApi_GetLastError();
		return false;
	}
	BaseLib_Memory_Free((XPPPMEM)&pptszRootName, nDiskNumber);

	XCHAR tszDriveStr[XPATH_MAX];
	memset(tszDriveStr, '\0', XPATH_MAX);
#ifdef _MSC_BUILD
	GetLogicalDriveStringsA(XPATH_MAX, tszDriveStr);
#else
	LPCXSTR lpszDir = _X("/");
	strcpy(tszDriveStr, lpszDir);
#endif

	if (!SystemApi_HardWare_GetDiskInfomation(tszDriveStr, &st_DiskInfo, XENGINE_SYSTEMSDK_API_SYSTEM_SIZE_MB))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = SystemApi_GetLastError();
		return false;
	}
	if (!SystemApi_HardWare_GetCpuInfomation(&st_CPUInfo))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = SystemApi_GetLastError();
		return false;
	}
	if (!SystemApi_System_GetMemoryUsage(&st_MemoryInfo, XENGINE_SYSTEMSDK_API_SYSTEM_SIZE_MB))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = SystemApi_GetLastError();
		return false;
	}

	Json::Value st_JsonRoot;
	Json::Value st_JsonDisk;
	Json::Value st_JsonCpu;
	Json::Value st_JsonSerial;
	Json::Value st_JsonMemory;
	Json::Value st_JsonNetCard;

	st_JsonDisk["DiskNumber"] = nDiskNumber;
	st_JsonDisk["DiskFree"] = (Json::UInt64)st_DiskInfo.dwDiskFree;
	st_JsonDisk["DiskTotal"] = (Json::UInt64)st_DiskInfo.dwDiskTotal;
	st_JsonDisk["DiskName"] = tszDriveStr;

	st_JsonCpu["CpuNumber"] = st_CPUInfo.nCPUNumber;
	st_JsonCpu["CpuSpeed"] = st_CPUInfo.nCPUSpeed;
	st_JsonCpu["CpuName"] = st_CPUInfo.tszCPUName;

	st_JsonMemory["MemoryFree"] = (Json::UInt64)st_MemoryInfo.dwMemory_Free;
	st_JsonMemory["MemoryTotal"] = (Json::UInt64)st_MemoryInfo.dwMemory_Total;

	int nListCount = 0;
	XSOCKET_CARDINFO** ppSt_ListIFInfo;
	XSocket_Api_GetCardInfo(&ppSt_ListIFInfo, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		Json::Value st_JsonIPAddr;
		st_JsonIPAddr["tszIFName"] = ppSt_ListIFInfo[i]->tszIFName;
		st_JsonIPAddr["tszIPAddr"] = ppSt_ListIFInfo[i]->tszIPAddr;
		st_JsonIPAddr["tszBroadAddr"] = ppSt_ListIFInfo[i]->tszBroadAddr;
		st_JsonIPAddr["tszDnsAddr"] = ppSt_ListIFInfo[i]->tszDnsAddr;
		st_JsonIPAddr["tszMacAddr"] = ppSt_ListIFInfo[i]->tszMacAddr;
		st_JsonNetCard.append(st_JsonIPAddr);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_ListIFInfo, nListCount);

	st_JsonRoot["Disk"] = st_JsonDisk;
	st_JsonRoot["Cpu"] = st_JsonCpu;
	st_JsonRoot["Memory"] = st_JsonMemory;
	st_JsonRoot["NetCard"] = st_JsonNetCard;

	if (NULL != pInt_Len)
	{
		*pInt_Len = st_JsonRoot.toStyledString().length();
	}
	memcpy(ptszSWInfo, st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());

	return true;
}
/********************************************************************
函数名称：InfoReport_APIMachine_Software
函数功能：获取软件系统信息
 参数.一：ptszSWInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出系统信息JSON结构
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：导出系统信息长度
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CInfoReport_APIMachine::InfoReport_APIMachine_Software(XCHAR* ptszSWInfo, int* pInt_Len /* = NULL */)
{
	InfoReport_IsErrorOccur = false;

	if ((NULL == ptszSWInfo))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARAMENT;
		return false;
	}
	int nProcessCount = 0;
	XLONG nOSProcessor = 0;
	XCHAR tszOSBuild[256] = {};
	XCHAR tszOSVersion[256] = {};
	XCHAR tszOSInfo[256] = {};
	XCHAR tszUPTime[256] = {};
	XENGINE_LIBTIME st_LibTimer = {};

	if (!SystemApi_System_GetSystemVer(tszOSInfo, tszOSVersion, tszOSBuild, &nOSProcessor))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = SystemApi_GetLastError();
		return false;
	}
	if (!SystemApi_System_GetProcessCount(&nProcessCount))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = SystemApi_GetLastError();
		return false;
	}
	if (!SystemApi_System_GetUpTime(&st_LibTimer))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = SystemApi_GetLastError();
		return false;
	}
	sprintf(tszUPTime, "%04d-%02d-%02d %02d:%02d:%02d", st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);

	Json::Value st_JsonRoot;
	Json::Value st_JsonOSObject;

	st_JsonOSObject["OSUPTime"] = tszUPTime;
	st_JsonOSObject["OSVersion"] = tszOSInfo;
	st_JsonOSObject["OSVersion"] = tszOSVersion;
	st_JsonOSObject["OSBuild"] = tszOSBuild;
	st_JsonOSObject["OSArch"] = (Json::Value::Int)nOSProcessor;
	st_JsonOSObject["OSProcessCount"] = nProcessCount;

	st_JsonRoot["OSInfo"] = st_JsonOSObject;

	if (NULL != pInt_Len)
	{
		*pInt_Len = st_JsonRoot.toStyledString().length();
	}
	memcpy(ptszSWInfo, st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());

	return true;
}