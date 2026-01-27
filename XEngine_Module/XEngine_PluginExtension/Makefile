CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ -I /usr/include/lua5.4 -I /usr/local/include/lua
LOADSO = 
LIB = -lXEngine_BaseLib
LIBEX = 
OBJECTS = PluginExtension_LibCore.o PluginExtension_LuaCore.o PluginExtension_Loader.o pch.o

ifeq ($(RELEASE),1)
FLAGS = -c -D _RELEASE
DEBUG = -O2
else
	ifeq ($(PLATFORM),mac)
		FLAGS = -c
		DEBUG = -g -D _DEBUG
	else ifeq ($(PLATFORM), android)
		FLAGS = -c
		DEBUG = -g -D _DEBUG
	else
		FLAGS = -c
		DEBUG = -g -D _DEBUG
	endif
endif

ifeq ($(UNICODE),1)
override UNICODE := -D __UNICODE__
else
override UNICODE := -D __ANSI__
endif

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
		LIB += -llua
	else
		PLATVER = -D __UBUNTU__
		LIB += -llua5.4
	endif
	FILEEXT = so
	LIBFLAG = -shared
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LOADHDR += -I /opt/homebrew/opt/lua/include/lua
	LOADSO += -L /opt/homebrew/opt/lua/lib
	ifeq ($(MACRO_LUA_SUPPORT),1)
		LIB += -llua
	endif
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
else ifeq ($(PLATFORM), android)
	PLATVER = -D __ANDROID__
	FILEEXT = so
	LIBFLAG = -shared
	LOADHDR += -I $(HOME)/XEngine_Android_Arm64
	LOADSO += -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_BaseLib
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
	SDKPATH = $(HOME)/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
	CC = $(SDKPATH) -Wall -fPIC -std=c++20 -target aarch64-linux-android30
else ifeq ($(PLATFORM), ios)
	PLATVER = -D __IOS__
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LOADHDR += -I $(HOME)/XEngine_IOS_Arm64
	LOADSO += -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_BaseLib
	SDKPATH = /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk
	CC = g++ -Wall -fPIC -std=c++20 -arch arm64 -Wno-deprecated-declarations -isysroot ${SDKPATH} -miphoneos-version-min=16.0
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_ModulePlugin.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

PluginExtension_LibCore.o:./PluginExtension_LibCore/PluginExtension_LibCore.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./PluginExtension_LibCore/PluginExtension_LibCore.cpp
PluginExtension_LuaCore.o:./PluginExtension_LuaCore/PluginExtension_LuaCore.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./PluginExtension_LuaCore/PluginExtension_LuaCore.cpp
PluginExtension_Loader.o:./PluginExtension_Loader/PluginExtension_Loader.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./PluginExtension_Loader/PluginExtension_Loader.cpp 

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXEngine_ModulePlugin.$(FILEEXT)
	cp ./libXEngine_ModulePlugin.$(FILEEXT) ../../XEngine_Release/libXEngine_ModulePlugin.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(FILEEXT)                        