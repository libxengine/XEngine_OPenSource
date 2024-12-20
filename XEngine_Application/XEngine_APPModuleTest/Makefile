CC = g++ -Wall -std=c++20
PLATFORM = linux
PLATVER = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./
LOADSO = -L ./
LIB = 	-lXEngine_BaseLib -lXEngine_BaseSafe -lXEngine_Algorithm                         \
		-lXEngine_Core -lXEngine_OPenSsl -lXEngine_ManagePool -lXEngine_Protocol         \
		-lXClient_Socket -lXClient_APIHelp -lXClient_OPenSsl -lXClient_Stream            \
		-lXEngine_ProcFile -lXEngine_SystemApi                                           \
		-lHelpComponents_BINPack -lHelpComponents_Compress -lHelpComponents_DataBase -lHelpComponents_Packets -lHelpComponents_WBlackList -lHelpComponents_XLog \
		-lNetHelp_APIFlow -lNetHelp_APIHelp -lNetHelp_XSocket                            \
		-lRfcComponents_HttpProtocol -lRfcComponents_MQTTProtocol -lRfcComponents_NatProtocol -lRfcComponents_NTPProtocol -lRfcComponents_ProxyProtocol -lRfcComponents_SIPPorotocol -lRfcComponents_SnmpProtocol -lRfcComponents_SSDPProtocol -lRfcComponents_WSProtocol \
		-lStreamMedia_FLVProtocol -lStreamMedia_HLSProtocol -lStreamMedia_MP4Protocol -lStreamMedia_RTCPProtocol -lStreamMedia_RTMPProtocol -lStreamMedia_RTPProtocol -lStreamMedia_RTSPProtocol -lStreamMedia_SDPProtocol  \
		-lXEngine_AudioCodec -lXEngine_AVCollect -lXEngine_AVFilter -lXEngine_AVFormat -lXEngine_AVFrame -lXEngine_AVHelp -lXEngine_VideoCodec
LIBEX = 
OBJECTS = XEngine_APPModuleTest.o

ifeq ($(RELEASE),1)
FLAGS = -c 
DEBUG = 
else
FLAGS = -c -lc_p
DEBUG = -g -pg 
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE = 
endif

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
	else
		PLATVER = -D __UBUNTU__
	endif
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o XEngine_ModuleTestAPP $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)
  
XEngine_APPModuleTest.o:./XEngine_APPModuleTest.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_APPModuleTest.cpp   


clean:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_ModuleTestAPP
	       
