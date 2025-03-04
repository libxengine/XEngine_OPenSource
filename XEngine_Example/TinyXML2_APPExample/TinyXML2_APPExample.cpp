#include <stdio.h>
#include "../../XEngine_Module/tinyxml2/tinyxml2.h"

#ifdef _MSC_BUILD
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../../x64/Debug/tinyxml2.lib")
#elif _M_ARM64
#pragma comment(lib,"../../x64/ARM64/tinyxml2.lib")
#elif _M_IX86
#pragma comment(lib,"../../Debug/tinyxml2.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../../x64/Release/tinyxml2.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Release/tinyxml2.lib")
#elif _M_IX86
#pragma comment(lib,"../../Release/tinyxml2.lib")
#endif
#endif
#endif


int main()
{
    tinyxml2::XMLDocument m_XMLDocument;
    m_XMLDocument.LoadFile("1.xml");


    return 0;
}
