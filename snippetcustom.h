#ifndef SNIPPETCUSTOM_H
#define SNIPPETCUSTOM_H

#include "snippetliveclass.h"

class snippetCustom : public snippetLiveClass
{

private:
    std::unordered_map<std::string,std::vector<string>> colorsData;

public:
    snippetCustom();
    bool isCustom() override;
    void putColors(std::unordered_map<std::string,std::vector<std::string>>& colorsData) override;
};

#endif // SNIPPETCUSTOM_H
