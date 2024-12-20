#include "snippetcustom.h"

snippetCustom::snippetCustom() {}

bool snippetCustom::isCustom()
{
    return true;
}

void snippetCustom::putColors(std::unordered_map<std::string, std::vector<std::string>> &colorsData)
{
    this->colorsData=colorsData;
}

void snippetCustom::setEditor(QTextDocument *doc)
{
}
