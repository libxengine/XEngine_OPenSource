#include "pch.h"
#include "PluginExtension_LuaCore.h"
/********************************************************************
//    Created:     2026/01/27  15:31:34
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\PluginExtension_LuaCore\PluginExtension_LuaCore.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_PluginExtension\PluginExtension_LuaCore
//    File Base:   PluginExtension_LuaCore
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     LUA脚本插件
//    History:
*********************************************************************/
CPluginExtension_LuaCore::CPluginExtension_LuaCore()
{
}
CPluginExtension_LuaCore::~CPluginExtension_LuaCore()
{
}
//////////////////////////////////////////////////////////////////////////
//                       公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：PluginExtension_LuaCore_Init
函数功能：初始化插件核心系统
返回值
  类型：逻辑型
  意思：是否成功初始化
备注：
*********************************************************************/
bool CPluginExtension_LuaCore::PluginExtension_LuaCore_Init()
{
    PluginExtension_IsErrorOccur = false;
    return true;
}
/********************************************************************
函数名称：PluginExtension_LuaCore_Push
函数功能：添加一个标准的插件到插件框架中
 参数.一：pxhModule
  In/Out：Out
  类型：模块句柄
  可空：N
  意思：导出一个加载成功的模块句柄
 参数.二：lpszPluginFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：插件模块路径
 参数.三：pSt_PluginParameter
  In/Out：In
  类型：数据结构指针
  可空：Y
  意思：输入插件初始化参数
返回值
  类型：逻辑型
  意思：是否成功添加
备注：
*********************************************************************/
bool CPluginExtension_LuaCore::PluginExtension_LuaCore_Push(XNETHANDLE* pxhModule, LPCXSTR lpszPluginFile, XENGINE_PLUGINPARAM* pSt_PluginParameter)
{
    PluginExtension_IsErrorOccur = false;

    if (!BaseLib_Handle_Create(pxhModule))
    {
        PluginExtension_IsErrorOccur = true;
        PluginExtension_dwErrorCode = BaseLib_GetLastError();
        return false;
    }
    if (!PluginExtension_LuaCore_Add(*pxhModule, lpszPluginFile, pSt_PluginParameter))
    {
        return false;
    }
    return true;
}
/********************************************************************
函数名称：PluginExtension_LuaCore_Exec
函数功能：调用一次插件
 参数.一：xhModule
  In/Out：In
  类型：句柄
  可空：N
  意思：输入模块句柄
 参数.二：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出负载的内容
 参数.三：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出内容大小
 参数.四：lpszMsgBufer
  In/Out：Out
  类型：常量字符指针
  可空：Y
  意思：输入负载内容
 参数.五：nMsgLen
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输入负载大小
 参数.六：pppInputParameters
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入参数列表
 参数.七：nInputPCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
 参数.八：pppOutputParameters
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出参数列表
 参数.九：pInt_OutputPCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CPluginExtension_LuaCore::PluginExtension_LuaCore_Exec(XNETHANDLE xhModule, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBufer, int nMsgLen, XCHAR*** pppInputParameters, int nInputPCount, XCHAR*** pppOutputParameters, int* pInt_OutputPCount)
{
    PluginExtension_IsErrorOccur = false;

    st_csStl.lock_shared();
	//执行指定插件函数
	unordered_map<XNETHANDLE, PLUGINCORE_LUAFRAMEWORK>::const_iterator stl_MapIterator = stl_MapFrameWork.find(xhModule);
	if (stl_MapIterator == stl_MapFrameWork.end())
	{
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_NOTFOUND;
		st_csStl.unlock_shared();
		return false;
	}
#ifdef _XENGINE_BUILD_SWITCH_LUA
	if (0 == lua_getglobal(stl_MapIterator->second.pSt_LuaState, "PluginCore_Call"))
	{
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_FPCALL;
		st_csStl.unlock_shared();
		return false;
	}
	//将输入参数逐个压入 Lua table
	lua_createtable(stl_MapIterator->second.pSt_LuaState, nInputPCount - 1, 0);
	for (int i = 1; i < nInputPCount; i++)
	{
		lua_pushstring(stl_MapIterator->second.pSt_LuaState, (*pppInputParameters)[i]);
		lua_rawseti(stl_MapIterator->second.pSt_LuaState, -2, i); // table[i] = param
	}
    lua_pushinteger(stl_MapIterator->second.pSt_LuaState, nInputPCount - 1);
    lua_pushstring(stl_MapIterator->second.pSt_LuaState, lpszMsgBufer);
    lua_pushinteger(stl_MapIterator->second.pSt_LuaState, nMsgLen);

    if (LUA_OK != lua_pcall(stl_MapIterator->second.pSt_LuaState, 4, 4, 0))
    {
		const char* errMsg = lua_tostring(stl_MapIterator->second.pSt_LuaState, -1);
		printf("Lua error: %s\n", errMsg ? errMsg : "unknown error");
		lua_pop(stl_MapIterator->second.pSt_LuaState, 1); // 弹出错误信息

		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_EXECTION;
		st_csStl.unlock_shared();
		return false;
    }
	// 调用后栈布局：
// [-4] outputTable
// [-3] msgBuffer
// [-2] msgLen
// [-1] retCode   <-- 栈顶，最先取
// 
	// 取 retCode（可根据需要判断）
	bool bRet = true;
	if (lua_isboolean(stl_MapIterator->second.pSt_LuaState, -1))
	{
		bRet = lua_toboolean(stl_MapIterator->second.pSt_LuaState, -1) != 0;
	}
	lua_pop(stl_MapIterator->second.pSt_LuaState, 1);

	//取回数据
	if (lua_isinteger(stl_MapIterator->second.pSt_LuaState, -1))
	{
		*pInt_MsgLen = (int)lua_tointeger(stl_MapIterator->second.pSt_LuaState, -1);
	}
	lua_pop(stl_MapIterator->second.pSt_LuaState, 1);

	if (*pInt_MsgLen > 0 && lua_isstring(stl_MapIterator->second.pSt_LuaState, -1))
	{
		_tcsxcpy(ptszMsgBuffer, lua_tostring(stl_MapIterator->second.pSt_LuaState, -1));
	}
	lua_pop(stl_MapIterator->second.pSt_LuaState, 1);
	//取输出参数 table
	if (NULL != pInt_OutputPCount)
	{
		*pInt_OutputPCount = 0;
		if (lua_istable(stl_MapIterator->second.pSt_LuaState, -1))
		{
			int nCount = (int)luaL_len(stl_MapIterator->second.pSt_LuaState, -1);
			*pInt_OutputPCount = nCount;

			if (nCount > 0 && pppOutputParameters != nullptr)
			{
				// 分配输出参数数组（调用方负责释放）
				BaseLib_Memory_Malloc((XPPPMEM)pppOutputParameters, nCount, XPATH_MAX);
				for (int i = 0; i < nCount; i++)
				{
					lua_rawgeti(stl_MapIterator->second.pSt_LuaState, -1, i + 1);  // 取 table[i]
					LPCXSTR lpszValueStr = lua_tostring(stl_MapIterator->second.pSt_LuaState, -1);
					if (NULL != lpszValueStr)
					{
						_tcsxcpy((*pppOutputParameters)[i], lpszValueStr);
					}
					lua_pop(stl_MapIterator->second.pSt_LuaState, 1);
				}
			}
		}
		lua_pop(stl_MapIterator->second.pSt_LuaState, 1); // pop outputTable
	}
	
	st_csStl.unlock_shared();
#endif
    return true;
}
/********************************************************************
函数名称：PluginExtension_LuaCore_Get
函数功能：获取插件基础信息函数
 参数.一：xhModule
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的模块句柄
 参数.二：ptszPluginName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：处理名称
 参数.三：ptszPluginVersion
  In/Out：Out
  类型：字符指针
  可空：N
  意思：版本号.使用x.x.x.x 格式
 参数.四：ptszPluginAuthor
  In/Out：Out
  类型：字符指针
  可空：N
  意思：作者
 参数.五：ptszPluginDesc
  In/Out：Out
  类型：字符指针
  可空：N
  意思：插件描述
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CPluginExtension_LuaCore::PluginExtension_LuaCore_Get(XNETHANDLE xhModule, XCHAR* ptszPluginName, XCHAR* ptszPluginVersion, XCHAR* ptszPluginAuthor, XCHAR* ptszPluginDesc)
{
	PluginExtension_IsErrorOccur = false;

	st_csStl.lock_shared();
	unordered_map<XNETHANDLE, PLUGINCORE_LUAFRAMEWORK>::const_iterator stl_MapIterator = stl_MapFrameWork.find(xhModule);
	if (stl_MapIterator == stl_MapFrameWork.end())
	{
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_NOTFOUND;
		st_csStl.unlock_shared();
		return false;
	}
	_tcsxcpy(ptszPluginName, stl_MapIterator->second.tszModuleName);
	_tcsxcpy(ptszPluginVersion, stl_MapIterator->second.tszModuleVer);
	_tcsxcpy(ptszPluginAuthor, stl_MapIterator->second.tszModuleAuthor);
	_tcsxcpy(ptszPluginDesc, stl_MapIterator->second.tszModuleDesc);
	st_csStl.unlock_shared();
	return true;
}
/********************************************************************
函数名称：PluginExtension_LuaCore_Destroy
函数功能：销毁插件核心并且清理资源
返回值
  类型：逻辑型
  意思：是否成功销毁
备注：
*********************************************************************/
bool CPluginExtension_LuaCore::PluginExtension_LuaCore_Destroy()
{
    PluginExtension_IsErrorOccur = false;

#ifdef _XENGINE_BUILD_SWITCH_LUA
    //清理STL元素空间
    st_csStl.lock();
    unordered_map<XNETHANDLE, PLUGINCORE_LUAFRAMEWORK>::iterator stl_MapIterator = stl_MapFrameWork.begin();
    for (; stl_MapIterator != stl_MapFrameWork.end(); stl_MapIterator++)
    {
        lua_getglobal(stl_MapIterator->second.pSt_LuaState, "PluginCore_UnInit");
		lua_pcall(stl_MapIterator->second.pSt_LuaState, 0, 0, 0);

        lua_close(stl_MapIterator->second.pSt_LuaState);
    }
    stl_MapFrameWork.clear();
    st_csStl.unlock();
#endif
    return true;
}
//////////////////////////////////////////////////////////////////////////
//                       保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：PluginExtension_LuaCore_Add
函数功能：添加一个指定模块到插件核心系统当中
 参数.一：xhNet
  In/Out：In
  类型：模块句柄
  可空：N
  意思：输入一个新的模块句柄
 参数.二：lpszPluginFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：插件模块路径
 参数.三：pSt_PluginParameter
  In/Out：In
  类型：数据结构指针
  可空：Y
  意思：输入插件初始化参数
