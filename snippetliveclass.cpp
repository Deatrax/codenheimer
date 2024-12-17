#include "snippetliveclass.h"

snippetLiveClass::snippetLiveClass() {}

void snippetLiveClass::innit(std::string nam, std::string filenam, int linNum, std::string lng, std::vector<std::string> tgs)
{
    name=nam;
    filename=filenam;
    lineNum=linNum;
    lang=lng;
    tags=tgs;
}

void snippetLiveClass::getDat(std::string& nam, std::string& lng, std::vector<std::string>&tgs)
{
    nam=name;
    lng=lang;
}

std::string snippetLiveClass::getSnippet()
{
    return SNIPPET;
}
