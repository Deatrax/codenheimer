#ifndef SNIPPETLIVECLASS_H
#define SNIPPETLIVECLASS_H

#include <snippetbaseclass.h>

class snippetLiveClass : public snippetBaseClass
{
protected:
    std::vector<std::string>tags;
public:
    snippetLiveClass();
    void innit(std::string nam, std::string flnam, int ln);
    void addTag(std::string tag);
};

#endif // SNIPPETLIVECLASS_H
