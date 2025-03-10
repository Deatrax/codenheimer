#include "snippetc.h"

snippetC::snippetC()
: highlighter(nullptr)
{}

void snippetC::setEditor(QTextDocument* doc)
{
    highlighter = new KeywordHighlighterC(doc);
}
