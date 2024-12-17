#ifndef SNIPPETBASECLASS_H
#define SNIPPETBASECLASS_H

#include <string>
// #include <QString>
#include <iostream>


class snippetBaseClass
{

protected:
    std::string name;
    std::string filename;
    int lineNum;
    std::string lang;
    std::string SNIPPET;
    std::vector<std::string> tags;

public:
    snippetBaseClass();

    virtual void innit(
        std::string nam,
        std::string filenam,
        int linNum,
        std::string lng,
        std::vector<std::string> tgs
    )=0;

    virtual void getDat(
        std::string &nam,
        std::string &lng,
        std::vector<std::string> &tgs
    )=0;

    virtual std::string getSnippet()=0;
    
};

#endif // SNIPPETBASECLASS_H
