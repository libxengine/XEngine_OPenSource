CC = g++ -Wall -std=c++20 -fPIC
PLATFORM = linux
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./
LOADSO = 
LIB = -lXEngine_BaseLib
LIBEX = 
OBJECTS = AVPlayer_Video.o AVPlayer_Audio.o pch.o

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
	LIBEX = -lSDL2 -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=../XEngine_BaseLib,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	FILEEXT = dylib
	LOADHDR += -I /opt/homebrew/opt/sdl2/include
	LOADSO += -L /opt/homebrew/opt/sdl2/lib
	LIBFLAG = -dynamiclib
	LIBEX = -lSDL2 -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/../XEngine_BaseLib/
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_AVPlayer.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

AVPlayer_Video.o:./AVPlayer_Video/AVPlayer_Video.cpp                      
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./AVPlayer_Video/AVPlayer_Video.cpp   
AVPlayer_Audio.o:./AVPlayer_Audio/AVPlayer_Audio.cpp                      
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./AVPlayer_Audio/AVPlayer_Audio.cpp
	
pch.o:./pch.cpp             
	$(CC) $(DEBUG) $(UNICODE) $(FLAGS) $(PLATVER) $(LOADHDR) ./pch.cpp


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                        
CleanMk:
	rm *.$(FILEEXT)                                
