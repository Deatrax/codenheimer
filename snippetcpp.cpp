#include "snippetcpp.h"

snippetCPP::snippetCPP()
: highlighter(nullptr)
{}

void snippetCPP::setEditor(QTextDocument* doc) {
    if (!highlighter) { // Ensure we don't create multiple instances
        highlighter = new KeywordHighlighterCPP(doc);
    } else {
        highlighter->setDocument(doc); // Reapply if already exists
    }
}
