#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QWidget>
#include <QTextDocument>
#include "snippetbaseclass.h"

namespace Ui {
class editorWidget;
}

class editorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit editorWidget(QWidget *parent = nullptr);
    ~editorWidget();

    QTextDocument *syntaxHighlightTarget();
    void assign(snippetBaseClass *snipObj, bool isOld);
private slots:
    void on_textEdit_textChanged();
    void on_editorSnippetSettingsButton_clicked();

    void on_editorCopyButton_clicked();

    void on_editorSaveButton_clicked();

private:
    Ui::editorWidget *ui;
    void synchronizeScroll(int value);
    snippetBaseClass* thisSnippet;
};

#endif // EDITORWIDGET_H
