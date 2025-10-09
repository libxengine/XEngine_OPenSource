#include "pch.h"
#include "Verification_XAuthNet.h"
/********************************************************************
//    Created:     2025/09/30  16:55:12
//    File Name:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth\Verification_XAuthNet.cpp
//    File Path:   D:\XEngine_OPenSource\XEngine_Module\XEngine_Verification\Verification_XAuth
//    File Base:   Verification_XAuthNet
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     XAuth网络验证
//    History:
*********************************************************************/
CVerification_XAuthNet::CVerification_XAuthNet()
{
}
CVerification_XAuthNet::~CVerification_XAuthNet()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
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
bool CVerification_XAuthNet::Verification_XAuthNet_Ver()
{
	Verification_IsErrorOccur = false;

	return true;
}