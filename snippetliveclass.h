#ifndef SNIPPETLIVECLASS_H
#define SNIPPETLIVECLASS_H

#include <snippetbaseclass.h>

class snippetLiveClass : public snippetBaseClass
{
protected:
    std::vector<std::string> tags;
public:
    snippetLiveClass();
    virtual void innit
    (
        std::string nam,
        std::string filenam,
        int linNum,
        std::string lng,
        std::vector<std::string> tgs
    ) override;

    virtual void getInfo
    (
        std::string &nam,
        std::string &lng, 
        std::vector<std::string> &tgs
    ) override;

    virtual std::string getSnippet() override;

    virtual std::vector<std::string> getTags() override;
};

#endif // SNIPPETLIVECLASS_H
