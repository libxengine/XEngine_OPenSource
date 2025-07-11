CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ 
LOADSO = 
LIB = 
LIBEX = 
OBJECTS = json_reader.o json_value.o json_writer.o

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
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
	SDKPATH = $(HOME)/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
	CC = $(SDKPATH) -Wall -fPIC -std=c++20 -target aarch64-linux-android30
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libjsoncpp.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

json_reader.o:json_reader.cpp
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) json_reader.cpp
json_value.o:json_value.cpp
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) json_value.cpp
json_writer.o:json_writer.cpp
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) json_writer.cpp


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(FILEEXT)                                           
