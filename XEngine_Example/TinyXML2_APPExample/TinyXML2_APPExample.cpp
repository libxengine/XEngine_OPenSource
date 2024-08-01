#include <stdio.h>
#include "../../XEngine_Module/tinyxml2/tinyxml2.h"

#ifdef _DEBUG
#pragma comment(lib,"../../Debug/tinyxml2")
#else
#pragma comment(lib,"../../Release/tinyxml2")
#endif

int main()
{
    tinyxml2::XMLDocument m_XMLDocument;
    m_XMLDocument.LoadFile("1.xml");


    return 0;
}
