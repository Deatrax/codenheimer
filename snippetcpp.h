#ifndef SNIPPETCPP_H
#define SNIPPETCPP_H
#include "snippetliveclass.h"


#include <qscrollbar.h>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QColor>

class KeywordHighlighterCPP : public QSyntaxHighlighter {
public:
    KeywordHighlighterCPP(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent) {
        // Format for keywords (Blue: #569CD6)
        QTextCharFormat keywordFormat;
        keywordFormat.setForeground(QColor("#569CD6"));
        QStringList keywords = {
            "class", "public", "QSyntaxHighlighter", "QTextDocument", "QTextCharFormat",
            "QStringList", "const", "QString", "HighlightRule", "QRegularExpression",
            "append", "protected", "void", "override", "qAsConst", "QRegularExpressionMatchIterator",
            "globalMatch", "hasNext", "QRegularExpressionMatch", "next", "capturedStart",
            "capturedLength", "setFormat", "struct", "QVector", "nullptr", "Qt::blue",
            "Qt::darkMagenta", "private", "return", "include", "new", "delete", "namespace",
            "using", "template", "typename", "static", "auto", "bool", "char", "double", "long",
            "short", "unsigned", "signed", "virtual", "explicit", "friend", "operator", "this",
            "try", "catch", "throw", "default", "switch", "case", "break", "continue", "do",
            "else", "enum", "extern", "goto", "register", "sizeof", "typedef", "union",
            "volatile", "wchar_t", "true", "false" , "int" , "cout" , "vector" , "for"
        };

        for (const QString &keyword : keywords) {
            HighlightRule rule;
            rule.pattern = QRegularExpression("\\b" + keyword + "\\b"); // Matches whole words
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        // Format for identifiers/functions/types (Light Yellow: #DCDCAA)
        QTextCharFormat identifierFormat;
        identifierFormat.setForeground(QColor("#DCDCAA"));
        QStringList identifiers = {
            // General C++ Standard Functions
            "std::cout", "std::cin", "std::cerr", "std::endl", "std::swap", "std::move",
            "std::forward", "std::max", "std::min", "std::sort", "std::find", "std::for_each",
            "std::accumulate", "std::transform", "std::copy", "std::fill", "std::equal",
            "std::unique", "std::reverse", "std::distance", "std::advance", "std::next", "std::prev",
        
            // std::vector Member Functions
            "std::vector", "push_back", "pop_back", "insert", "erase", "size", "clear",
            "empty", "at", "operator[]", "front", "back", "begin", "end", "rbegin",
            "rend", "resize", "capacity", "reserve", "shrink_to_fit", "data", "assign",
        
            // std::map Member Functions
            "std::map", "insert", "erase", "find", "count", "size", "empty", "clear",
            "operator[]", "at", "begin", "end", "rbegin", "rend", "lower_bound",
            "upper_bound", "equal_range", "swap", "emplace", "emplace_hint",
        
            // std::set Member Functions
            "std::set", "insert", "erase", "find", "count", "size", "empty", "clear",
            "begin", "end", "rbegin", "rend", "lower_bound", "upper_bound", "equal_range",
            "swap", "emplace", "emplace_hint", "contains",
        
            // std::string Member Functions
            "std::string", "length", "size", "empty", "clear", "append", "insert",
            "erase", "replace", "substr", "find", "rfind", "find_first_of", "find_last_of",
            "find_first_not_of", "find_last_not_of", "compare", "c_str", "data", "begin",
            "end", "operator+", "operator=", "operator[]",
        
            // std::queue and std::stack Member Functions
            "std::queue", "push", "pop", "front", "back", "size", "empty",
            "std::stack", "push", "pop", "top", "size", "empty",
        
            // std::list Member Functions
            "std::list", "push_back", "push_front", "pop_back", "pop_front", "insert",
            "erase", "remove", "sort", "merge", "splice", "unique", "reverse", "clear",
            "size", "empty", "begin", "end", "rbegin", "rend",
        
            // std::unordered_map Member Functions
            "std::unordered_map", "insert", "erase", "find", "count", "size", "empty", "clear",
            "operator[]", "at", "begin", "end", "bucket_count", "load_factor", "max_load_factor",
            "rehash", "reserve",
        
            // Miscellaneous std Functions
            "std::unique_ptr", "std::shared_ptr", "std::weak_ptr", "std::make_unique",
            "std::make_shared", "std::enable_shared_from_this", "std::mutex", "std::lock_guard",
            "std::unique_lock", "std::thread", "std::async", "std::future", "std::promise",
            "std::condition_variable", "std::atomic", "std::bind", "std::function",
            "std::chrono", "std::this_thread::sleep_for", "std::this_thread::sleep_until"
        };
        

        for (const QString &identifier : identifiers) {
            HighlightRule rule;
            rule.pattern = QRegularExpression("\\b" + identifier + "\\b");
            rule.format = identifierFormat;
            highlightingRules.append(rule);
        }

        // Format for String literals (Brown/Tan: #CE9178)
        QTextCharFormat stringFormat;
        stringFormat.setForeground(QColor("#CE9178"));
        QStringList stringLiterals = {
            "\"int\"", "\"string\"", "\"float\"", "\"for\"", "\"while\"", "\"if\""
        };

        for (const QString &literal : stringLiterals) {
            HighlightRule rule;
            rule.pattern = QRegularExpression(QRegularExpression::escape(literal)); // Exact match
            rule.format = stringFormat;
            highlightingRules.append(rule);
        }

        // Format for comments (Green: #6A9955)
        QTextCharFormat commentFormat;
        commentFormat.setForeground(QColor("#6A9955"));
        HighlightRule commentRule;
        commentRule.pattern = QRegularExpression("//.*"); // Matches single-line comments
        commentRule.format = commentFormat;
        highlightingRules.append(commentRule);
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

class snippetCPP : public snippetLiveClass
{
private:
    KeywordHighlighterCPP *highlighter;

public:
    snippetCPP();
    void setEditor(QTextDocument*doc) override;

};

#endif // SNIPPETCPP_H
