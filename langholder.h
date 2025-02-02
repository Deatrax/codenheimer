#ifndef langholder_H
#define langholder_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include "snippetbaseclass.h"

class langHolder {
private:
    class lang {
    public:
        std::string langName;
        std::vector<snippetBaseClass*> snippetsStorage;

        virtual void putCustomColors(std::unordered_map<std::string, std::vector<std::string>>& passed) = 0;
        virtual int getNoOfCustomColors() = 0;
        virtual std::string getNthCustomColor(int n) = 0;
        virtual void printColorsOFCustomLang() const = 0;
        virtual std::unordered_map<std::string, std::vector<std::string>>& returnCustomColors() = 0;
        virtual ~lang() {}
    };

    class customLang : public lang {
    public:
        std::unordered_map<std::string, std::vector<std::string>> colorsData;
        void putCustomColors(std::unordered_map<std::string, std::vector<std::string>>& passed) override;
        int getNoOfCustomColors() override;
        std::string getNthCustomColor(int n) override;
        void printColorsOFCustomLang() const override;
        std::unordered_map<std::string, std::vector<std::string>>& returnCustomColors() override;
    };

    std::unordered_map<std::string, lang*> stringTolang;
    char langFile[500] = "langDat.cdh";
    bool noAdditionallangs;

    void prepPredefinedLangs();

public:
    langHolder(int n);
    void insert(snippetBaseClass* snippet);
    std::vector<snippetBaseClass*>& getSnippetsFromLang(std::string lang);
    void testPrintCustomLang(const std::string& lng);
};

#endif // langholder_H
