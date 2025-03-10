#ifndef TAGHOLDER_H
#define TAGHOLDER_H

#include <string>
#include <vector>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <cstring>
#include <string>
#include <stack>
#include <queue>
#include <fstream>
#include <sstream>
#include "helperFunctions.h"
#include "snippetbaseclass.h"


/**
 * @brief a class for holing the info like tag name and the color of the 
 *  tag as well as store pointers to the corresponding snippet classes. 
 * 
 *  this is done by implementing an internal struct called tag that holds that
 *  tag's information: tag name, tag color. the tag's information can be accessed
 *  by using the hashmap (unordered_map) by passing the string of the tag name
 *  and getting a pointer to the tag in return. 
 *  
 *  the tag's information is held by a hashmap (unordered_map) with strings as keys and 
 *  vectors at the key location 
 */
class tagHolder{

    private:
        struct tag{
            bool hasData=false;
            std::string tagName;
            std::string tagColor;
        };

        std::unordered_map<std::string, std::vector<snippetBaseClass*>> storage;
        std::unordered_map<std::string,tag*>tagStorage;
        char tagFile[assist::PATH_SIZE]="tagDat.cdh";
        bool noTags;


    public:
        /**
         * @brief Construct a new tag Holder object. 
         * 
         * @param n takes in the number of tags and then reads them from the tagDat.cdh file. 
         */
        tagHolder(int n);

        /**
         * @brief inserts a snippet's pointer into the vector of the tag's hashmap
         * 
         * @param snippet 
         */
        void insert(snippetBaseClass* snippet);

        /**
         * @brief Get the Snippets From Tag object. Uses the unordered_map to get the pointer to the snippet
         * 
         * @param tag 
         * @return std::vector<snippetBaseClass*>& 
         */
        std::vector<snippetBaseClass*>& getSnippetsFromTag(std::string tag);

        void getTagInfo(string tagName, std::string &passedName, std::string& passedColor);

        std::vector<std::string> getTagList();

        tag* operator[](std::string str);

        bool removeSnippet(snippetBaseClass* obj);
        void addTag(const std::string &tagName, const std::string &tagColor);
        void removeTag(const std::string &tagName);
    //END OF PUBLIC
};

#endif // TAGHOLDER_H
