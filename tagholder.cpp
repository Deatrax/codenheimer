#include "tagholder.h"

tagHolder::tagHolder(int n){
    if(n==0){
        noTags=true;
        return;
    }
    noTags=false;
    assist::make_appData_filePath(tagFile);
    std::ifstream inFile(tagFile, std::ios::in);
    if (!inFile.is_open()){
        std::cerr<<"failed to open the tagDat.cdh file\n";
        return;
    }
    for (int var = 0; var < n; ++var) {
        std::string str,tgName,tgColor;
        std::getline(inFile, str);
        qDebug("got line: %s",str.c_str());
        std::stringstream ss(str);
        std::getline(ss,tgName,',');
        std::getline(ss,tgColor,',');
        tag* tg=new tag;
        tg->hasData=true;
        tg->tagColor=tgColor;
        tg->tagName=tgName;
        tagStorage.emplace(tgName,tg);
    }
}

void tagHolder::insert(snippetBaseClass* snippet){
    std::cerr<<"entered the insert of tagHolder"<<std::endl;
    std::vector<std::string> gotTags=snippet->getTags();
    std::cerr<<"got the list of tags from the snippet"<<std::endl;
    for (int i = 0; i < gotTags.size(); i++) {
        //if(tagStorage[gotTags[i]].hasData)continue;
        if(tagStorage.find(gotTags[i])==tagStorage.end()){
            std::cerr<<"tag: "<<gotTags[i]<<" was not found in tag list"<<std::endl;
            continue;
        }
        storage[gotTags[i]].push_back(snippet);
    }
    std::cerr<<"loop completed, exiting tagHolder insert"<<std::endl;
}

std::vector<snippetBaseClass*>& tagHolder::getSnippetsFromTag(std::string tag){
    return storage[tag];
}

void tagHolder::getTagInfo(string tagName, std::string &passedName, std::string& passedColor){
    passedName=tagStorage[tagName]->tagName;
    passedColor=tagStorage[tagName]->tagColor;
}

tagHolder::tag* tagHolder::operator[](std::string str) {
    auto it = tagStorage.find(str);
    if (it != tagStorage.end()) {
        return it->second; // Return the pointer if found
    }
    return nullptr;
}