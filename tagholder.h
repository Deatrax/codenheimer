#ifndef tagholder_H
#define tagholder_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "snippetbaseclass.h"

class tagHolder {
    private:
        struct tag {
            bool hasData = false;
            std::string tagName;
            std::string tagColor;
        };

        std::unordered_map<std::string, std::vector<snippetBaseClass*>> storage;
        std::unordered_map<std::string, tag*> tagStorage;
        char tagFile[500] = "tagDat.cdh";
        bool noTags;
    
    public:
        tagHolder(int n);
        void insert(snippetBaseClass* snippet);
        std::vector<snippetBaseClass*>& getSnippetsFromTag(std::string tag);
        void getTagInfo(std::string tagName, std::string &passedName, std::string& passedColor);
        tag* operator[](std::string str);
};

#endif // tagholder_H
