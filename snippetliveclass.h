#ifndef SNIPPETLIVECLASS_H
#define SNIPPETLIVECLASS_H

#include <snippetbaseclass.h>
#include "helperFunctions.h"
#include <sstream>

class snippetLiveClass : public snippetBaseClass
{
protected:
    std::vector<std::string> tags;
    bool isLockedVar;


public:
    snippetLiveClass();
    void innit
    (
        std::string nam,
        std::string filenam,
        int linNum,
        std::string lng,
        std::vector<std::string> tgs,
        QMainWindow* MM
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

// <<<<<<< ryexocious-making-search-page
    // search class
    void insert(const std::string& str, snippetBaseClass* targ) override;
    
    bool search(const std::string& str, std::vector<snippetBaseClass*>& ret) override;

    std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>> searchWithPrefix(const std::string& prefix) override;
// =======
    std::string& EditSnippet() override;

    bool saveSnippetToFile(std::string snippet) override;
    
    bool updateSnippetDetails(string nam,std::string filenam, std::vector<string> tgs, string lng, bool lock) override;
    // >>>>>>> main
    std::string getOldFilename() override;

    bool deleteFromVault() override;

    bool remove(snippetBaseClass *obj) override;
private:
    bool updateSnippetFilename(std::string newFilename) override;


};

#endif // SNIPPETLIVECLASS_H