返回值
  类型：逻辑型
  意思：是否成功执行
备注：
*********************************************************************/
bool CPluginExtension_LuaCore::PluginExtension_LuaCore_Add(XNETHANDLE xhNet, LPCXSTR lpszPluginFile, XENGINE_PLUGINPARAM* pSt_PluginParameter /* = NULL */)
{
    PluginExtension_IsErrorOccur = false;

    if (NULL == lpszPluginFile)
    {
        PluginExtension_IsErrorOccur = true;
        PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_PARAMENT;
        return false;
    }
#ifdef _XENGINE_BUILD_SWITCH_LUA
    PLUGINCORE_LUAFRAMEWORK st_LuaCore;
    memset(&st_LuaCore, '\0', sizeof(PLUGINCORE_LUAFRAMEWORK));

    st_LuaCore.pSt_LuaState = luaL_newstate();
    _tcsxcpy(st_LuaCore.tszModuleFile, lpszPluginFile);

    if (NULL == st_LuaCore.pSt_LuaState)
	{
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_MALLOC;
		return false;
	}
	luaL_openlibs(st_LuaCore.pSt_LuaState);

    if (LUA_OK != luaL_loadfile(st_LuaCore.pSt_LuaState, lpszPluginFile))
    {
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_OPENDL;
		return false;
    }
	if (LUA_OK != lua_pcall(st_LuaCore.pSt_LuaState, 0, 0, 0))
	{
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_EXECTION;
		return false;
	}
	//得到初始化函数
    if (0 == lua_getglobal(st_LuaCore.pSt_LuaState, "PluginCore_Init"))
    {
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_FPINIT;
		return false;
    }
	lua_newtable(st_LuaCore.pSt_LuaState);

	lua_pushstring(st_LuaCore.pSt_LuaState, "APIVersion");
	lua_pushstring(st_LuaCore.pSt_LuaState, pSt_PluginParameter->tszAPIVersion);
	lua_settable(st_LuaCore.pSt_LuaState, -3);

	lua_pushstring(st_LuaCore.pSt_LuaState, "XEngineVer");
	lua_pushstring(st_LuaCore.pSt_LuaState, pSt_PluginParameter->tszXEngineVer);
	lua_settable(st_LuaCore.pSt_LuaState, -3);

	if (LUA_OK != lua_pcall(st_LuaCore.pSt_LuaState, 1, 1, 0))
	{
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_EXECTION;
		return false;
	}
    if (!lua_toboolean(st_LuaCore.pSt_LuaState, -1))
    {
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_EXECTION;
		return false;
    }
    lua_pop(st_LuaCore.pSt_LuaState, 1);
    //得到信息函数
	if (0 == lua_getglobal(st_LuaCore.pSt_LuaState, "PluginCore_GetInfo"))
	{
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_FPINIT;
		return false;
	}
	if (LUA_OK != lua_pcall(st_LuaCore.pSt_LuaState, 0, 4, 0))
	{
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_EXECTION;
		return false;
	}
	if (!lua_toboolean(st_LuaCore.pSt_LuaState, -1))
	{
		PluginExtension_IsErrorOccur = true;
		PluginExtension_dwErrorCode = ERROR_XENGINE_THIRDPART_PLUGIN_EXECTION;
		return false;
	}
	// Lua 栈从右到左是逆序压栈的，所以要从 -4 开始取
	_tcsxcpy(st_LuaCore.tszModuleName, lua_tostring(st_LuaCore.pSt_LuaState, -4));
	_tcsxcpy(st_LuaCore.tszModuleVer, lua_tostring(st_LuaCore.pSt_LuaState, -3));
	_tcsxcpy(st_LuaCore.tszModuleAuthor, lua_tostring(st_LuaCore.pSt_LuaState, -2));
	_tcsxcpy(st_LuaCore.tszModuleDesc, lua_tostring(st_LuaCore.pSt_LuaState, -1));
	lua_pop(st_LuaCore.pSt_LuaState, 4);

    st_csStl.lock();
    stl_MapFrameWork.insert(make_pair(xhNet, st_LuaCore));
    st_csStl.unlock();
#endif
    return true;
}