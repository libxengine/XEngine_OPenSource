[中文](README.md) ||  [English](README.en.md)  

# XEngine_OPenSource
This repository is the repository for XEngine's open source modules and examples  

## Introduction

Development language:c c++  
Development tools:vs2022  
support systems:linux,macos,windows

## Dir Struct

- XEngine_Application  tools Source  
- XEngine_Module       Module Source
- XEngine_Example      Example Source

#### Module Struct
- XEngine_Token       Handle Management module
- XEngine_InfoReport  information report module
- XEngine_MSGNotify   Message Notify Modules
- XEngine_AIApi       AI chat api module
- jsoncpp             json module
- tinyxml2            XML module

#### 工具结构
- XEngine_APPFileSort          file sort tools
- XEngine_APPSimulateKeyInput  Simulate keybroad Input Tools
- XEngine_APPSocketTest        network test tool(server and client)
- XEngine_APPModuleTest        XEngine install completed check

#### Example Struct
- InfoReport_APPExample   information report example
- MSGNotify_APPExample    message notify example
- TinyXML2_APPExample     xml module example
- AIApi_APPExample        ai module example

## install

#### XEngine environment

you have to install xengine before complie,You can refer to its Readme document  
GITEE:https://gitee.com/libxengine/libxengine  
GITHUB:https://github.com/libxengine/libxengine  

###### fast to deployment

git clone https://gitee.com/libxengine/libxengine.git or git clone https://github.com/libxengine/libxengine.git  
window Exection XEngine_WINEnv.bat  
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3  
Macos Exection:./XEngine_LINEnv.sh -i 3  

#### Windows
use vs open and complie  

#### Linux
Linux use Makefile complie  

##### Compile Command

execute command in XEngine_Source path   
make complie  

#### MacOS

Linux use Makefile to complie,terminal exection,Requires mac 13 and above 

##### Compile Command

make PLATFORM=mac

## Use

1.  Switch to the MASTER branch
2.  download code
3.  complie
4.  install
5.  run

## Participate in contribution

1.  Fork this code
2.  Create new Feat_xxx branch
3.  Submit the code
4.  New Pull Request

## Follow us

If you think this software is helpful to you, please give us a START  
You can also follow us via WeChat  
![qrcode](https://www.xyry.org/qrcode.jpg)

## post issues

if you have eny quest.post issues...