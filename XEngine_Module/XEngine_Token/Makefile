CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./
LOADSO = 
LIB = -lXEngine_BaseLib
LIBEX = 
OBJECTS = Session_Dynamic.o Session_Token.o pch.o

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
	LOADSO += -L $(HOME)/XEngine_Android_Arm64/XEngine_Android/XEngine_BaseLib
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
	SDKPATH = $(HOME)/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
	CC = $(SDKPATH) -Wall -fPIC -std=c++20 -target aarch64-linux-android30
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_Token.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

Session_Dynamic.o:./Session_Dynamic/Session_Dynamic.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./Session_Dynamic/Session_Dynamic.cpp 
Session_Token.o:./Session_Token/Session_Token.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./Session_Token/Session_Token.cpp 

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./pch.cpp


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(FILEEXT)                        
