CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ -I ../jsoncpp 
LOADSO = -L ../jsoncpp 
LIB = -lXEngine_BaseLib -lXEngine_SystemApi -lXClient_APIHelp -lNetHelp_XSocket -ljsoncpp
LIBEX = 
OBJECTS = InfoReport_APIMachine.o pch.o

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
	else ifeq ($(PLATFORM), ios)
		FLAGS = -c
		DEBUG = -g -D _DEBUG
	else
		FLAGS = -c -lc_p
		DEBUG = -g -pg -D _DEBUG
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
	else
		PLATVER = -D __UBUNTU__
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
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
else ifeq ($(PLATFORM), android)
	PLATVER = -D __ANDROID__
	FILEEXT = so
	LIBFLAG = -shared
	LOADHDR += -I $(HOME)/XEngine_Android_Arm64
	LOADSO += -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_BaseLib -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_SystemSdk -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_NetHelp -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_Client
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
	SDKPATH = $(HOME)/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
	CC = $(SDKPATH) -Wall -fPIC -std=c++20 -target aarch64-linux-android30
else ifeq ($(PLATFORM), ios)
	PLATVER = -D __IOS__
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LOADHDR += -I $(HOME)/XEngine_IOS_Arm64
	LOADSO += -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_BaseLib -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_SystemSdk -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_NetHelp -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_Client
	SDKPATH = /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk
	CC = g++ -Wall -fPIC -std=c++20 -arch arm64 -Wno-deprecated-declarations -isysroot ${SDKPATH} -miphoneos-version-min=16.0
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_InfoReport.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

InfoReport_APIMachine.o:./InfoReport_APIMachine/InfoReport_APIMachine.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./InfoReport_APIMachine/InfoReport_APIMachine.cpp 

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./pch.cpp


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(FILEEXT)                        
