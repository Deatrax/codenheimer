#include "snippetc.h"

snippetC::snippetC() {}

void snippetC::setEditor(QTextDocument* doc)
{
    highlighter = new KeywordHighlighterC(doc);
}
