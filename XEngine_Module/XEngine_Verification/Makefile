CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ -I ../jsoncpp
LOADSO = -L ../jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_BaseSafe -lXEngine_Cryption -lXClient_Socket -lXClient_APIHelp -lRfcComponents_HttpProtocol -lXEngine_SystemConfig -lXEngine_SystemApi -ljsoncpp
LIBEX = 
OBJECTS = Verification_HTTP.o Verification_OAuth.o Verification_XAuthKey.o Verification_XAuthNet.o pch.o

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
	LOADSO += -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_BaseLib -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_Core -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_Client -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_RfcComponents -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_SystemSdk
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
	SDKPATH = $(HOME)/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
	CC = $(SDKPATH) -Wall -fPIC -std=c++20 -target aarch64-linux-android30
else ifeq ($(PLATFORM), ios)
	PLATVER = -D __IOS__
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LOADHDR += -I $(HOME)/XEngine_IOS_Arm64
	LOADSO += -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_BaseLib -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_Core -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_Client -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_RfcComponents -L $(HOME)/XEngine_IOS_Arm64/XEngine_IOS/XEngine_SystemSdk
	SDKPATH = /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk
	CC = g++ -Wall -fPIC -std=c++20 -arch arm64 -Wno-deprecated-declarations -isysroot ${SDKPATH} -miphoneos-version-min=16.0
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_Verification.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

Verification_HTTP.o:./Verification_HTTP/Verification_HTTP.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./Verification_HTTP/Verification_HTTP.cpp 
Verification_OAuth.o:./Verification_OAuth/Verification_OAuth.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./Verification_OAuth/Verification_OAuth.cpp 
Verification_XAuthKey.o:./Verification_XAuth/Verification_XAuthKey.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./Verification_XAuth/Verification_XAuthKey.cpp
Verification_XAuthNet.o:./Verification_XAuth/Verification_XAuthNet.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./Verification_XAuth/Verification_XAuthNet.cpp

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./pch.cpp


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(FILEEXT)                        
