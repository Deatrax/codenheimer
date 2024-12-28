#include "snippetliveclass.h"

snippetLiveClass::snippetLiveClass() {}

void snippetLiveClass::innit(std::string nam, std::string filenam, int linNum, std::string lng, std::vector<std::string> tgs)
{
    name=nam;
    filename=filenam;
    lineNum=linNum;
    lang=lng;
    tags=tgs;

    std::cout << "Snippet Initialized: " << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Filename: " << filename << std::endl;
    std::cout << "Line Number: " << lineNum << std::endl;
    std::cout << "Language: " << lang << std::endl;

    std::cout << "Tags: ";
    if (tags.empty()) {
        std::cout << "No tags" << std::endl;
    } else {
        for (const auto& tag : tags) {
            std::cout << tag << " ";
        }
        std::cout << std::endl;
    }
}

void snippetLiveClass::getInfo(std::string& nam, std::string& lng, std::vector<std::string>* tgs)
{
    nam=name;
    lng=lang;
    tgs=&tags;
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
    std::cerr<<"get lang in live class was called"<<std::endl;
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
