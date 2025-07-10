CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ -I ../jsoncpp
LOADSO = -L ../jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_BaseSafe -lXClient_APIHelp -ljsoncpp
LIBEX = 
OBJECTS = AIApi_Chat.o AIApi_Image.o pch.o

ifeq ($(RELEASE),1)
FLAGS = -c -D _RELEASE
DEBUG = -O2
else
	ifeq ($(PLATFORM),mac)
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
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_AIApi.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

AIApi_Chat.o:./AIApi_Chat/AIApi_Chat.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./AIApi_Chat/AIApi_Chat.cpp 
AIApi_Image.o:./AIApi_Image/AIApi_Image.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./AIApi_Image/AIApi_Image.cpp 

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./pch.cpp


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(FILEEXT)                        
