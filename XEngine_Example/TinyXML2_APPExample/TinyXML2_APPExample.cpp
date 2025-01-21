#include <stdio.h>
#include "../../XEngine_Module/tinyxml2/tinyxml2.h"

#ifdef _MSC_BUILD
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib,"../../x64/Debug/tinyxml2")
#else
#pragma comment(lib,"../../x64/Release/tinyxml2")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"../../Debug/tinyxml2")
#else
#pragma comment(lib,"../../Release/tinyxml2")
#endif
#endif
#endif



int main()
{
    tinyxml2::XMLDocument m_XMLDocument;
    m_XMLDocument.LoadFile("1.xml");


    return 0;
}
