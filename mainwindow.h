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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class langHolder{
    private:
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
            stringTolang[snippet->getLang()]->snippetsStorage.push_back(snippet);
            if (snippet->isCustom())
            {
                snippet->putColors(stringTolang[snippet->getLang()]->returnCustomColors());
            }
            
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
 * tag as well as store pointers to the corresponding snippet classes
 * 
 */
class tagHolder{

    private:
        struct tag{
            bool hasData=false;
            std::string tagName;
            std::string tagColor;
        };

        std::unordered_map<std::string, std::vector<snippetBaseClass*>> storage;
        std::unordered_map<std::string,tag>tagStorage;
        char tagFile[500]="tagDat.cdh";
        bool noTags;

    public:
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
                std::string str;
                std::getline(inFile, str);
                qDebug("got line: %s",str.c_str());
                //input logic is incomplete still
            }
        }

        void insert(snippetBaseClass* snippet){
            std::vector<std::string> gotTags=snippet->getTags();
            for (int i = 0; i < gotTags.size(); i++) {
                if(tagStorage[gotTags[i]].hasData)continue;
                storage[gotTags[i]].push_back(snippet);
            }
        }

        std::vector<snippetBaseClass*>& getSnippetsFromTag(std::string tag){
            return storage[tag];
        }
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
    static int firstTimeInit();
    void readUconfig();
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
    void loadConfig();
    char coreUserConfig[1000];

protected:
    QFont CutiveMonoFont;
    QFont CreteRoundFont;
    int tagCount;
    int additionalTypeCount;
    std::string vaultLocation;
    std::string username;
    std::string hashResult;
    tagHolder* mainTagHolder;
    langHolder* mainLangHolder;


    void loadCustomFonts();
    void centreSidebarButtons();
    void setSidebarButtonIcons();
    void prepareCentralArea();

};
#endif // MAINWINDOW_H
