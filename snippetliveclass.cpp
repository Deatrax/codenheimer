#include "snippetliveclass.h"

snippetLiveClass::snippetLiveClass() {}

void snippetLiveClass::innit(std::string nam, std::string filenam, int linNum, std::string lng, std::vector<std::string> tgs)
{
    name=nam;
    filename=filenam;
    lineNum=linNum;
    lang=lng;
    tags=tgs;

    size_t pos = filename.find_last_of('.');

    if (pos != std::string::npos) {
        // Extract the extension
        std::string extension = filename.substr(pos + 1);
        qDebug("got the file extension: %s",extension.c_str());
        // Perform actions based on the extension
        if (extension == "scdh") {
            isLockedVar=false;
        } else if (extension == "pcdh") {
            isLockedVar=true;
        }
    }


    std::cout << "Snippet Initialized: " << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Filename: " << filename << std::endl;
    std::cout << "Line Number: " << lineNum << std::endl;
    std::cout << "Language: " << lang << std::endl;
    std::cout << "Locked Status: " << isLockedVar << std::endl;

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

std::vector<std::string>* snippetLiveClass::getInfo(std::string& nam, std::string& lng)
{
    nam=name;
    lng=lang;
    return &tags;
}

std::string snippetLiveClass::getSnippet()
{
    return SNIPPET;
}

std::vector<std::string> snippetLiveClass::getTags()
{
    return tags;
}

bool search(const std::string &str,   std::vector<snippetBaseClass*> ret){
    return true;
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

bool snippetLiveClass::isLocked()
{
    return isLockedVar;
}

//search option
void snippetLiveClass::insert(const std::string &str, snippetBaseClass *targ){
    //absolutely nothing;
}

void snippetLiveClass::insert(std::string str, int n){
    thisLol=new lol(str,n);
}

bool snippetLiveClass::search(const std::string& str, std::vector<snippetBaseClass*>& ret)
{
    return false;
}

void snippetLiveClass::getData(std::string& str, int& n){
    str=thisLol->str;
    n=thisLol->num;
}

std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>> snippetLiveClass::searchWithPrefix(const std::string &prefix)
{
    return std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>>();
}
