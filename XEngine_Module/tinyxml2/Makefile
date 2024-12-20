CC = g++ -Wall -std=c++20 -fPIC
PLATFORM = linux
PLATVER = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
LOADHDR = -I ./ 
LOADSO = -L 
LIB = 
LIBEX = 
OBJECTS = pch.o tinyxml2.o

ifeq ($(RELEASE),1)
FLAGS = -c 
DEBUG = 
else
	ifeq ($(PLATFORM),mac)
		FLAGS = -c
		DEBUG = -g 
	else
		FLAGS = -c -lc_p
		DEBUG = -g -pg 
	endif
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
	$(CC) $(DEBUG) $(OBJECTS) -o libtinyxml2.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(PLATVER) $(LOADHDR) pch.cpp
tinyxml2.o:tinyxml2.cpp
	$(CC) $(DEBUG) $(FLAGS) $(PLATVER) $(LOADHDR) tinyxml2.cpp

CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(FILEEXT)                                           
