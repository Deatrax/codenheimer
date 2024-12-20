#ifndef SNIPPETBASECLASS_H
#define SNIPPETBASECLASS_H

#include <string>
// #include <QString>
#include <iostream>
#include <QTextDocument>
#include "helperFunctions.h"

class snippetBaseClass
{

protected:
    std::string name;
    std::string filename;
    int lineNum;
    std::string lang;
    std::string SNIPPET;

public:
    snippetBaseClass();

    virtual void innit(
        std::string nam,
        std::string filenam,
        int linNum,
        std::string lng,
        std::vector<std::string> tgs
    )=0;

    virtual void getInfo(
        std::string &nam,
        std::string &lng,
        std::vector<std::string> &tgs
    )=0;

    virtual std::string getSnippet()=0;

    virtual std::vector<std::string> getTags()=0;

    virtual void setEditor(QTextDocument*)=0;

    virtual std::string getLang()=0;
    
    virtual bool isCustom()=0;

    virtual void putColors(std::unordered_map<std::string,std::vector<std::string>>& colorsData)=0;

    virtual void putTags(std::vector<std::string>&)=0;
    
};

#endif // SNIPPETBASECLASS_H
