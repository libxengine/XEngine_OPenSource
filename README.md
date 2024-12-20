[中文](README.md) ||  [English](README.en.md)  

# XEngine_OPenSource

本仓库是XEngine已经开源的模块和使用例子所在仓库  

## 介绍

开发语言:C/C++  
开发工具:vs2022  
支持系统:linux,macos,windows

## 目录结构

- XEngine_Application  工具库代码  
- XEngine_Module       模块源代码
- XEngine_Example      例子源代码

#### 模块结构
- XEngine_AVPlayer    音视频播放模块
- XEngine_InfoReport  信息报告模块
- jsoncpp             json模块
- tinyxml2            XML模块 

#### 工具结构
- XEngine_APPFileSort          文件排序工具
- XEngine_APPSimulateKeyInput  模拟键盘输入工具
- XEngine_APPSocketTest        网络测试工具(服务器和客户端)
- XEngine_APPModuleTest        XEngine库安装完整性检查程序

#### 例子结构
- AVCodec_APPPlayer       音视频播放器
- InfoReport_APPExample   报告模块例子

## 安装教程

#### XEngine环境

在编译之前必须安装XEngine,可以参考其Readme文档  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

###### 快速部署

git clone https://gitee.com/xyry/libxengine.git 或者 git clone https://github.com/libxengine/xengine.git  
window执行XEngine_WINEnv.bat 脚本  
Linux执行:sudo ./XEngine_LINEnv.sh -i 3  
macos执行:./XEngine_LINEnv.sh -i 3  

#### 开源库依赖

XEngine_AVPlayer 需要sdl2支持  

#### Windows
使用VS打开并且编译,你需要优先配置XEngine环境才能使用  

#### Linux
Linux使用Makefile编译  
UBUNTU24.04

###### 编译命令

在XEngine_Source目录下执行命令  
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译

#### MacOS
使用makefile编译,控制台运行,需要mac 13以及以上版本

###### 编译命令

参考Linux

## 使用说明

1. 切换到MASTER分支
2. 下载代码
3. 编译
4. 安装
5. 运行

## 参与贡献

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request

## 关注我们

如果你觉得这个软件对你有帮助,请你给我们一个START吧  
也可以通过微信关注我们  
![qrcode](https://www.xyry.org/qrcode.jpg)

## 提交问题

如果你有问题或者发现程序有BUG和意见,可以在issues中提交