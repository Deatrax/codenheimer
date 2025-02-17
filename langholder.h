#ifndef LANGHOLDER_H
#define LANGHOLDER_H

#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <set>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include <string>
#include <stack>
#include <queue>
#include <fstream>
#include <sstream>
#include "helperFunctions.h"
#include "snippetbaseclass.h"

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
    char langFile[assist::PATH_SIZE]="langDat.cdh";
    bool noAdditionallangs;

    void prepPredefinedLangs();

public:
    langHolder(int n);

    void insert(snippetBaseClass* snippet);

    std::vector<snippetBaseClass*>& getSnippetsFromLang(std::string lang);

    void testPrintCustomLang(const std::string& lng);

    int getNoOfLangs();

    lang *getLangFromString(std::string str);

    std::vector<std::string> getLangList();
};
#endif // LANGHOLDER_H
