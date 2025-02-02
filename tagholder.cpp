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

#include "tagholder.h"

class tagHolder{

    private:
        struct tag{
            bool hasData=false;
            std::string tagName;
            std::string tagColor;
        };

        std::unordered_map<std::string, std::vector<snippetBaseClass*>> storage;
        std::unordered_map<std::string,tag*>tagStorage;
        char tagFile[500]="tagDat.cdh";
        bool noTags;

    public:
        /**
         * @brief Construct a new tag Holder object. 
         * 
         * @param n takes in the number of tags and then reads them from the tagDat.cdh file. 
         */
        tagHolder(int n){
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

        /**
         * @brief inserts a snippet's pointer into the vector of the tag's hashmap
         * 
         * @param snippet 
         */
        void insert(snippetBaseClass* snippet){
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

        /**
         * @brief Get the Snippets From Tag object. Uses the unordered_map to get the pointer to the snippet
         * 
         * @param tag 
         * @return std::vector<snippetBaseClass*>& 
         */
        std::vector<snippetBaseClass*>& getSnippetsFromTag(std::string tag){
            return storage[tag];
        }

        void getTagInfo(string tagName, std::string &passedName, std::string& passedColor){
            passedName=tagStorage[tagName]->tagName;
            passedColor=tagStorage[tagName]->tagColor;
        }

        tag* operator[](std::string str) {
            auto it = tagStorage.find(str);
            if (it != tagStorage.end()) {
                return it->second; // Return the pointer if found
            }
            return nullptr;
        }
    //END OF PUBLIC
};
