#include "snippetc.h"

snippetC::snippetC() {}

void snippetC::setEditor(QTextDocument* doc)
{
    highlighter = new KeywordHighlighter(doc);
}
