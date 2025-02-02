#include "langholder.h"
/**
 * @brief The langHolder class is designed to manage and store snippets efficiently 
 *        based on their language or type. It facilitates quick access to snippets 
 *        categorized by language, which is necessary for browsing or searching
 *        in the application.\n 
 *
 * The class contains two internal private classes:
 *  - lang: A base class representing a generic language. It provides virtual methods for 
 *          managing and retrieving custom color data, allowing for extensibility.
 *  - customLang: A derived class from lang that implements functionality for handling 
 *                custom languages with specific color coding for syntax highlighting or 
 *                other purposes.
 *
 * @details Key Features:
 *  - Predefined languages: A set of common programming languages (e.g., C++, Python, Java) 
 *                          are preloaded for quick access.\n 
 *  - Custom languages: Users can define additional languages with custom color mappings, 
 *                      which are loaded from an external file.\n 
 *  - Snippet management: The class maintains a collection of snippets for each language, 
 *                        enabling efficient organization and retrieval.
 *
 * Usage:
 *  - Initialize the langHolder with the desired number of custom languages.
 *  - Insert snippets into the holder using the insert() method.
 *  - Retrieve snippets or perform operations like printing custom language data.
 */

class langHolder{
    private:
        /**
         * @brief The lang class serves as a base class for representing a language.
         *        It provides a virtual interface for managing custom colors and 
         *        storing associated snippets.
         */
        class lang{
            public:
                std::string langName;
                std::vector<snippetBaseClass*> snippetsStorage;

                virtual void putCustomColors(std::unordered_map<std::string,std::vector<std::string>>& passed){
                    //lol
                }

                virtual int getNoOfCustomColors(){
                    //lol
                }

                virtual std::string getNthCustomColor(int n){
                    //lol
                }

                virtual void printColorsOFCustomLang() const {
                    std::cerr<<"ERROR base class function was called"<<std::endl;
                }

                virtual std::unordered_map<std::string,std::vector<std::string>>& returnCustomColors(){
                    //lol
                }
        };

        class customLang: public lang{
            public:
                std::unordered_map<std::string,std::vector<std::string>> colorsData;
                void putCustomColors(std::unordered_map<std::string,std::vector<std::string>>& passed) override{
                    colorsData=passed;
                }

                int getNoOfCustomColors() override{
                    return colorsData.size();
                }

                std::string getNthCustomColor(int n) override {
                    auto itr=colorsData.begin();
                    std::advance(itr, n);

                    return itr->first;
                }

                void printColorsOFCustomLang() const override {
                    std::cout<<"derieved class function was called"<<std::endl;
                    for (const auto& pair : colorsData) {
                        std::cout << pair.first << ": ";
                        for (size_t i = 0; i < pair.second.size(); ++i) {
                            std::cout << pair.second[i];
                            if (i < pair.second.size() - 1) {
                                std::cout << ", ";
                            }
                        }
                        std::cout << std::endl;
                    }
                }

                std::unordered_map<std::string,std::vector<std::string>>& returnCustomColors(){
                    return colorsData;
                }
                
        };

        std::unordered_map<std::string, lang*>stringTolang;
        char langFile[500]="langDat.cdh";
        bool noAdditionallangs;

        void prepPredefinedLangs(){
            std::string langs[]={"cpp","c","java","css","py"};
            for (int i = 0; i < 5; i++)
            {
                lang* ty=new lang;
                ty->langName=langs[i];
                stringTolang.emplace(langs[i],ty);
            }
            

        }

    public:
        langHolder(int n){
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

        void insert(snippetBaseClass* snippet){
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

        std::vector<snippetBaseClass*>& getSnippetsFromLang(std::string lang){
            return stringTolang[lang]->snippetsStorage;
        }

        void testPrintCustomLang(const std::string& lng) {
            std::cout << "Test printing......." << std::endl;
            if (stringTolang.find(lng) == stringTolang.end()) {
                std::cerr << "Error: Language '" << lng << "' not found in stringTolang.\n";
                return;
            }
            stringTolang[lng]->printColorsOFCustomLang();
        }
};

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
