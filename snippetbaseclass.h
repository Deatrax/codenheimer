#ifndef SNIPPETBASECLASS_H
#define SNIPPETBASECLASS_H

#include <string>
#include <QString>
#include <iostream>


class snippetBaseClass
{

protected:
    std::string name;
    std::string filename;
    int lineNum;


public:
    snippetBaseClass();
};

#endif // SNIPPETBASECLASS_H
