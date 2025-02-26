#include "langholder.h"

void langHolder::prepPredefinedLangs(){
    std::string langs[]={"cpp","c","java","css","py"};
    for (int i = 0; i < 5; i++)
    {
        lang* ty=new lang;
        ty->langName=langs[i];
        stringTolang.emplace(langs[i],ty);
    }
}


langHolder::langHolder(int n){
    prepPredefinedLangs();
    if(n==0){
        noAdditionallangs=true;
        return;
    }
    noAdditionallangs=false;
    assist::make_appData_filePath(langFile);
    std::ifstream inFile(langFile, std::ios::in);
    if (!inFile.is_open()){
        std::cerr<<"failed to open the langDat.cdh file\n";
        return;
    }
    for (int var = 0; var < n; ++var) {
        std::string line;
        std::getline(inFile, line);

        std::stringstream ss(line);
        std::string typeName, colorsPart;

        if (!std::getline(ss, typeName, '|') || !std::getline(ss, colorsPart, '|'))
        {
            std::cerr << "Invalid line format: " << line << "\n";
            continue;
        }
        std::cout << "TypeName: " << typeName << std::endl;
        std::cout << "Colors Part: " << colorsPart << std::endl;

        std::unordered_map<std::string, std::vector<std::string>> colorsDataParsed;
        std::stringstream colorsStream(colorsPart);
        std::string colorBlock;

        // Parse the color blocks
        while (std::getline(colorsStream, colorBlock, '|')) {
            std::stringstream colorStream(colorBlock);
            std::string colorName, wordList;

            // Parse color and its associated words
            if (std::getline(colorStream, colorName, ';') && std::getline(colorStream, wordList)) {
                std::stringstream wordsStream(wordList);
                std::string word;

                while (std::getline(wordsStream, word, ',')) {
                    colorsDataParsed[colorName].push_back(word);
                }

                std::cout << "Parsed color: " << colorName << std::endl;
                for (const auto &w : colorsDataParsed[colorName]) {
                    std::cout << "  Word: " << w << std::endl;
                }
            } else {
                std::cerr << "Invalid color block: " << colorBlock << "\n";
            }
        }


        lang* custom=new customLang;
        custom->langName=typeName;
        custom->putCustomColors(colorsDataParsed);
        stringTolang.emplace(typeName,custom);
    }
}


void langHolder::insert(snippetBaseClass* snippet){
    //DEBUG std::cerr<<"enetered insert of lang holder"<<std::endl;
    if (stringTolang.find(snippet->getLang()) != stringTolang.end()) {
        //DEBUG std::cerr<<"find condition passed in the lang holder"<<std::endl;
        stringTolang[snippet->getLang()]->snippetsStorage.push_back(snippet);
    } else {
        //DEBUG std::cerr << "Language " << snippet->getLang() << " not found in stringTolang!" << std::endl;
        return;
    }
    //DEBUG std::cerr<<"pushed back the snippet to where it belongs"<<std::endl;
    if (snippet->isCustom())
    {
        std::cerr<<"snippet is custom"<<std::endl;
        snippet->putColors(stringTolang[snippet->getLang()]->returnCustomColors());
    }
    //DEBUGstd::cerr<<"exiting insert of lang holder"<<std::endl;
}

void langHolder::insert(snippetBaseClass* snippet, std::string lang){

    if (stringTolang.find(lang) != stringTolang.end()) {
        //DEBUG std::cerr<<"find condition passed in the lang holder"<<std::endl;
        stringTolang[lang]->snippetsStorage.push_back(snippet);
    } else {
        //DEBUG std::cerr << "Language " << snippet->getLang() << " not found in stringTolang!" << std::endl;
        return;
    }
    //DEBUG std::cerr<<"pushed back the snippet to where it belongs"<<std::endl;
    if (snippet->isCustom())
    {
        std::cerr<<"snippet is custom"<<std::endl;
        snippet->putColors(stringTolang[lang]->returnCustomColors());
    }
    //DEBUGstd::cerr<<"exiting insert of lang holder"<<std::endl;
}

std::vector<snippetBaseClass*>& langHolder::getSnippetsFromLang(std::string lang){
    return stringTolang[lang]->snippetsStorage;
}

void langHolder::testPrintCustomLang(const std::string& lng) {
    std::cout << "Test printing......." << std::endl;
    if (stringTolang.find(lng) == stringTolang.end()) {
        std::cerr << "Error: Language '" << lng << "' not found in stringTolang.\n";
        return;
    }
    stringTolang[lng]->printColorsOFCustomLang();
}

int langHolder::getNoOfLangs()
{
    return stringTolang.size();
}

langHolder::lang* langHolder::getLangFromString(string str)
{
    auto it = stringTolang.find(str);
    if (it != stringTolang.end()) {
        return it->second;
    }
    else return nullptr;
}

std::vector<string> langHolder::getLangList()
{
    std::vector<std::string> keys;

    // Iterate and collect keys
    for (const auto& pair : stringTolang) {
        keys.push_back(pair.first);
    }

    return keys;
}

bool langHolder::removeSnippet(snippetBaseClass* obj){
    return stringTolang[obj->getLang()]->deleteSnippet(obj);
}

bool langHolder::rename(snippetBaseClass *obj)
{
    //lol names are not stored here
}
