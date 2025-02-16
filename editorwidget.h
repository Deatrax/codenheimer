#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QWidget>
#include <QTextDocument>
#include "snippetbaseclass.h"
#include "mainwindow.h"

namespace Ui {
class editorWidget;
}

class editorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit editorWidget(MainWindow* mainwindow, QWidget *parent = nullptr);
    ~editorWidget();

    QTextDocument *syntaxHighlightTarget();
    void assign(snippetBaseClass *snipObj, bool isOld);
    void tellIdx(int i);
private slots:
    void on_textEdit_textChanged();
    void on_editorSnippetSettingsButton_clicked();

    void on_editorCopyButton_clicked();

    void on_editorSaveButton_clicked();

    void on_tabCloseButton_clicked();

private:
    Ui::editorWidget *ui;
    void synchronizeScroll(int value);
    snippetBaseClass* thisSnippet;
    MainWindow* mainWindow;
    int idx;
};

#endif // EDITORWIDGET_H
