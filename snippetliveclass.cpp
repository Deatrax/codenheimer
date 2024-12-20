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

void snippetLiveClass::getInfo(std::string& nam, std::string& lng, std::vector<std::string>&tgs)
{
    nam=name;
    lng=lang;
}

std::string snippetLiveClass::getSnippet()
{
    return SNIPPET;
}

std::vector<std::string> snippetLiveClass::getTags()
{
    return tags;
}

// void snippetLiveClass::setEditor(QTextDocument *)
// {

// }

std::string snippetLiveClass::getLang() {
    return lang;
}

bool snippetLiveClass::isCustom()
{
    return false;
}

void snippetLiveClass::putColors(std::unordered_map<std::string, std::vector<std::string>> &colorsData)
{
    return;
}

void snippetLiveClass::putTags(std::vector<std::string> &foundTags)
{
    tags=foundTags;
}
