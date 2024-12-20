#ifndef SNIPPETPY_H
#define SNIPPETPY_H
#include "snippetliveclass.h"


#include <qscrollbar.h>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
class KeywordHighlighterPY : public QSyntaxHighlighter {
public:
    KeywordHighlighterPY(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent) {
        // Format for keywords like int and string
        QTextCharFormat blueFormat;
        blueFormat.setForeground(Qt::blue);
        QStringList blueKeywords = {"int", "string", "float"};

        for (const QString &keyword : blueKeywords) {
            HighlightRule rule;
            rule.pattern = QRegularExpression("\\b" + keyword + "\\b");  // Matches whole words
            rule.format = blueFormat;
            highlightingRules.append(rule);
        }

        // Format for control statements like for and while
        QTextCharFormat purpleFormat;
        purpleFormat.setForeground(Qt::darkMagenta);
        QStringList purpleKeywords = {"for", "while", "if"};

        for (const QString &keyword : purpleKeywords) {
            HighlightRule rule;
            rule.pattern = QRegularExpression("\\b" + keyword + "\\b");
            rule.format = purpleFormat;
            highlightingRules.append(rule);
        }
    }

protected:
    void highlightBlock(const QString &text) override {
        for (const HighlightRule &rule : qAsConst(highlightingRules)) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }

private:
    struct HighlightRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightRule> highlightingRules;
};


class snippetPY : public snippetLiveClass
{
    private:
    KeywordHighlighterPY *highlighter;
public:
    snippetPY();
    void setEditor(QTextDocument*doc) override;

};

#endif // SNIPPETPY_H
