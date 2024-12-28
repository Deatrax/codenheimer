#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <predefines.h>
#include <QFontDatabase>
#include "helperFunctions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "snippetbaseclass.h"
#include "snippetc.h"
#include "snippetcpp.h"
#include "snippetcss.h"
#include "snippetjava.h"
#include "snippetpy.h"
#include "snippetcustom.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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
            // std::cerr<<"enetered insert of lang holder"<<std::endl;
            if (stringTolang.find(snippet->getLang()) != stringTolang.end()) {
                // std::cerr<<"find condition passed in the lang holder"<<std::endl;
                stringTolang[snippet->getLang()]->snippetsStorage.push_back(snippet);
            } else {
                // std::cerr << "Language " << snippet->getLang() << " not found in stringTolang!" << std::endl;
                return;
            }
            // std::cerr<<"pushed back the snippet to where it belongs"<<std::endl;
            if (snippet->isCustom())
            {   
                std::cerr<<"snippet is custom"<<std::endl;
                snippet->putColors(stringTolang[snippet->getLang()]->returnCustomColors());
            }
            // std::cerr<<"exiting insert of lang holder"<<std::endl;
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
    //END OF PUBLIC
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QFont getFont(std::string str);
    void setMainIndex(int n);
    void readData();
    /**
     * @brief makes and prepares all the folders and files in the
     *  appdata directory for the app to use when the app is first 
     *  run by the user so that the app doesn't crash when it tries
     *  to access the data files it needs.  
     * 
     * @return int - different values based on the execution of the function
     */
    static int firstTimeInit();
    void readUconfig();
    void getTagInfo(string tagName, std::string &passedName, std::string &passedColor);
private slots:
    void on_sidebarButton_clicked();

    void on_AddnewSidebarButton_clicked();

    void on_SearchSidebarButton_clicked();

    void on_EditorsSidebarButton_clicked();

    void on_BrowseSidebarButton_clicked();

    void on_usernameAndMainSettingsButton_clicked();

    void on_centralSearchIcon_clicked();

    void on_centralSearchBoxLE_returnPressed();

private:
    Ui::MainWindow *ui;
    /// @brief this function encapsulates all the opertations that take place automatically first when the app is started
    void loadConfig();
    char coreUserConfig[1000];

    void sandBox();
protected:
    QFont CutiveMonoFont;
    QFont CreteRoundFont;
    int tagCount; //the number of tags that exists
    int additionalTypeCount; //number of user added types or languages
    std::string vaultLocation; //location of the snippet files
    std::string username; //username of the user
    std::string hashResult; //hashresult of the password
    tagHolder* mainTagHolder; //pointer to hold a tagHolder object to be added in readData() function
    langHolder* mainLangHolder; //pointer to hold a langHolder object to be added in readData() function
    std::vector<snippetBaseClass*> mainStorage; // a vector to hold all the snippet's pointers


    /// @brief loads all the custom fonts into the QFont objects that have been decleared in the header
    void loadCustomFonts();
    /// @brief centering the sidebar buttons
    void centreSidebarButtons();
    /// @brief assigning the icons of the sidebar buttons
    void setSidebarButtonIcons();
    /// @brief prepares the central area with the search, add new and browse islands
    void prepareCentralArea();
    /// @brief generates a snippet object like a predefined cpp, c, java, py, css or custom. 
    /// @param lang 
    /// @return returns a snippetBaseClass* that uses polymorphism to store a snippetliveclass derived object. 
    snippetBaseClass *generateSnippetObject(std::string lang);
};
#endif // MAINWINDOW_H
