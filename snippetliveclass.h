#ifndef SNIPPETLIVECLASS_H
#define SNIPPETLIVECLASS_H

#include <snippetbaseclass.h>

class snippetLiveClass : public snippetBaseClass
{
protected:
    std::vector<std::string> tags;
    bool isLockedVar;

    lol* thisLol;
public:
    snippetLiveClass();
    void innit
    (
        std::string nam,
        std::string filenam,
        int linNum,
        std::string lng,
        std::vector<std::string> tgs
    ) override;

    std::vector<std::string> *getInfo(
        std::string &nam,
        std::string &lng
    ) override;

    std::string getSnippet() override;

    std::vector<std::string> getTags() override;

    // virtual void setEditor(QTextDocument*) override;
    std::string getLang() override;

    virtual bool isCustom() override;

    void putColors(std::unordered_map<std::string,std::vector<std::string>>& colorsData) override;

    void putTags(std::vector<std::string>& foundTags) override;

    bool isLocked() override;

    // search class
    void insert(const std::string& str, snippetBaseClass* targ) override;

    void insert(std::string str, int n);
    
    bool search(const std::string& str, std::vector<snippetBaseClass*>& ret) override;
    
    void getData(std::string &str, int &n);

    std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>> searchWithPrefix(const std::string& prefix) override;
};

#endif // SNIPPETLIVECLASS_H
